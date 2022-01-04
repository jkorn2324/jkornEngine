#include "EnginePCH.h"
#include "Application.h"

#include "GraphicsRenderer.h"
#include "GraphicsRenderer2D.h"

#include "RenderingAPI.h"

#include "Window.h"
#include "Layer.h"

#include "Event.h"
#include "ApplicationEvent.h"
#include "Timestep.h"

namespace Engine
{
	using time_point = std::chrono::high_resolution_clock::time_point;

	const std::uint32_t WINDOW_WIDTH = 1600;
	const std::uint32_t WINDOW_HEIGHT = 900;

	Application* Application::s_instance = nullptr;

	Application& Application::Get()
	{
		return *s_instance;
	}

	Application::Application(const std::string& name)
		: m_window(nullptr), 
		m_running(true), 
		m_windowLayerStack(),
		m_prevTime(std::chrono::high_resolution_clock::now()),
		m_graphicsRenderer(nullptr)
	{
		DebugAssert(s_instance == nullptr, "Application is already running.");
		s_instance = this;

		WindowProperties properties = 
		{
			name, WINDOW_WIDTH, WINDOW_HEIGHT, true
		};
		m_window = std::make_unique<Window>(properties);
		m_window->SetCallback(BIND_EVENT_FUNCTION(Application::OnEvent));
		
		m_graphicsRenderer = new GraphicsRenderer();
		m_graphicsRenderer->Initialize(m_window.get());
		GraphicsRenderer2D::Init();
	}

	Application::~Application()
	{
		m_windowLayerStack.Clear();

		GraphicsRenderer2D::Release();
		delete m_graphicsRenderer;
	}

	void Application::Run()
	{
		while (m_running)
		{
			// Gets the time step.
			time_point currentTime = std::chrono::high_resolution_clock::now();
			std::chrono::nanoseconds diff =
				std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - m_prevTime);
			m_prevTime = currentTime;
			Timestep ts = (float)diff.count() * 0.000000001f;
			
			// Update the window layer stack.
			{
				for (Layer* layer : m_windowLayerStack)
				{
					layer->OnUpdate(ts);
				}
			}
			m_window->OnUpdate();
		}
	}

	void Application::AddLayer(Layer* layer)
	{
		m_windowLayerStack.AddLayer(layer);
	}

	void Application::AddOverlay(Layer* overlay)
	{
		m_windowLayerStack.AddOverlay(overlay);
	}

	void Application::RemoveLayer(Layer* layer)
	{
		m_windowLayerStack.RemoveLayer(layer);
	}

	void Application::RemoveOverlay(Layer* overlay)
	{
		m_windowLayerStack.RemoveOverlay(overlay);
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Invoke<WindowClosedEvent>(BIND_EVENT_FUNCTION(Application::OnWindowClosed));
		dispatcher.Invoke<WindowResizedEvent>(BIND_EVENT_FUNCTION(Application::OnWindowResized));

		for(auto iterator = m_windowLayerStack.rbegin(); iterator != m_windowLayerStack.rend(); iterator++)
		{
			(*iterator)->OnEvent(event);
		}
	}

	bool Application::OnWindowClosed(WindowClosedEvent& event)
	{
		m_running = false;
		return true;
	}

	bool Application::OnWindowResized(WindowResizedEvent& event)
	{
		m_graphicsRenderer->GetRenderingAPI()
			->SetResolution(event.width, event.height);
		return true;
	}
}
