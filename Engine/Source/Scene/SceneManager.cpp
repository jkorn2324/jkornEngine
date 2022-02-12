#include "EnginePCH.h"
#include "SceneManager.h"

#include "Scene.h"
#include "SceneSerializer.h"
#include "Entity.h"

#include "SceneEvent.h"
#include "EntityHierarchyComponent.h"

namespace Engine
{
	static Scene* s_activeScene = nullptr;
	static EventFunc s_eventFunc = nullptr;

	void SceneManager::Init()
	{
		s_activeScene = Scene::CreateDefaultScene();
		s_activeScene->BindEventFunc(s_eventFunc);
	}
	
	void SceneManager::Release()
	{
		delete s_activeScene;
	}
	
	void SceneManager::OnEvent(Event& event)
	{
		if (s_activeScene != nullptr)
		{
			s_activeScene->OnEvent(event);
		}
	}

	void SceneManager::LoadScene(const wchar_t* filePath)
	{
		if (s_activeScene != nullptr)
		{
			delete s_activeScene;
		}

		s_activeScene = new Scene();
		s_activeScene->BindEventFunc(s_eventFunc);

		SceneSerializer serializer(s_activeScene);
		serializer.Deserialize(filePath);
	}

	void SceneManager::SetActiveScene(Scene* scene)
	{
		if (s_activeScene != nullptr)
		{
			delete s_activeScene;
		}
		s_activeScene = scene;
		s_activeScene->BindEventFunc(s_eventFunc);
	}

	Scene& SceneManager::GetActiveScene()
	{
		return *s_activeScene;
	}

	void SceneManager::OnUpdate(const Timestep& ts)
	{
		if (s_activeScene != nullptr)
		{
			s_activeScene->OnUpdate(ts);
		}
	}
	
	void SceneManager::OnRuntimeUpdate(const Timestep& ts)
	{
		if (s_activeScene != nullptr)
		{
			s_activeScene->OnRuntimeUpdate(ts);
		}
	}
	
	void SceneManager::OnEditorUpdate(const Timestep& ts)
	{
		if (s_activeScene != nullptr)
		{
			s_activeScene->OnEditorUpdate(ts);
		}
	}

	void SceneManager::BindEventFunc(const EventFunc& func)
	{
		s_eventFunc = func;
		
		if (s_activeScene != nullptr)
		{
			s_activeScene->BindEventFunc(s_eventFunc);
		}
	}

	void SceneManager::Render()
	{
		if (s_activeScene != nullptr)
		{
			s_activeScene->Render();
		}
	}

	void SceneManager::Render(const CameraConstants& cameraConstants)
	{
		if (s_activeScene != nullptr)
		{
			s_activeScene->Render(cameraConstants);
		}
	}
}
