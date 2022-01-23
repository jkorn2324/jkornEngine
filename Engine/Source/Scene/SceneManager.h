#pragma once

#include <functional>

namespace Engine
{
	class Scene;
	class Timestep;
	class Entity;

	using EventFunc = std::function<void(class Event&)>;

	class SceneManager
	{
	public:
		static void Init();
		static void Release();

		static void OnEvent(Event& event);
		static void LoadScene(const wchar_t* filePath);

		static Scene& GetActiveScene();

		static void OnUpdate(const Timestep& ts);
		static void OnRuntimeUpdate(const Timestep& ts);
		static void OnEditorUpdate(const Timestep& ts);

		static void Render();
		static void Render(const struct CameraConstants& cameraConstants);

	private:
		static void BindEventFunc(const EventFunc& func);

		friend class Application;
	};
}