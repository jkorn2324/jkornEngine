#include "EnginePCH.h"
#include "ImGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

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
		: Layer("Editor Layer"),
        m_graphicsLayer(), m_windowLayer()
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
		OnLayerRemoved();
	}

	void ImGuiLayer::OnEvent(IEvent& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Invoke<WindowEventType, WindowResizedEvent>(
			BIND_EVENT_FUNCTION(OnWindowResized));
	}

	bool ImGuiLayer::OnWindowResized(WindowResizedEvent& event)
	{
		// TODO: Fix aspect ratio with dpi.
		return true;
	}

	void ImGuiLayer::OnLayerAdded()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.ConfigWindowsMoveFromTitleBarOnly = true;

		ImGui::StyleColorsDark();

		m_windowLayer.OnLayerAdded();
        m_graphicsLayer.OnLayerAdded();
	}

	void ImGuiLayer::OnLayerRemoved()
	{
        m_graphicsLayer.OnLayerRemoved();
		m_windowLayer.OnLayerRemoved();

		ImGui::DestroyContext();
	}

	void ImGuiLayer::BeginRender()
	{
        m_graphicsLayer.BeginFrame();
        m_windowLayer.BeginFrame();
        
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::EndRender()
	{
		ImGuiIO& io = ImGui::GetIO();
		const auto& window = Application::Get().GetWindow();
		io.DisplaySize = ImVec2((float)window.GetWidth(), (float)window.GetHeight());

		ImGui::Render();
        
		m_graphicsLayer.EndFrame();
		m_windowLayer.EndFrame();
	}
}
