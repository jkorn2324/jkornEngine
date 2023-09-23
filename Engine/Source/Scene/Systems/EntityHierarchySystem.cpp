#include "EnginePCH.h"
#include "EntityHierarchySystem.h"

#include "Scene.h"
#include "Source/Matrix.h"

namespace Engine
{
	namespace EHS::Internal
	{

		template<typename TRegistry = entt::registry>
		void UpdateEntityHierarchies(Entity& rootEntity, EntityHierarchyComponent& rootComponent, TRegistry& registry)
		{
			if (!rootComponent.HasChildren())
			{
				return;
			}

			EntityRef rootEntityRef(rootEntity, registry);
			MathLib::Matrix4x4 mat = MathLib::Matrix4x4::Identity;
			{
				if (rootEntity.HasComponent<Transform3DComponent, TRegistry>(registry))
				{
					mat = rootEntity.GetComponent<Transform3DComponent>(registry).GetTransformMatrix();
				}
				if (rootEntity.HasComponent<Transform2DComponent, TRegistry>(registry))
				{
					mat = rootEntity.GetComponent<Transform2DComponent, TRegistry>(registry).GetTransformMatrix();
				}
			}

			int32_t size = (int32_t)rootComponent.GetChildren().size() - 1;
			for (int32_t i = size; i >= 0; i--)
			{
				auto entity = rootComponent.GetChildren()[i];
				EntityRef e(entity, registry);

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
					UpdateEntityHierarchies<TRegistry>(entity, hierarchyComponent, registry);
				}
			}
		}

		void UpdateHierarchies(EntityRef& reference, EntityHierarchyComponent& component)
		{
			Entity rootEntity = reference.GetEntity();
			UpdateEntityHierarchies(rootEntity, component, reference.GetRegistry());
		}
	}

	void EntityHierarchySystem::OnUpdate(const UpdateSystemContext& context, EntityRef& entity, Components& components)
	{
		EntityHierarchyComponent& component = GetComponent<0>(components);
		if (component.HasParent(entity.GetRegistry()))
		{
			return;
		}
		EHS::Internal::UpdateHierarchies(entity, component);
	}
}
