#include "EnginePCH.h"
#include "Scene.h"
#include "Entity.h"

#include "Components.h"
#include "Camera.h"

#include "Profiler.h"
#include "GraphicsRenderer.h"
#include "GraphicsRenderer2D.h"

namespace Engine
{

	Scene* Scene::CreateDefaultScene()
	{
		Scene* scene = new Scene();
		// TODO: Add Default Scene

		return scene;
	}

	Scene::Scene()
		: m_entityRegistry(),
		m_markedForDestroyEntities(),
		m_camera(nullptr),
		m_sceneName(L"DefaultScene")
	{

	}

	Scene::Scene(const std::wstring& name)
		: m_entityRegistry(),
		m_markedForDestroyEntities(),
		m_camera(nullptr),
		m_sceneName(name)
	{
	}

	Scene::~Scene()
	{
	}

	const std::wstring& Scene::GetSceneName() const { return m_sceneName; }

	void Scene::OnRuntimeUpdate(const Timestep& ts)
	{
		PROFILE_SCOPE(RuntimeUpdate, Update);

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
				sceneCam.UpdateProjectionMatrix();

				if (cameraComponent.mainCamera)
				{
					m_camera = &cameraComponent.camera;
					break;
				}
			}
		}
	}

	void Scene::OnEditorUpdate(const Timestep& ts)
	{
		PROFILE_SCOPE(EditorUpdate, Update);

		// TODO: Implement editor update.
	}

	void Scene::Render()
	{
		PROFILE_SCOPE(SceneRender, Rendering);

		CameraConstants constants;
		{
			if (m_camera != nullptr)
			{
				auto mat = m_camera->GetViewMatrix();
				mat.Invert();

				constants.c_cameraPosition = mat.GetTranslation();
				constants.c_viewProjection =
					m_camera->GetViewProjectionMatrix();
			}
		}
		
		GraphicsRenderer::BeginScene(constants);
		
		// Render the sprites.
		{
			m_entityRegistry.view<SpriteComponent, Transform3DComponent>().each(
			[](auto entity, SpriteComponent& sprite, Transform3DComponent& transform)
			{
				if (!sprite.enabled) return;

				GraphicsRenderer2D::DrawRect(transform.GetTransformMatrix(),
					sprite.color, sprite.texture);
			});
		}
		
		GraphicsRenderer::EndScene();
	}

	Entity Scene::Find(const std::string& entityName) const
	{
		const auto entityView = m_entityRegistry.view<const NameComponent>();
		for (auto entity : entityView)
		{
			auto nameComponent = entityView.get(entity);
			if (nameComponent.name == entityName)
			{
				return Entity(entity, this);
			}
		}
		return Entity(entt::null, this);
	}

	Camera* Scene::GetCamera() const
	{
		return m_camera;
	}

	Entity Scene::CreateEntity(const char* entityName)
	{
		Entity createdEntity = Entity(m_entityRegistry.create(), this);
		createdEntity.AddComponent<NameComponent>(entityName);
		return createdEntity;
	}

	Entity Scene::CreateEntity()
	{
		return CreateEntity("Entity");
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
