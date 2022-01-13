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
		m_sceneHierarchy()
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
	}

	void EditorLayer::OnImGuiRender()
	{
		DrawDemo();

		m_sceneHierarchy.Draw();
	}
	
	void EditorLayer::OnEvent(Engine::Event& event)
	{
		EditorSceneManager::OnEvent(event);
		m_sceneHierarchy.OnEvent(event);
	}
}