#pragma once

#include "Layer.h"
#include "EngineTime.h"

#include "SceneHierarchy.h"
#include "EntityInspector.h"
#include "ProjectMenu.h"
#include "SceneView.h"
#include "GameView.h"

namespace Engine
{
	class FrameBuffer;
	class IEvent;
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

		void OnUpdate(const Engine::Timestep& timestep) override;
		void OnImGuiRender() override;

		void OnEvent(Engine::IEvent& event) override;

	private:
		void DrawEditorTopWindow();
		void DrawEditorMainWindow();

		void DrawMenuBar();
		void DrawEditorButtons();

		void OnSceneRuntimeButtonSelected(bool play);

	private:
		SceneHierarchy m_sceneHierarchy;
		EntityInspector m_entityInspector;
		ProjectMenu m_projectMenu;
		SceneView m_sceneView;
		GameView m_gameView;
	};
}
