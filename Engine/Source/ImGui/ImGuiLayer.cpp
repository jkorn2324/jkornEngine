#include "EnginePCH.h"
#include "ImGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Application.h"
#include "Window.h"

#include "GraphicsRenderer.h"
#include "RenderingAPI.h"
#include "PlatformImGuiLayer.h"
#include "FrameBuffer.h"

#include "Event.h"
#include "ApplicationEvent.h"

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

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Invoke<WindowResizedEvent>(
			BIND_EVENT_FUNCTION(OnWindowResized));
	}

	bool ImGuiLayer::OnWindowResized(WindowResizedEvent& event)
	{
		// TODO: Fix aspect ratio with dpi.
		return true;
	}

	void ImGuiLayer::OnLayerAdded()
	{
		const auto& window = Application::Get().GetWindow();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		
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
		ImGuiIO& io = ImGui::GetIO();
		const auto& window = Application::Get().GetWindow();
		io.DisplaySize = ImVec2((float)window.GetWidth(), (float)window.GetHeight());

		ImGui::Render();
		m_graphicsImGuiLayer->EndFrame();
		m_windowImGuiLayer->EndFrame();
	}
}