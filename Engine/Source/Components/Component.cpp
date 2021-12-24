#include "EnginePCH.h"

#include "Component.h"
#include "Entities\Entity.h"

namespace Engine
{
	Component::Component(class Entity* owner)
		: m_owner(owner)
	{
		owner->AddComponent(this);
	}
	
	Component::~Component()
	{
		m_owner->RemoveComponent(this);
	}

	void Component::Update(float deltaTime) { }
	
	Entity* Component::GetOwner() const
	{
		return m_owner;
	}
}