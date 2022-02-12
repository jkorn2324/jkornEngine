#include "EnginePCH.h"
#include "Entity.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Components.h"

#include "AssetManager.h"
#include "AssetCache.h"
#include "Texture.h"
#include "Mesh.h"

namespace Engine
{

    template<typename T>
    static void CopyComponent(const Entity& from, Entity& to, const std::function<void(T&, T&)>& func, bool addIfNotExist = true)
    {
        if (from.HasComponent<T>())
        {
            T& fromComponent = from.GetComponent<T>();
            if (to.HasComponent<T>())
            {
                T& toComp = to.GetComponent<T>();
                func(fromComponent, toComp);
            }
            else
            {
                T& toComp = to.AddComponent<T>();
                func(fromComponent, toComp);
            }
        }
    }

    const Entity Entity::None = Entity(entt::null, nullptr);

    EventFunc Entity::s_componentEventFunc = nullptr;

    Entity::Entity(const entt::entity& entity, Scene* scene)
        : m_scene(scene),
        m_entity(entity)
    {
    }

    bool Entity::IsValid() const
    {
        return m_scene != nullptr
            && m_scene->m_entityRegistry.valid(m_entity);
    }
    
    void Entity::BindEventFunc(const EventFunc& func)
    {
        s_componentEventFunc = func;
    }

    void CopyEntity(const Entity& fromEntity, Entity& toEntity, bool copyName)
    {
        if (copyName)
        {
            CopyComponent<NameComponent>(fromEntity, toEntity,
                [=](NameComponent& from, NameComponent& to) -> void
                {
                    to.name = from.name;
                });
        }

        CopyComponent<Transform3DComponent>(fromEntity, toEntity,
            [=](Transform3DComponent& from, Transform3DComponent& to) -> void
            {
                to.SetLocalPosition(from.GetLocalPosition());
                to.SetLocalRotation(from.GetLocalRotation());
                to.SetLocalScale(from.GetLocalScale());
                to.SetParentTransformMatrix(from.GetParentTransformMatrix());
            });

        CopyComponent<Transform2DComponent>(fromEntity, toEntity,
            [=](Transform2DComponent& from, Transform2DComponent& to) -> void
            {
                to.SetLocalPosition(from.GetLocalPosition());
                to.SetLocalRotation(from.GetLocalRotation());
                to.SetLocalScale(from.GetLocalScale());
                to.SetParentTransformMatrix(from.GetParentTransformMatrix());
            });

        CopyComponent<SpriteComponent>(fromEntity, toEntity,
            [=](SpriteComponent& from, SpriteComponent& to) -> void
            {
                to.color = from.color;
                to.enabled = from.enabled;
                to.texture = from.texture;
            });

        CopyComponent<MeshComponent>(fromEntity, toEntity,
            [=](MeshComponent& from, MeshComponent& to) -> void
            {
                to.material = from.material;
                to.enabled = from.enabled;
                to.mesh = from.mesh;
            });

        CopyComponent<DirectionalLightComponent>(fromEntity, toEntity,
            [=](DirectionalLightComponent& from, DirectionalLightComponent& to) -> void
            {
                to.enabled = from.enabled;
                to.lightColor = from.lightColor;
                to.lightIntensity = from.lightIntensity;
            });

        CopyComponent<PointLightComponent>(fromEntity, toEntity,
            [=](PointLightComponent& from, PointLightComponent& to) -> void
            {
                to.enabled = from.enabled;
                to.innerRadius = from.innerRadius;
                to.outerRadius = from.outerRadius;
                to.lightColor = from.lightColor;
                to.lightIntensity = from.lightIntensity;
            });

        CopyComponent<SceneCameraComponent>(fromEntity, toEntity,
            [=](SceneCameraComponent& from, SceneCameraComponent& to) -> void
            {
                to.enabled = from.enabled;
                to.camera = from.camera;
                to.mainCamera = from.mainCamera;
            });

        CopyComponent<BehaviorComponent>(fromEntity, toEntity,
            [=](const BehaviorComponent& from, BehaviorComponent& to) -> void
            {
                Copy(from, to);
            });

        CopyComponent<EntityHierarchyComponent>(fromEntity, toEntity,
            [=](const EntityHierarchyComponent& from, EntityHierarchyComponent& to) -> void
            {
                Engine::Scene& active = (Engine::Scene&)toEntity.GetScene();
                for (const auto& e : from.GetChildren())
                {
                    // Creates the entity.
                    Engine::Entity created = active.CreateEntity(
                        e.GetComponent<NameComponent>().name, toEntity);
                    CopyEntity(e, created);
                }
            });
    }
}