#include "EnginePCH.h"
#include "Scene.h"
#include "Entity.h"
#include "SceneManager.h"

#include "AssetManager.h"
#include "AssetCache.h"

#include "EntityEvents.h"
#include "Event.h"

#include "Components.h"
#include "Camera.h"

#include "Profiler.h"
#include "GraphicsRenderer.h"
#include "GraphicsRenderer2D.h"
#include "GraphicsRenderer3D.h"

#include <sstream>

namespace Engine
{
    namespace SceneUtility::Internals
    {
        entt::registry& GetEntityRegistry(Scene& scene)
        {
            return scene.m_entityRegistry;
        }
    }

	void Scene::CreateDefaultScene(Scene*& scene)
	{
		if (scene == nullptr)
		{
			scene = new Scene;
		}
		CreateDefaultScene(*scene);
	}

	void Scene::CreateDefaultScene(std::shared_ptr<Scene>& scene)
	{
		scene.reset();
		scene = std::make_shared<Scene>();
		CreateDefaultScene(*scene.get());
	}

	void Scene::CreateDefaultScene(std::unique_ptr<Scene>& scene)
	{
		scene.reset();
		scene = std::make_unique<Scene>();
		CreateDefaultScene(*scene.get());
	}

	void Scene::CreateDefaultScene(Scene& scene)
	{
		// Creates the scene camera.
		{
			EntityRef entity = scene.CreateEntity("Main Camera");
			Transform3DComponent& cameraTransform
				= entity.AddComponent<Transform3DComponent>();
			cameraTransform.SetLocalPosition(MathLib::Vector3{ 0.0f, 0.0f, -10.0f });
			entity.AddComponent<SceneCameraComponent>(true,
				SceneCameraType::TYPE_PERSPECTIVE);
		}

		// Creates the cube.
		{
			EntityRef entity = scene.CreateEntity("Cube");
			entity.AddComponent<Transform3DComponent>();
			MeshComponent& component =
				entity.AddComponent<MeshComponent>();
			AssetManager::GetMeshes().Get(L"DefaultCube", component.mesh);
			AssetManager::GetMaterials().Get(L"Unlit - ColorUV", component.material);
			component.enabled = true;
		}

		// Creates a directional light.
		{
			EntityRef entity = scene.CreateEntity("Directional Light");
			Transform3DComponent& transform
				= entity.AddComponent<Transform3DComponent>();
			transform.SetLocalEulerAngles(MathLib::Vector3{ 45, 0, 0 });

			DirectionalLightComponent& directionalLight
				= entity.AddComponent<DirectionalLightComponent>();
			directionalLight.enabled = true;
			directionalLight.lightColor = MathLib::Vector3::One;
			directionalLight.lightIntensity = 1.0f;
		}
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

	void Scene::OnEvent(IEvent& event)
	{
		EventDispatcher dispatcher(event);
#if false
		dispatcher.Invoke<EntityEventType, EntityComponentAddedEvent<Transform3DComponent>>(
			BIND_EVENT_FUNCTION(OnComponentAdded));
		dispatcher.Invoke<EntityEventType, EntityComponentRemovedEvent<Transform3DComponent>>(
			BIND_EVENT_FUNCTION(OnComponentRemoved));
#endif
		dispatcher.Invoke<EntityEventType, EntityHierarchyChangedEvent>(
			BIND_EVENT_FUNCTION(OnEntityHierarchyChanged));
	}

	const std::wstring& Scene::GetSceneName() const { return m_sceneName; }

	Scene* Scene::CopyScene()
	{
		Scene* cpyScene = new Scene(m_sceneName);
		for (auto& e : m_rootEntities)
		{
			if (e.IsValid(m_entityRegistry))
			{
				EntityRef createdEntity = cpyScene->CreateEntity();
				Entity pasted = (Entity)createdEntity;
				Entity::CopyEntity(e, m_entityRegistry, pasted, cpyScene->m_entityRegistry);
			}
		}
		return cpyScene;
	}

	void Scene::OnUpdate(const Timestep& ts)
	{
		PROFILE_SCOPE(OnUpdate, Scene);

		// Destroys the entities if they are marked for destroy.
		{
			int32_t sizeOfVec = (int32_t)m_markedForDestroyEntities.size() - 1;
			while (sizeOfVec >= 0)
			{
				m_entityRegistry.destroy(m_markedForDestroyEntities[sizeOfVec]);
				m_markedForDestroyEntities.pop_back();
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

				if (cameraComponent.enabled
					&& cameraComponent.mainCamera)
				{
					m_camera = &cameraComponent.camera;
					break;
				}
			}
		}

		// Applies the scene lights.
		{
			// Directional Light.
			{
				auto entityView = m_entityRegistry.view<DirectionalLightComponent>();
				int32_t size = (int32_t)entityView.size() - 1;
				for (int32_t back = size; back >= 0; back--)
				{
					auto eid = entityView[back];
					EntityRef e(entityView[back], m_entityRegistry);
					DirectionalLightComponent& directionalLight = entityView.get<0>(
						(entt::entity)e.GetEntity());
					if (e.HasComponent<Engine::Transform3DComponent>())
					{
						GraphicsRenderer3D::SetDirectionalLight(
							e.GetComponent<Engine::Transform3DComponent>().GetWorldForward(),
							directionalLight);
						break;
					}
				}
			}

			// Point Lights.
			{
				auto entityView = m_entityRegistry.view<PointLightComponent, Transform3DComponent>();
				for (auto e : entityView)
				{
					auto [pointLight, transform3D] = entityView.get<PointLightComponent, Transform3DComponent>(e);
					if (!GraphicsRenderer3D::AddPointLight(transform3D.GetWorldPosition(), pointLight))
					{
						break;
					}
				}
			}
		}
	}

	void Scene::OnRuntimeUpdate(const Timestep& ts)
	{
		PROFILE_SCOPE(RuntimeUpdate, Scene);
	}

	void Scene::OnEditorUpdate(const Timestep& ts)
	{
		PROFILE_SCOPE(EditorUpdate, Scene);
	}

	void Scene::Render(const CameraConstants& cameraConstants)
	{
		PROFILE_SCOPE(SceneRender, Rendering);

		GraphicsRenderer::BeginScene(cameraConstants);

		// Render the meshes.
		{
			m_entityRegistry.view<MeshComponent, Transform3DComponent>().each(
				[](auto entity, MeshComponent& mesh, Transform3DComponent& transform)
				{
					if (!mesh.enabled) return;

					if (mesh.mesh)
					{
						// Draws the mesh with a material.
						if (mesh.material)
						{
							GraphicsRenderer3D::DrawMesh(transform.GetTransformMatrix(),
								*mesh.mesh, *mesh.material, (int32_t)entity);
							return;
						}
						// Draws the mesh without a material.
						GraphicsRenderer3D::DrawMesh(transform.GetTransformMatrix(),
							*mesh.mesh, (int32_t)entity);
					}
				});
		}

		// Render the sprites.
		{
			auto entityView = m_entityRegistry.view<SpriteComponent>();
			for (auto entity : entityView)
			{
				auto sprite = entityView.get<SpriteComponent>(entity);
				if (!sprite.enabled) return;

				EntityRef e(entity, m_entityRegistry);
				if (e.HasComponent<Transform2DComponent>())
				{
					MathLib::Matrix4x4 transformMat =
						e.GetComponent<Transform2DComponent>().GetTransformMatrix();
					GraphicsRenderer2D::DrawRect(
						transformMat, sprite.color, sprite.texture, (int32_t)entity);
				}
				if (e.HasComponent<Transform3DComponent>())
				{
					MathLib::Matrix4x4 transformMat
						= e.GetComponent<Transform3DComponent>().GetTransformMatrix();
					GraphicsRenderer2D::DrawRect(
						transformMat, sprite.color, sprite.texture, (int32_t)entity);
				}
			}
		}

		GraphicsRenderer::EndScene();
	}

	void Scene::Render()
	{
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
		Render(constants);
	}

	Camera* Scene::GetCamera() const
	{
		return m_camera;
	}

	const std::vector<Entity>& Scene::GetRootEntities() const
	{
		return m_rootEntities;
	}

	EntityRef Scene::CreateEntity(const GUID& guid, const std::string& entityName, const Engine::Entity& parent)
	{
		PROFILE_SCOPE(CreateEntity, Scene);

		Entity createdEntity = Entity(m_entityRegistry.create());
		EntityRef createdEntityRef = EntityRef(createdEntity, m_entityRegistry);
		EntityRef parentEntityRef(parent, m_entityRegistry);

		createdEntityRef.AddComponent<NameComponent>(entityName);
		createdEntityRef.AddComponent<IDComponent>(guid);

		EntityHierarchyComponent& ehc
			= createdEntityRef.AddComponent<EntityHierarchyComponent>(createdEntity);
		if (parentEntityRef.IsValid())
		{
			ehc.SetParent(parentEntityRef);
		}

		// Appends the entity back to the root entities.
		if (!ehc.HasParent(m_entityRegistry))
		{
			m_rootEntities.push_back(createdEntity);
		}
		EntityCreatedEvent createdEvent(createdEntity);
		EventInvoker::Global().Invoke(createdEvent);
		return createdEntityRef;
	}

	EntityRef Scene::CreateEntity(const std::string& entityName, const Engine::Entity& parent)
	{
		PROFILE_SCOPE(CreateEntity, Scene);

		EntityRef parentEntityRef(parent, m_entityRegistry);
		Entity createdEntity = Entity(m_entityRegistry.create());
		EntityRef createdEntityRef(createdEntity, m_entityRegistry);

		createdEntityRef.AddComponent<NameComponent>(entityName);
		createdEntityRef.AddComponent<IDComponent>();

		EntityHierarchyComponent& ehc
			= createdEntityRef.AddComponent<EntityHierarchyComponent>(createdEntity);
		if (parentEntityRef.IsValid())
		{
			ehc.SetParent(parentEntityRef);
		}

		// Appends the entity back to the root entities.
		if (!ehc.HasParent(m_entityRegistry))
		{
			m_rootEntities.push_back(createdEntity);
		}

		EntityCreatedEvent createdEvent(createdEntity);
		EventInvoker::Global().Invoke(createdEvent);
		return createdEntityRef;
	}

	EntityRef Scene::CreateEntity(const GUID& guid, const Entity& parent)
	{
		return CreateEntity(guid, "Entity", parent);
	}

	EntityRef Scene::CreateEntity(const GUID& guid, const std::string& entity)
	{
		return CreateEntity(guid, entity, Entity{});
	}

	EntityRef Scene::CreateEntity(const GUID& guid)
	{
		return CreateEntity(guid, "Entity");
	}

	EntityRef Scene::CreateEntity(const std::string& entityName)
	{
		return CreateEntity(entityName.c_str());
	}

	EntityRef Scene::CreateEntity(const char* entityName)
	{
		return CreateEntity(entityName, Entity());
	}

	EntityRef Scene::CreateEntity()
	{
		return CreateEntity("Entity");
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		PROFILE_SCOPE(DestroyEntity, Scene);

		EntityRef entityRef(entity, m_entityRegistry);
		if (!entityRef.IsValid())
		{
			return;
		}

		// Destroys the entity's children.
		{
			EntityHierarchyComponent& ehc =
				entityRef.GetComponent<EntityHierarchyComponent>();
			if (ehc.HasChildren())
			{
				for (const auto& e : ehc.GetChildren())
				{
					DestroyEntity(e);
				}
			}
		}

		const auto& find = std::find(m_markedForDestroyEntities.begin(),
			m_markedForDestroyEntities.end(), (entt::entity)entity);
		if (find == m_markedForDestroyEntities.end())
		{
			m_markedForDestroyEntities.push_back((entt::entity)entity);
		}

		// Updates the root entities.
		{
			const auto& found = std::find(m_rootEntities.begin(),
				m_rootEntities.end(), entity);
			if (found != m_rootEntities.end())
			{
				m_rootEntities.erase(found);
			}
		}
		EntityDestroyedEvent destroyedEvent(entity);
		EventInvoker::Global().Invoke(destroyedEvent);
	}

	bool Scene::OnEntityHierarchyChanged(EntityHierarchyChangedEvent& event)
	{
		const auto& found = std::find(m_rootEntities.begin(),
			m_rootEntities.end(), event.entityHierarchy.GetOwner());
		if (found != m_rootEntities.end())
		{
			if (event.entityHierarchy.HasParent(m_entityRegistry))
			{
				m_rootEntities.erase(found);
			}
			return true;
		}
		if (!event.entityHierarchy.HasParent(m_entityRegistry))
		{
			m_rootEntities.push_back(event.entityHierarchy.GetOwner());
		}
		return false;
	}
}
