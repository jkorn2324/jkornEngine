#include "EnginePCH.h"
#include "ImGuiLayer.h"

#include "imgui.h"

#include "Application.h"
#include "Window.h"

#include "GraphicsRenderer.h"
#include "PlatformImGuiLayer.h"

namespace Engine
{

	ImGuiLayer::ImGuiLayer()
		: Layer("Editor Layer") 
	{
		m_graphicsImGuiLayer = PlatformImGuiLayer::Create(LayerType::TYPE_GRAPHICS);
		m_windowImGuiLayer = PlatformImGuiLayer::Create(LayerType::TYPE_WINDOW);
	}

	ImGuiLayer::~ImGuiLayer()
	{
		OnLayerRemoved();
	}

	void ImGuiLayer::OnLayerAdded()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		m_windowImGuiLayer->OnLayerAdded();
		m_graphicsImGuiLayer->OnLayerAdded();
	}

	void ImGuiLayer::OnLayerRemoved()
	{
		m_graphicsImGuiLayer->OnShutdown();
		m_windowImGuiLayer->OnShutdown();

		ImGui::DestroyContext();
	}

	void ImGuiLayer::BeginRender()
	{
		m_graphicsImGuiLayer->BeginFrame();
		m_windowImGuiLayer->BeginFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::EndRender()
	{
		Application& app = Application::Get();
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		m_graphicsImGuiLayer->EndFrame();
		m_windowImGuiLayer->EndFrame();
	}
}