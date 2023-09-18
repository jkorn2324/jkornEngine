
#include "EnginePCH.h"
#include "Entity.h"

namespace Engine
{
	// The none entity.
	const Entity Entity::None = Entity();

	EventFunc Entity::s_componentEventFunc = nullptr;

	void Entity::BindEventFunc(const Engine::EventFunc& eventFunc)
	{
		s_componentEventFunc = eventFunc;
	}
}