#pragma once

#include <vector>
#include <memory>

#include "Source\Transform.h"

namespace Engine
{

	class Component;

	class Entity
	{
	public:
		// TODO: Make this so that it requires a scene.
		explicit Entity();
		~Entity();

		template<typename T>
		T* GetComponent() const;

		const MathLib::Transform3D& GetTransform() const;

		void Update(const float& deltaTime);

	private:
		void AddComponent(Component* component);
		void RemoveComponent(Component* component);

	private:
		std::vector<Component*> m_components;
		MathLib::Transform3D m_transform;

		friend class Component;
	};

	template<typename T>
	T* Entity::GetComponent() const
	{
		std::uint32_t componentsCount = m_components.size();
		for (std::uint32_t i = 0; i < componentsCount; i++)
		{
			class Component* c = m_components[i];
			T* dynamicCast = dynamic_cast<T*>(c);
			if (dynamicCast != nullptr)
			{
				return dynamicCast;
			}
		}
		return nullptr;
	}
}