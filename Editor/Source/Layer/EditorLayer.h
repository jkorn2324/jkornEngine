#pragma once

#include "Layer.h"
#include "Timestep.h"

#include "SceneHierarchy.h"
#include "EntityInspector.h"
#include "ProjectMenu.h"
#include "SceneView.h"
#include "GameView.h"

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
		void DrawMenuBar();

	private:
		SceneHierarchy m_sceneHierarchy;
		EntityInspector m_entityInspector;
		ProjectMenu m_projectMenu;
		SceneView m_sceneView;
		GameView m_gameView;
	};
}