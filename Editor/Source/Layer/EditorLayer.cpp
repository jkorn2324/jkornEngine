#include "EditorPCH.h"
#include "EditorLayer.h"

#include "Engine.h"
#include <imgui.h>

#include "Vector.h"

#include "EditorSelection.h"
#include "EditorSceneManager.h"
#include "EditorAssetManager.h"

#include "EditorCamera.h"
#include "ImGuizmo.h"

#include "CameraController.h"

#include "SystemManager.h"
#include "ISystemBase.h"
#include "SystemUtility.h"

#include <string>

namespace Editor
{

	static const float TOP_WINDOW_HEIGHT = 60.0f;
	static const float WINDOW_HEIGHT_DIFFERENCE = 15.0f;


	static void DrawDemo()
	{
#if 0
		ImGui::ShowDemoWindow();
#endif
	}

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
		// TODO: Initialize Systems

		// Sets the camera system.
		Engine::SystemManager::AddSystem<Editor::CameraControllerSystem>();
		Engine::SystemManager::AddSystem<Engine::EntityHierarchySystem>();

		Engine::GraphicsRenderer::GetRenderingAPI().SetClearColor(
			MathLib::Vector4(0.0f, 0.0f, 1.0f, 1.0f));

		// Tests the graphics
		// Sets the ambient light for the scene.
		Engine::GraphicsRenderer3D::SetAmbientLight(
			MathLib::Vector3 { 0.25f, 0.25f, 0.25f });

		// Creates a cube to test phong lighting calculations.
		{
			Engine::Scene& scene = Engine::SceneManager::GetActiveScene();

			// Add Behavior script to camera.
			{
				auto entity = scene.FindEntity<Engine::NameComponent>("Main Camera",
					[](const char* Name, const Engine::NameComponent& component) -> bool
					{
						return component.name == Name;
					});
				if (entity)
				{
					entity.AddComponent<CameraController>();
				}
			}

			auto e = scene.CreateEntity("Test Cube");
			Engine::Transform3DComponent& component
				= e.AddComponent<Engine::Transform3DComponent>();
			Engine::MeshComponent& meshComponent
				= e.AddComponent<Engine::MeshComponent>();

			// TODO: Temporary (Remove Soon), makes sure that the material is getting deleted.
			static std::shared_ptr<Engine::Material> material = std::make_shared<Engine::Material, Engine::MaterialConstantsLayout>(
				{
					{ "c_diffuseColor", Engine::LayoutType_Vector4 },
					{ "c_specularColor", Engine::LayoutType_Vector4 },
					{ "c_specularPower", Engine::LayoutType_Float },
					{ Engine::LayoutType_Vector3 }
				});
			meshComponent.material = material.get();

			meshComponent.material->SetConstantValue("c_diffuseColor", MathLib::Vector4::One);
			meshComponent.material->SetConstantValue("c_specularPower", 10.0f);
			meshComponent.material->SetConstantValue("c_specularColor", MathLib::Vector4::One);
			
			// TODO: Temporary, makes sure that it gets deleted when program ends.
			static std::shared_ptr<Engine::Texture> texture;
			texture.reset(Engine::Texture::LoadFromFile(L"Assets/brick-texture.png"));
			meshComponent.material->SetTexture(0, texture.get());

			{
				Engine::BufferLayout bufferLayout =
				{
					{
						Engine::BufferLayoutParam::Position0
					},
					{
						Engine::BufferLayoutParam::Normal0
					},
					{
						Engine::BufferLayoutParam::Uv0
					}
				};

				// TODO: Temporary, makes sure that it gets deleted when program ends.
				static std::shared_ptr<Engine::Shader> shader;
				shader.reset(Engine::Shader::LoadFromFile(
					L"Shaders/LitShader.hlsl", bufferLayout));
				meshComponent.material->SetShader(shader.get());
			}
			meshComponent.mesh = &Engine::GraphicsRenderer3D::GetCubeMesh();
		}
	}

	void EditorLayer::OnLayerRemoved()
	{
	}

	void EditorLayer::OnUpdate(const Engine::Timestep& timestep)
	{
		PROFILE_SCOPE(OnUpdate, EditorLayer);

		// For testing purposes.
		m_projectMenu.OnUpdate(timestep);
		m_gameView.OnUpdate(timestep);

		// Updates the scene view.
		{
			m_sceneView.OnUpdate(timestep);
			EditorSceneManager::GetEditorCamera().OnEditorUpdate(timestep);
		}

		// Calls update for the system.
		Engine::SystemUtility::InvokeOnUpdate(timestep, EditorSceneManager::IsPlaying());

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
		PROFILE_SCOPE(OnImGuiRender, Editor);

		DrawDemo();

		DrawEditorTopWindow();
		DrawEditorMainWindow();
	}

	// ---------------------- BEGIN EDITOR TOP WINDOW -------------------------

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
					std::filesystem::path currentPath(Engine::Application::Get().GetRootPath());
					if (m_projectMenu.IsOpen())
					{
						currentPath = m_projectMenu.GetCurrentPath();
					}
					EditorSceneManager::SaveScene(currentPath);
					// TODO: Asset Management
				}
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
			OnSceneRuntimeButtonSelected(!EditorSceneManager::IsPlaying());
		}
		ImGui::EndGroup();
	}

	// ---------------------- END EDITOR TOP WINDOW -------------------------

	void EditorLayer::OnSceneRuntimeButtonSelected(bool play)
	{
		EditorSceneManager::SetPlaying(play);
	}

	// ---------------------- BEGIN EDITOR MAIN WINDOW -------------------------

	void EditorLayer::DrawEditorMainWindow()
	{
		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

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

			ImGui::End();
		}
	}

	// ----------------------------- END EDITOR MAIN WINDOW ------------------------------

	void EditorLayer::OnEvent(Engine::IEvent& event)
	{
		EditorSceneManager::OnEvent(event);
		EditorSelection::OnEvent(event);

		m_sceneHierarchy.OnEvent(event);
		m_entityInspector.OnEvent(event);
		m_projectMenu.OnEvent(event);
		m_sceneView.OnEvent(event);
	}
}
