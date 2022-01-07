#include "EnginePCH.h"
#include "EditorLayer.h"

#include "imgui.h"

#include "Application.h"
#include "Window.h"

#include "GraphicsRenderer.h"
#include "PlatformEditorLayer.h"

namespace Engine
{

	EditorLayer::EditorLayer()
		: Layer("Editor Layer") 
	{
		m_graphicsEditorLayer = PlatformEditorLayer::Create(LayerType::TYPE_GRAPHICS);
		m_windowEditorLayer = PlatformEditorLayer::Create(LayerType::TYPE_WINDOW);
	}

	EditorLayer::~EditorLayer()
	{
		OnLayerRemoved();
	}

	void EditorLayer::OnLayerAdded()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui::StyleColorsDark();

		m_windowEditorLayer->OnLayerAdded();
		m_graphicsEditorLayer->OnLayerAdded();
	}

	void EditorLayer::OnLayerRemoved()
	{
		m_graphicsEditorLayer->OnShutdown();
		m_windowEditorLayer->OnShutdown();

		ImGui::DestroyContext();
	}

	void EditorLayer::BeginRender()
	{
		m_graphicsEditorLayer->BeginFrame();
		m_windowEditorLayer->BeginFrame();
		ImGui::NewFrame();
	}

	void EditorLayer::EndRender()
	{
		Application& app = Application::Get();
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		m_graphicsEditorLayer->EndFrame();
		m_windowEditorLayer->EndFrame();
	}
}