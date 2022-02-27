#include "EditorPCH.h"
#include "EditorSceneManager.h"

#include "EditorCamera.h"

namespace Editor
{
	struct ActiveSceneData
	{
		Engine::Scene* editorSceneWhilePlaying = nullptr;
	};

	static EditorCamera s_editorCamera;
	static bool s_paused = false, s_playing = false;

	ActiveSceneData s_activeSceneData;

	static bool OnWindowSizeChanged(Engine::WindowResizedEvent& event)
	{
		return true;
	}

	void EditorSceneManager::Init() { }

	void EditorSceneManager::Release()
	{
		if (s_activeSceneData.editorSceneWhilePlaying)
		{
			delete s_activeSceneData.editorSceneWhilePlaying;
		}
	}

	void EditorSceneManager::OnEvent(Engine::Event& event)
	{
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Invoke<Engine::WindowResizedEvent>(
			BIND_STATIC_EVENT_FUNCTION(OnWindowSizeChanged));
	}

	void EditorSceneManager::OpenScene(const std::filesystem::path& path)
	{
		if (!IsPlaying())
		{
			Engine::SceneManager::LoadScene(path);
		}
	}

	void EditorSceneManager::SaveScene(const std::filesystem::path& path)
	{
		if (!s_playing)
		{
			Engine::SceneSerializer sceneSerializer(
				&Engine::SceneManager::GetActiveScene());
			sceneSerializer.Serialize(path);
		}
	}

	bool EditorSceneManager::IsPlaying()
	{
		return s_playing;
	}

	void EditorSceneManager::SetPlaying(bool playing)
	{
		if (s_playing != playing)
		{
			if (playing)
			{
				s_activeSceneData.editorSceneWhilePlaying
					= Engine::SceneManager::GetActiveScene().CopyScene();
			}
			else
			{
				if (s_activeSceneData.editorSceneWhilePlaying != nullptr)
				{
					Engine::SceneManager::SetActiveScene(
						s_activeSceneData.editorSceneWhilePlaying);
					s_activeSceneData.editorSceneWhilePlaying = nullptr;
				}
			}
		}
		s_playing = playing;
	}

	bool EditorSceneManager::IsPaused()
	{
		return s_paused;
	}

	void EditorSceneManager::SetPaused(bool paused)
	{
		s_paused = paused;
	}

	EditorCamera& EditorSceneManager::GetEditorCamera()
	{
		return s_editorCamera;
	}
}