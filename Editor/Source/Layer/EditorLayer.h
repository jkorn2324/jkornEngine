#pragma once

#include "Layer.h"
#include "Timestep.h"

#include "SceneHierarchy.h"

namespace Engine
{
	class FrameBuffer;
	class Event;
}

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

		void OnEvent(Engine::Event& event) override;

	private:
		Engine::FrameBuffer* m_frameBuffer;
		SceneHierarchy m_sceneHierarchy;
	};
}