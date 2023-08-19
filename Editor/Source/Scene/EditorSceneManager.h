#pragma once

#include <string>
#include <functional>

#include "SceneSerializer.h"

namespace Engine
{
	class IEvent;
}

namespace Editor
{
	class EditorCamera;

	using EventFunc = std::function<void(Engine::IEvent&)>;

	class EditorSceneManager
	{
	public:
		static void Init();
		static void Release();

		static void OnEvent(Engine::IEvent& event);

		static void OpenScene(const std::filesystem::path& path);
		static void SaveScene(const std::filesystem::path& path);

		static bool IsPlaying();
		static void SetPlaying(bool playing);

		static bool IsPaused();
		static void SetPaused(bool paused);

		static class EditorCamera& GetEditorCamera();
		static Engine::SceneSerializer GetDefaultSceneSerializer();
	};
}