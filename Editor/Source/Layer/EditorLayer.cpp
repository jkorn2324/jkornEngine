#include "EditorPCH.h"
#include "EditorLayer.h"

#include "Engine.h"
#include <imgui.h>

#include "Source\Vector.h"
#include "EditorUtils.h"

namespace Editor
{


	static void DrawDemo()
	{
		ImGui::ShowDemoWindow();
	}


	EditorLayer::EditorLayer()
		: Layer("Editor"),
		m_frameBuffer(nullptr),
		m_sceneHierarchy(),
		m_entityInspector()
	{
	}

	EditorLayer::~EditorLayer()
	{
		delete m_frameBuffer;
	}

	void EditorLayer::OnLayerAdded()
	{
		Engine::GraphicsRenderer::GetRenderingAPI().SetClearColor(
			MathLib::Vector4(0.0f, 0.0f, 1.0f, 1.0f));

		Engine::FrameBufferSpecification specifications(
		{
			Engine::FrameBufferAttachmentType::DEPTH_STENCIL
		});
		specifications.width = Engine::Application::Get().GetWindow().GetWidth();
		specifications.height = Engine::Application::Get().GetWindow().GetHeight();
		m_frameBuffer = Engine::FrameBuffer::Create(specifications);
	}

	void EditorLayer::OnLayerRemoved()
	{
		// TODO: Implementation
	}

	void EditorLayer::OnUpdate(const Engine::Timestep& timestep)
	{
		m_frameBuffer->Bind();

		Engine::SceneManager::OnEditorUpdate(timestep);

		// TODO: Play button.
#if 1
		Engine::SceneManager::OnRuntimeUpdate(timestep);
		Engine::SceneManager::Render();
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
					// TODO: Implementation of Different Menu Items
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::End();
		}

		m_sceneHierarchy.Draw();
		m_entityInspector.Draw();
	}
	
	void EditorLayer::OnEvent(Engine::Event& event)
	{
		EditorSceneManager::OnEvent(event);
		EditorSelection::OnEvent(event);

		m_sceneHierarchy.OnEvent(event);
		m_entityInspector.OnEvent(event);

		Engine::EventDispatcher dispatcher(event);
		// TODO: Listen for key press events
	}
}