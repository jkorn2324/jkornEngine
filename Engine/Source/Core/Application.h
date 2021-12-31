#pragma once

#include <string>
#include <memory>
#include <chrono>

#include "LayerStack.h"

namespace Engine
{

	class Event;

	class Application
	{
	public:
		Application(const std::string& name);
		~Application();

		void Run();

		void AddLayer(class Layer* layer);
		void AddOverlay(class Layer* overlay);
		void RemoveLayer(class Layer* layer);
		void RemoveOverlay(class Layer* overlay);

	private:
		void OnEvent(Event& event);

		bool OnWindowClosed(class WindowClosedEvent& event);
		bool OnWindowResized(class WindowResizedEvent& event);

	private:
		std::unique_ptr<class Window> m_window;
		class GraphicsRenderer* m_graphicsRenderer;
		
		LayerStack m_windowLayerStack;
		std::chrono::high_resolution_clock::time_point m_prevTime;
		

		bool m_running;

#pragma region static_funcs

	private:
		static class Application* s_instance;

	public:
		static class Application& Get();

#pragma endregion
	};
}
