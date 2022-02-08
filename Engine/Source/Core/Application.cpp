#include "EnginePCH.h"
#include "Application.h"

#include "GraphicsRenderer.h"
#include "GraphicsRenderer2D.h"
#include "GraphicsRenderer3D.h"

#include "RenderingAPI.h"

#include "Window.h"
#include "Layer.h"
#include "Event.h"
#include "ApplicationEvent.h"
#include "EngineTime.h"
#include "ImGuiLayer.h"
#include "Input.h"
#include "Profiler.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "Entity.h"
#include "EntityHierarchyComponent.h"

namespace Engine
{
	using time_point = std::chrono::high_resolution_clock::time_point;

	const std::uint32_t WINDOW_WIDTH = 1600;
	const std::uint32_t WINDOW_HEIGHT = 900;

	const uint32_t MIN_WINDOW_WIDTH = 1200;
	const uint32_t MIN_WINDOW_HEIGHT = 675;

	Application* Application::s_instance = nullptr;

	Application& Application::Get()
	{
		return *s_instance;
	}

	Application::Application(const std::string& name, const std::string& rootPath)
		: m_window(nullptr), 
		m_running(true), 
		m_windowLayerStack(),
		m_prevTime(std::chrono::high_resolution_clock::now()),
		m_graphicsRenderer(nullptr),
		m_imguiLayer(nullptr),
		m_rootPath(rootPath)
	{
		DebugAssert(s_instance == nullptr, "Application is already running.");
		s_instance = this;

		Logger::Init();
		Profiler::Init();

		WindowProperties properties = 
		{
			name, WINDOW_WIDTH, WINDOW_HEIGHT, 
			MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, true
		};
		m_window = Window::GenerateWindow(properties);
		m_window->SetCallback(BIND_EVENT_FUNCTION(Application::OnEvent));
		
		Input::BindInputEventFunc(BIND_EVENT_FUNCTION(Application::OnEvent));
		Input::Init();
		
		GraphicsRenderer::Init();
		GraphicsRenderer2D::Init();
		GraphicsRenderer3D::Init();

		SceneManager::BindEventFunc(BIND_EVENT_FUNCTION(Application::OnEvent));
		SceneManager::Init();

		// Binds the component add and remove to the event func.
		Entity::BindEventFunc(BIND_EVENT_FUNCTION(Application::OnEvent));
		EntityHierarchyComponent::BindEventFunc(BIND_EVENT_FUNCTION(Application::OnEvent));

		m_imguiLayer = new ImGuiLayer();
		m_windowLayerStack.AddOverlay(m_imguiLayer);
	}

	Application::~Application()
	{
		m_windowLayerStack.Clear();

		SceneManager::Release();
		AssetManager::UncacheAssets();
		GraphicsRenderer3D::Release();
		GraphicsRenderer2D::Release();
		GraphicsRenderer::Release();
		Input::Release();

		Profiler::Release();
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
			float timestep = (float)diff.count() * 0.000000001f;
			Timestep ts = { timestep, Time::GetTimeScale() };

			// Update the window layer stack.
			{
				for (Layer* layer : m_windowLayerStack)
				{
					layer->OnUpdate(ts);
				}
			}

			{
				m_imguiLayer->BeginRender();
				{
					for (Layer* layer : m_windowLayerStack)
					{
						layer->OnImGuiRender();
					}
				}
				m_imguiLayer->EndRender();
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

	Window& Application::GetWindow() const
	{
		return *m_window.get();
	}

	void Application::OnEvent(Event& event)
	{
		Input::OnEvent(event);
		SceneManager::OnEvent(event);

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
		GraphicsRenderer::OnWindowResized(event.width, event.height);
		return true;
	}
}
