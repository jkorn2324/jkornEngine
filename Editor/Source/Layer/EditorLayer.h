#pragma once

#include "Layer.h"
#include "Timestep.h"

namespace Editor
{

	class EditorLayer : public Engine::Layer
	{
	public:
		explicit EditorLayer();
		~EditorLayer();

		void OnLayerAdded() override;
		void OnLayerRemoved() override;

		void OnUpdate(const Engine::Timestep& timestep);
		void OnImGuiRender() override;
	};
}