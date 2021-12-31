#include "EnginePCH.h"
#include "Scene.h"
#include "Entity.h"

#include "Components.h"
#include "Camera.h"

namespace Engine
{

	Scene::Scene()
		: m_entityRegistry(),
		m_markedForDestroyEntities(),
		m_camera(nullptr)
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Update(const Timestep& ts)
	{
		// Destroys the entities if they are marked for destroy.
		{
			std::int32_t sizeOfVec = m_markedForDestroyEntities.size() - 1;
			while (sizeOfVec >= 0)
			{
				m_entityRegistry.destroy(m_markedForDestroyEntities[sizeOfVec]);
				sizeOfVec--;
			}
		}

		// Selects the view cameras based on a set of camera components.
		{
			auto entityView = m_entityRegistry.view<SceneCameraComponent, Transform3DComponent>();
			for (auto entity : entityView)
			{
				auto [cameraComponent, transformComponent]
					= entityView.get<SceneCameraComponent, Transform3DComponent>(entity);
				SceneCamera& sceneCam = cameraComponent.camera;

				// The camera's view matrix is from world to camera,
				// but the transform matrix is from object to world.
				auto matrix = transformComponent.GetTransformMatrix();
				matrix.Invert();
				sceneCam.SetViewMatrix(matrix);

				if (cameraComponent.mainCamera)
				{
					m_camera = &cameraComponent.camera;
					break;
				}
			}
		}
	}

	Camera* Scene::GetCamera() const
	{
		return m_camera;
	}

	Entity Scene::CreateEntity()
	{
		Entity createdEntity = Entity(m_entityRegistry.create(), this);
		return createdEntity;
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		const auto& find = std::find(m_markedForDestroyEntities.begin(), 
			m_markedForDestroyEntities.end(), entity.m_entity);

		if (find == m_markedForDestroyEntities.end())
		{
			m_markedForDestroyEntities.push_back(entity.m_entity);
		}
	}
}
