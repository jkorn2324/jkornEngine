#include "EditorPCH.h"
#include "EditorLayer.h"

#include "Engine.h"
#include <imgui.h>

#include "Source\Vector.h"
#include "EditorUtils.h"
#include "EditorCamera.h"
#include "ImGuizmo.h"

#include "CameraController.h"

#include <string>

namespace Editor
{
	static const float TOP_WINDOW_HEIGHT = 60.0f;
	static const float WINDOW_HEIGHT_DIFFERENCE = 15.0f;

	static void SaveScene(const std::wstring& filePath)
	{
		Engine::Scene& scene = Engine::SceneManager::GetActiveScene();
		Engine::SceneSerializer sceneSerializer(&scene);
		sceneSerializer.Serialize(filePath + 
			scene.GetSceneName() + L".scene");
	}

	static void SaveScene()
	{
		SaveScene(L"");
	}


	static void DrawDemo()
	{
#if 0
		ImGui::ShowDemoWindow();
#endif
	}

	// TODO: Find a better way to get the main project menu path.

	EditorLayer::EditorLayer()
		: Layer("Editor"),
		m_sceneHierarchy(),
		m_entityInspector(),
		m_projectMenu(Engine::Application::Get().GetRootPath()),
		m_sceneView(),
		m_gameView()
	{
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnLayerAdded()
	{
		EditorSceneManager::Init();

		Engine::GraphicsRenderer::GetRenderingAPI().SetClearColor(
			MathLib::Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		// Sets the ambient light for the scene.
		Engine::GraphicsRenderer3D::SetAmbientLight(
			MathLib::Vector3 { 0.25f, 0.25f, 0.25f });
		
		// Creates a cube to test phong lighting calculations.
		{
			Engine::Scene& scene = Engine::SceneManager::GetActiveScene();

			// Add Behavior script to camera.
			{
				Engine::Entity entity = scene.Find("Main Camera");
				if (entity.IsValid())
				{
					Engine::BehaviorComponent& component
						= entity.GetComponent<Engine::BehaviorComponent>();
					component.Get().AddBehavior<CameraController>();
				}
			}


			Engine::Entity e = scene.CreateEntity("Test Cube");
			Engine::Transform3DComponent& component 
				= e.AddComponent<Engine::Transform3DComponent>();
			Engine::MeshComponent& meshComponent
				= e.AddComponent<Engine::MeshComponent>();

			meshComponent.material = Engine::AssetManager::GetMaterials().Cache(
				L"LitShader");
			meshComponent.material->SetConstantsLayout(
			{
				Engine::MaterialConstantLayoutAttribute { "c_diffuseColor", sizeof(MathLib::Vector4) },
				Engine::MaterialConstantLayoutAttribute { "c_specularColor", sizeof(MathLib::Vector4) },
				Engine::MaterialConstantLayoutAttribute { "c_specularPower", sizeof(float) },
				Engine::MaterialConstantLayoutAttribute { sizeof(MathLib::Vector3) }
			});

			Engine::MaterialConstants& constants = meshComponent.material->GetMaterialConstants();
			constants.SetMaterialConstant("c_diffuseColor", MathLib::Vector4::One);
			constants.SetMaterialConstant("c_specularPower", 10.0f);
			constants.SetMaterialConstant("c_specularColor", MathLib::Vector4::One);

			meshComponent.material->SetTexture(0,
				Engine::AssetManager::GetTextures().Load(L"Assets/brick-texture.png"));

			{
				// Ugly for now, but this will due for testing.
				struct CubeMeshVert
				{
					MathLib::Vector3 pos;
					MathLib::Vector3 normal;
					MathLib::Vector2 uv;
				};

				std::unique_ptr<Engine::BufferLayout> bufferLayout = Engine::BufferLayout::Create({
					{ "POSITION", offsetof(CubeMeshVert, pos),
						sizeof(MathLib::Vector3), Engine::BufferLayoutType::FLOAT3 },
					{ "NORMAL", offsetof(CubeMeshVert, normal),
						sizeof(MathLib::Vector3), Engine::BufferLayoutType::FLOAT3 },
					{ "TEXCOORD", offsetof(CubeMeshVert, uv),
						sizeof(MathLib::Vector2), Engine::BufferLayoutType::FLOAT2 }
				});

				Engine::Shader* shader = Engine::AssetManager::GetShaders().Load(
					L"Shaders/LitShader.hlsl", bufferLayout);
				meshComponent.material->SetShader(shader);
			}

			meshComponent.mesh = Engine::AssetManager::GetMeshes().Get(L"DefaultCube");
		}
	}

	void EditorLayer::OnLayerRemoved()
	{
		// TODO: Implementation
	}

	void EditorLayer::OnUpdate(const Engine::Timestep& timestep)
	{
		m_projectMenu.OnUpdate(timestep);
		
		// Updates the scene view.
		{
			m_sceneView.OnUpdate(timestep);
			EditorSceneManager::GetEditorCamera().OnEditorUpdate(timestep);
		}
		
		Engine::SceneManager::OnUpdate(timestep);

		if (EditorSceneManager::IsPlaying())
		{
			Engine::SceneManager::OnRuntimeUpdate(timestep);
		}
		else
		{
			Engine::SceneManager::OnEditorUpdate(timestep);
		}

		m_sceneView.RenderScene();
		m_gameView.RenderScene();
	}

	void EditorLayer::OnImGuiRender()
	{
		DrawDemo();

		DrawEditorTopWindow();
		DrawEditorMainWindow();
	}

	void EditorLayer::DrawEditorTopWindow()
	{
		// Draws the top mini-window with file menu & play button panels.
		{
			ImGuiWindowFlags topMiniWindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			topMiniWindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			topMiniWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
				| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize({ viewport->WorkSize.x, TOP_WINDOW_HEIGHT });
			ImGui::SetNextWindowViewport(viewport->ID);

			ImGui::Begin("editor-top", nullptr, topMiniWindowFlags);

			DrawMenuBar();
			DrawEditorButtons();

			ImGui::End();
		}
	}

	void EditorLayer::DrawMenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					SaveScene();
				}
				// TODO: Loading Scenes

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::BeginMenu("General"))
				{
					if (ImGui::MenuItem("Scene View"))
					{
						m_sceneView.SetOpen(true);
					}
					if (ImGui::MenuItem("Game View"))
					{
						m_gameView.SetOpen(true);
					}
					if (ImGui::MenuItem("Scene Hierarchy"))
					{
						m_sceneHierarchy.SetOpen(true);
					}
					if (ImGui::MenuItem("Entity Inspector"))
					{
						m_entityInspector.SetOpen(true);
					}
					if (ImGui::MenuItem("Project Menu"))
					{
						m_projectMenu.SetOpen(true);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void EditorLayer::DrawEditorButtons()
	{
		ImGui::BeginGroup();

		float windowHeight = ImGui::GetContentRegionAvail().y - 4.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (windowHeight * 0.5f));
		if (ImGui::ImageButton(nullptr, ImVec2{ windowHeight, windowHeight }))
		{
			if (EditorSceneManager::IsPlaying())
			{
				EditorSceneManager::SetPlaying(false);
			}
			else
			{
				EditorSceneManager::SetPlaying(true);
			}
		}
		ImGui::EndGroup();
	}

	void EditorLayer::DrawEditorMainWindow()
	{
		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		static bool optFullscreen = true;
		static bool optPadding = false;

		{
			if (optFullscreen)
			{
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos({ viewport->WorkPos.x,
					viewport->WorkPos.y + TOP_WINDOW_HEIGHT - WINDOW_HEIGHT_DIFFERENCE });
				ImGui::SetNextWindowSize({ viewport->WorkSize.x,
					(viewport->WorkSize.y - TOP_WINDOW_HEIGHT + WINDOW_HEIGHT_DIFFERENCE) });
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}
			else
			{
				dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
			}

			if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			{
				windowFlags |= ImGuiWindowFlags_NoBackground;
			}

			if (!optPadding)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			}
		}

		// Draws the editor dockspace.
		{
			ImGui::Begin("editor-main", nullptr, windowFlags);

			if (!optPadding) ImGui::PopStyleVar();
			if (optFullscreen) ImGui::PopStyleVar(2);

			// Apply the dockspace.
			{
				ImGuiID dockspace_id = ImGui::GetID("editor-dockspace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
			}

			m_sceneHierarchy.Draw();
			m_entityInspector.Draw();
			m_projectMenu.Draw(Engine::Application::Get().GetRootPath());
			m_sceneView.Draw();
			m_gameView.Draw();

			DrawEditorButtons();

			ImGui::End();
		}
	}
	
	void EditorLayer::OnEvent(Engine::Event& event)
	{
		EditorSceneManager::OnEvent(event);
		EditorSelection::OnEvent(event);

		m_sceneHierarchy.OnEvent(event);
		m_entityInspector.OnEvent(event);
		m_projectMenu.OnEvent(event);
		m_sceneView.OnEvent(event);
	}
}