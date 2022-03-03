#pragma once

#include <string>
#include <memory>
#include <chrono>
#include <filesystem>

#include "LayerStack.h"

int main(int argsc, char** argsv);

namespace Engine
{
	struct ApplicationArgs
	{
		int argsCount;
		char** args;
	};
	
	class Event;

	class Application
	{
	public:
		Application(const std::string& name, const std::filesystem::path& rootPath);
		virtual ~Application();

		void Run();

		void AddLayer(class Layer* layer);
		void AddOverlay(class Layer* overlay);
		void RemoveLayer(class Layer* layer);
		void RemoveOverlay(class Layer* overlay);

		const std::filesystem::path& GetRootPath() const { return m_rootPath; }
		class Window& GetWindow() const;

	private:
		void OnEvent(Event& event);

		bool OnWindowClosed(class WindowClosedEvent& event);
		bool OnWindowResized(class WindowResizedEvent& event);

	private:
		std::unique_ptr<class Window> m_window;
		class ImGuiLayer* m_imguiLayer;
		class GraphicsRenderer* m_graphicsRenderer;

		LayerStack m_windowLayerStack;
		std::filesystem::path m_rootPath;
		std::chrono::high_resolution_clock::time_point m_prevTime;

		bool m_running;

#pragma region static_funcs

	private:
		static class Application* s_instance;
		friend int ::main(int argsc, char** argsv);

	public:
		static class Application& Get();

#pragma endregion
	};

	Application* Create(const ApplicationArgs& args);
}

