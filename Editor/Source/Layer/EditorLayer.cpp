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
		m_runtimeFrameBuffer(nullptr),
		m_editorFrameBuffer(nullptr),
		m_sceneHierarchy(),
		m_entityInspector(),
		m_projectMenu(Engine::GetWorkingDirectory())
	{
	}

	EditorLayer::~EditorLayer()
	{
		delete m_runtimeFrameBuffer;
		delete m_editorFrameBuffer;
	}

	void EditorLayer::OnLayerAdded()
	{
		EditorSceneManager::Init();

		Engine::GraphicsRenderer::GetRenderingAPI().SetClearColor(
			MathLib::Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		Engine::Application& app = Engine::Application::Get();

		// Runtime Specifications.
		{
			Engine::FrameBufferSpecification runtimeSpecifications(
				{
					// Engine::FrameBufferAttachmentType::DEPTH_STENCIL
				});

			runtimeSpecifications.width = app.GetWindow().GetWidth();
			runtimeSpecifications.height = app.GetWindow().GetHeight();
			m_runtimeFrameBuffer = Engine::FrameBuffer::Create(runtimeSpecifications);

		}

		// Editor Specifiations.
		{
			Engine::FrameBufferSpecification editorSpecifications(
			{
				Engine::FrameBufferAttachmentType::DEPTH_STENCIL
				// Engine::FrameBufferAttachmentType::RENDER_TARGET
			});
			editorSpecifications.width = app.GetWindow().GetWidth();
			editorSpecifications.height = app.GetWindow().GetHeight();
			m_editorFrameBuffer = Engine::FrameBuffer::Create(editorSpecifications);
		}
	}

	void EditorLayer::OnLayerRemoved()
	{
		// TODO: Implementation
	}

	void EditorLayer::OnUpdate(const Engine::Timestep& timestep)
	{
		m_projectMenu.OnUpdate(timestep);
		Engine::SceneManager::OnUpdate(timestep);

		if (!EditorSceneManager::IsPlaying())
		{
			// Updates camera and scene.
			EditorCamera& camera = EditorSceneManager::GetEditorCamera();
			camera.OnEditorUpdate(timestep);
			Engine::SceneManager::OnEditorUpdate(timestep);
			
			// Render to a view texture.
			{
				Engine::CameraConstants cameraConstants;
				cameraConstants.c_cameraPosition = camera.GetTransformMatrix().GetTranslation();
				cameraConstants.c_viewProjection = camera.GetViewProjectionMatrix();
				// TODO: Render the editor to a texture, then bind the texture to a viewport.
				
				m_editorFrameBuffer->Bind();
				Engine::SceneManager::Render(cameraConstants);
			}
		}
		else
		{
			Engine::SceneManager::OnRuntimeUpdate(timestep);
		}
		// TODO: Play button.
	
#if 0
		// Runtime frame buffer.
		{
			// TODO: Render the scene to a texture.
			m_runtimeFrameBuffer->Bind();
			Engine::SceneManager::Render();
		}
#endif
	}

	void EditorLayer::OnImGuiRender()
	{
		DrawDemo();

		// Draws the main window.
		{
			ImGuiWindowFlags mainWindowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
			ImGui::Begin("Editor", nullptr, mainWindowFlags);

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
				ImGui::EndMenuBar();
			}

			ImGui::End();
		}

		m_sceneHierarchy.Draw();
		m_entityInspector.Draw();
		m_projectMenu.Draw();
	}
	
	void EditorLayer::OnEvent(Engine::Event& event)
	{
		EditorSceneManager::OnEvent(event);
		EditorSelection::OnEvent(event);

		m_sceneHierarchy.OnEvent(event);
		m_entityInspector.OnEvent(event);
		m_projectMenu.OnEvent(event);
	}
}