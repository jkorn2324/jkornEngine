#pragma once

#include <string>

#include "Timestep.h"

namespace Engine
{


	class Layer
	{
	public:
		explicit Layer(const std::string& name);
		virtual ~Layer();

		virtual void OnLayerAdded() { }
		virtual void OnLayerRemoved() { }

		virtual void OnUpdate(const Timestep& ts) { }
		virtual void OnEvent(class Event& event) { }
		const std::string& GetName() const { return m_name; }

	protected:
		std::string m_name;
	};
}