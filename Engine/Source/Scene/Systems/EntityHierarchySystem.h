#pragma once

#include "EntityHierarchyComponent.h"
#include "IUpdateSystem.h"
#include "IEventSystem.h"

namespace Engine
{

	/**
	 * The entity hierarchy system. 
	 */
	class EntityHierarchySystem : public IUpdateSystem<EntityHierarchyComponent>
	{
	protected:
		/**
		 * Updates the entity hierarchy system. 
		 */
		void OnUpdate(const UpdateSystemContext& context, EntityRef& entity, Components& components) override;
	};
}