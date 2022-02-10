#include "EnginePCH.h"
#include "Scene.h"
#include "Entity.h"
#include "SceneManager.h"

#include "AssetManager.h"
#include "AssetCache.h"

#include "SceneEvent.h"
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

	static void UpdateEntityHierarchies(Entity& rootEntity, EntityHierarchyComponent& rootComponent)
	{
		if (!rootComponent.HasChildren())
		{
			return;
		}

		MathLib::Matrix4x4 mat = MathLib::Matrix4x4::Identity;
		{
			if (rootEntity.HasComponent<Transform3DComponent>())
			{
				mat = rootEntity.GetComponent<Transform3DComponent>().GetTransformMatrix();
			}
			if (rootEntity.HasComponent<Transform2DComponent>())
			{
				mat = rootEntity.GetComponent<Transform2DComponent>().GetTransformMatrix();
			}
		}

		int32_t size = (int32_t)rootComponent.GetChildren().size() - 1;
		for (int32_t i = size; i >= 0; i--)
		{
			auto e = rootComponent.GetChildren()[i];
			if (e.HasComponent<Transform3DComponent>())
			{
				Transform3DComponent& component
					= e.GetComponent<Transform3DComponent>();
				component.SetParentTransformMatrix(mat);
			}

			if (e.HasComponent<Transform2DComponent>())
			{
				Transform2DComponent& component
					= e.GetComponent<Transform2DComponent>();
				component.SetParentTransformMatrix(mat);
			}

			if (e.HasComponent<EntityHierarchyComponent>())
			{
				EntityHierarchyComponent& hierarchyComponent
					= e.GetComponent<EntityHierarchyComponent>();
				UpdateEntityHierarchies(e, hierarchyComponent);
			}
		}
	}

	Scene* Scene::CreateDefaultScene()
	{
		Scene* scene = new Scene();

		// Creates the scene camera.
		{
			Entity entity = scene->CreateEntity("Main Camera");
			Transform3DComponent& cameraTransform
				= entity.AddComponent<Transform3DComponent>();
			cameraTransform.SetLocalPosition(MathLib::Vector3{ 0.0f, 0.0f, -10.0f });
			entity.AddComponent<SceneCameraComponent>(true,
				SceneCameraType::TYPE_PERSPECTIVE);
		}

		// Creates the cube.
		{
			Entity entity = scene->CreateEntity("Cube");
			entity.AddComponent<Transform3DComponent>();
			MeshComponent& component =
				entity.AddComponent<MeshComponent>();
			component.mesh = AssetManager::GetMeshes().Get(L"DefaultCube");
			component.material = AssetManager::GetMaterials().Get(L"Unlit - ColorUV");
			component.enabled = true;
		}

		// Creates a directional light.
		{
			Entity entity = scene->CreateEntity("Directional Light");
			Transform3DComponent& transform
				= entity.AddComponent<Transform3DComponent>();
			transform.SetLocalEulerAngles(MathLib::Vector3 { 45, 0, 0 });

			DirectionalLightComponent& directionalLight
				= entity.AddComponent<DirectionalLightComponent>();
			directionalLight.enabled = true;
			directionalLight.lightColor = MathLib::Vector3::One;
			directionalLight.lightIntensity = 1.0f;
		}
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

	void Scene::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Invoke<EntityComponentAddedEvent<Transform3DComponent>>(
			BIND_EVENT_FUNCTION(OnComponentAdded));
		dispatcher.Invoke<EntityComponentRemovedEvent<Transform3DComponent>>(
			BIND_EVENT_FUNCTION(OnComponentRemoved));
		dispatcher.Invoke<EntityHierarchyChangedEvent>(
			BIND_EVENT_FUNCTION(OnEntityHierarchyChanged));
	}

	const std::wstring& Scene::GetSceneName() const { return m_sceneName; }

	void Scene::BindEventFunc(const EventFunc& func)
	{
		m_eventFunc = func;
	}

	void Scene::OnUpdate(const Timestep& ts)
	{
		PROFILE_SCOPE(OnUpdate, Scene);

		// Destroys the entities if they are marked for destroy.
		{
			int32_t sizeOfVec = (int32_t)m_markedForDestroyEntities.size() - 1;
			while (sizeOfVec >= 0)
			{
				{
					Entity e = Entity{ m_markedForDestroyEntities[sizeOfVec], this };
					e.GetComponent<BehaviorComponent>().Destroy();
				}
				m_entityRegistry.destroy(m_markedForDestroyEntities[sizeOfVec]);
				m_markedForDestroyEntities.pop_back();
				sizeOfVec--;
			}
		}

		// Updates each behavior component generally.
		{
			auto entityView = m_entityRegistry.view<BehaviorComponent>();
			for (auto entity : entityView)
			{
				auto behaviorComponent = entityView.get<BehaviorComponent>(entity);
				behaviorComponent.Get().OnUpdate(ts);
			}
		}

		// Updates the entity hierarchies.
		{
			const auto& rootEntities
				= GetRootEntities();
			for (auto entity : rootEntities)
			{
				EntityHierarchyComponent& hierarchy
					= entity.GetComponent<EntityHierarchyComponent>();
				UpdateEntityHierarchies(entity, hierarchy);
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
					Entity e = Entity{ entityView[back], this };
					if (e.HasComponent<Engine::Transform3DComponent>())
					{
						GraphicsRenderer3D::SetDirectionalLight(
							e.GetComponent<Engine::Transform3DComponent>().GetWorldForward(), 
							entityView.raw()[back]);
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

		// Updates each behavior component at runtime.
		{
			auto entityView = m_entityRegistry.view<BehaviorComponent>();
			for (auto entity : entityView)
			{
				auto behaviorComponent = entityView.get<BehaviorComponent>(entity);
				behaviorComponent.Get().OnRuntimeUpdate(ts);
			}
		}
	}

	void Scene::OnEditorUpdate(const Timestep& ts)
	{
		PROFILE_SCOPE(EditorUpdate, Scene);

		// Updates each behavior component.
		{
			auto entityView = m_entityRegistry.view<BehaviorComponent>();
			for (auto entity : entityView)
			{
				auto behaviorComponent = entityView.get<BehaviorComponent>(entity);
				behaviorComponent.Get().OnEditorUpdate(ts);
			}
		}
	}

	void Scene::Render(const CameraConstants& cameraConstants)
	{
		PROFILE_SCOPE(SceneRender, Rendering);

		GraphicsRenderer::BeginScene(cameraConstants);

		// Apply the scene lights.
		{


			// Add the point lights.
			{
				// TODO: Implementation
			}
		}

		// Render the meshes.
		{
			m_entityRegistry.view<MeshComponent, Transform3DComponent>().each(
				[](auto entity, MeshComponent& mesh, Transform3DComponent& transform)
				{
					if (!mesh.enabled) return;

					if (mesh.mesh != nullptr)
					{
						// Draws the mesh with a material.
						if (mesh.material != nullptr)
						{
							GraphicsRenderer3D::DrawMesh(transform.GetTransformMatrix(),
								*mesh.mesh, *mesh.material);
							return;
						}
						// Draws the mesh without a material.
						GraphicsRenderer3D::DrawMesh(transform.GetTransformMatrix(),
							*mesh.mesh);
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

				Entity e = Entity{ entity, this };
				if (e.HasComponent<Transform2DComponent>())
				{
					MathLib::Matrix4x4 transformMat = 
						e.GetComponent<Transform2DComponent>().GetTransformMatrix();
					GraphicsRenderer2D::DrawRect(
						transformMat, sprite.color, sprite.texture);
				}
				if (e.HasComponent<Transform3DComponent>())
				{
					MathLib::Matrix4x4 transformMat
						= e.GetComponent<Transform3DComponent>().GetTransformMatrix();
					GraphicsRenderer2D::DrawRect(
						transformMat, sprite.color, sprite.texture);
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

	Entity Scene::Find(const std::string& entityName) const
	{
		const auto entityView = m_entityRegistry.view<const NameComponent>();
		for (auto entity : entityView)
		{
			auto nameComponent = entityView.get(entity);
			if (nameComponent.name == entityName)
			{
				return Entity(entity, (Scene*)this);
			}
		}
		return Entity::None;
	}

	Camera* Scene::GetCamera() const
	{
		return m_camera;
	}

	const std::vector<Entity>& Scene::GetRootEntities() const
	{
		return m_rootEntities;
	}

	Entity Scene::CreateEntity(const char* entityName)
	{
		PROFILE_SCOPE(CreateEntity, Scene);

		Entity createdEntity = Entity(m_entityRegistry.create(), this);
		createdEntity.AddComponent<NameComponent>(entityName);
	
		{
			BehaviorComponent& entity
				= createdEntity.AddComponent<BehaviorComponent>();
			entity.Create(createdEntity);
		}

		EntityHierarchyComponent& ehc
			= createdEntity.AddComponent<EntityHierarchyComponent>(createdEntity);

		// Appends the entity back to the root entities.
		if (!ehc.HasParent())
		{
			m_rootEntities.push_back(createdEntity);
		}

		if (m_eventFunc != nullptr)
		{
			EntityCreatedEvent createdEvent(createdEntity);
			m_eventFunc(createdEvent);
		}
		return createdEntity;
	}

	Entity Scene::CreateEntity()
	{
		return CreateEntity("Entity");
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		PROFILE_SCOPE(DestroyEntity, Scene);

		if (!entity.IsValid())
		{
			return;
		}
		
		// Destroys the entity's children.
		{
			EntityHierarchyComponent& ehc =
				entity.GetComponent<EntityHierarchyComponent>();
			if (ehc.HasChildren())
			{
				for (const auto& e : ehc.GetChildren())
				{
					DestroyEntity(e);
				}
			}
		}

		// Triggers on destroy for the children.
		{
			BehaviorComponent& bc =
				entity.GetComponent<BehaviorComponent>();
			bc.Get().OnDestroy();
		}

		const auto& find = std::find(m_markedForDestroyEntities.begin(), 
			m_markedForDestroyEntities.end(), entity.m_entity);
		if (find == m_markedForDestroyEntities.end())
		{
			m_markedForDestroyEntities.push_back(entity.m_entity);
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

		if (m_eventFunc != nullptr)
		{
			EntityDestroyedEvent destroyedEvent(entity);
			m_eventFunc(destroyedEvent);
		}
	}

	bool Scene::OnEntityHierarchyChanged(EntityHierarchyChangedEvent& event)
	{
		const auto& found = std::find(m_rootEntities.begin(),
			m_rootEntities.end(), event.entityHierarchy.GetOwner());
		if (found != m_rootEntities.end())
		{
			if (event.entityHierarchy.HasParent())
			{
				m_rootEntities.erase(found);
			}
			return true;
		}
		if (!event.entityHierarchy.HasParent())
		{
			m_rootEntities.push_back(event.entityHierarchy.GetOwner());
		}
		return false;
	}

	template<typename T>
	bool Scene::OnComponentAdded(EntityComponentAddedEvent<T>& event)
	{
		static_assert(false);
		return true;
	}

	template<>
	bool Scene::OnComponentAdded(EntityComponentAddedEvent<Transform3DComponent>& event)
	{
		// TODO: Implementation
		return true;
	}

	template<typename T>
	bool Scene::OnComponentRemoved(EntityComponentRemovedEvent<T>& event)
	{
		static_assert(false);
		return true;
	}
	
	template<>
	bool Scene::OnComponentRemoved(EntityComponentRemovedEvent<BehaviorComponent>& event)
	{
		event.component.Get().OnDestroy();
		event.component.Destroy();
		return true;
	}

	template<>
	bool Scene::OnComponentRemoved(EntityComponentRemovedEvent<Transform3DComponent>& event)
	{
		// TODO: Implementation
		return true;
	}
}
