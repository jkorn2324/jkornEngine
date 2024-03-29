#include "EnginePCH.h"
#include "SceneManager.h"

#include "Scene.h"
#include "SceneSerializer.h"
#include "Entity.h"

#include "EntityEvents.h"
#include "EntityHierarchyComponent.h"
#include "Profiler.h"

#include "ConstantBuffer.h"

namespace Engine
{
	static Scene* s_activeScene = nullptr;
	static ConstantBuffer* c_cameraBuffer = nullptr;

	void SceneManager::Init()
	{
		PROFILE_SCOPE(Init, SceneManager);

		Scene::CreateDefaultScene(s_activeScene);
	}

	void SceneManager::Release()
	{
		delete s_activeScene;
		delete c_cameraBuffer;

		c_cameraBuffer = nullptr;
	}

	void SceneManager::OnEvent(IEvent& event)
	{
		if (s_activeScene != nullptr)
		{
			s_activeScene->OnEvent(event);
		}
	}

	void SceneManager::LoadScene(const std::filesystem::path& path)
	{
		if (s_activeScene != nullptr)
		{
			delete s_activeScene;
			s_activeScene = nullptr;
		}

		s_activeScene = new Scene();
		SceneSerializer serializer(s_activeScene);
		serializer.Deserialize(path);
	}

	void SceneManager::LoadScene(const wchar_t* filePath)
	{
		if (s_activeScene != nullptr)
		{
			delete s_activeScene;
		}

		s_activeScene = new Scene();
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

	void SceneManager::Render()
	{
		if (s_activeScene != nullptr)
		{
			ConstantBuffer** refCpy = &c_cameraBuffer;
			s_activeScene->Render(refCpy);
			c_cameraBuffer = *refCpy;
		}
	}

	void SceneManager::Render(const CameraConstants& cameraConstants)
	{
		if (s_activeScene != nullptr)
		{
			ConstantBuffer** refCpy = &c_cameraBuffer;
			s_activeScene->Render(cameraConstants, refCpy);
			c_cameraBuffer = *refCpy;
		}
	}
}
