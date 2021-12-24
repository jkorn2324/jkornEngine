#pragma once

#include <functional>

namespace Engine
{

	class Component
	{
	public:
		explicit Component(class Entity* owner);
		virtual ~Component();

		virtual void Update(float deltaTime);
		class Entity* GetOwner() const;

	private:
		class Entity* m_owner;
	};
}