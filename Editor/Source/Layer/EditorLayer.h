#pragma once

#include "Layer.h"
#include "Timestep.h"

#include "SceneHierarchy.h"
#include "EntityInspector.h"
#include "ProjectMenu.h"

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
		Engine::FrameBuffer* m_editorFrameBuffer;
		Engine::FrameBuffer* m_runtimeFrameBuffer;

		SceneHierarchy m_sceneHierarchy;
		EntityInspector m_entityInspector;
		ProjectMenu m_projectMenu;
	};
}