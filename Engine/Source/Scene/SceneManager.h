#pragma once

#include <functional>
#include <filesystem>

namespace Engine
{
	class Scene;
	class Timestep;
	class Entity;
	class IEvent;

	using EventFunc = std::function<void(IEvent&)>;

	class SceneManager
	{
	public:
		static void Init();
		static void Release();

		static void OnEvent(IEvent& event);
		static void LoadScene(const std::filesystem::path& path);
		static void LoadScene(const wchar_t* filePath);

		static void SetActiveScene(Scene* scene);
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