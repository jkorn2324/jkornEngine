#include "EditorPCH.h"
#include "EditorLayer.h"

#include "Engine.h"
#include <imgui.h>

#include "Source\Vector.h"
#include "EditorUtils.h"
#include "EditorCamera.h"

#include <string>

namespace Editor
{

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
		ImGui::ShowDemoWindow();
	}

	// TODO: Find a better way to get the main project menu path.

	EditorLayer::EditorLayer()
		: Layer("Editor"),
		m_sceneHierarchy(),
		m_entityInspector(),
		m_projectMenu(Engine::FileUtils::GetWorkingDirectory()),
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
	}

	void EditorLayer::OnLayerRemoved()
	{
		// TODO: Implementation
	}

	void EditorLayer::OnUpdate(const Engine::Timestep& timestep)
	{
		m_projectMenu.OnUpdate(timestep);

		EditorSceneManager::GetEditorCamera().OnEditorUpdate(timestep);
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

		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags sWindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoTitleBar;
		ImGuiWindowFlags windowFlags = sWindowFlags;

		
		// Draws the main window.
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			
			windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// Draws the editor dockspace.
		{
			ImGui::Begin("Editor", nullptr, windowFlags);

			// Apply the dockspace.
			{
				ImGuiID dockspace_id = ImGui::GetID("editor-dockspace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
			}

			DrawMenuBar();

			ImGui::End();
		}

		m_sceneHierarchy.Draw();
		m_entityInspector.Draw();
		m_projectMenu.Draw();
		m_sceneView.Draw();
		m_gameView.Draw();
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