#pragma once

#include <string>

#include "EngineTime.h"

namespace Engine
{
	class IEvent;

	class Layer
	{
	public:
		explicit Layer(const std::string& name);
		virtual ~Layer();

		virtual void OnLayerAdded() { }
		virtual void OnLayerRemoved() { }

		virtual void OnUpdate(const Timestep& ts) { }
		virtual void OnImGuiRender() { }

		virtual void OnEvent(IEvent& event) { }
		const std::string& GetName() const { return m_name; }

	protected:
		std::string m_name;
	};
}