#pragma once

#include "EditorCamera.h"
#include "EditorWidgets.h"

namespace Engine
{
	class FrameBuffer;
	class Timestep;
	class IEvent;
	class Texture;
}

namespace Editor
{

	class SceneView
	{
	public:
		SceneView();
		~SceneView();

		void OnEvent(Engine::IEvent& event);

		void OnUpdate(const Engine::Timestep& ts);
		void RenderScene();
		void Draw();

		void SetOpen(bool open) { m_open = open; }
		bool IsOpen() const { return m_open; }

		bool IsFocused() const { return m_focused; }

		const MathLib::Vector2& GetWindowSize() const { return m_windowSize; }

		TransformationWidget& GetTransformationWidget() { return m_transformationWidget; }
		const TransformationWidget& GetTransformationWidget() const { return m_transformationWidget; }

	private:
		void HandleCameraInput(const Engine::Timestep& ts);
		void UpdateTransformationWidget(const Engine::Timestep& ts);
		void UpdateEntityIDs(const Engine::Timestep& ts);
		void UpdateMousePosition(const Engine::Timestep& ts);

		bool GetCameraDirection(MathLib::Vector3& cameraDirection,
			const EditorCamera& editorCamera, const Engine::PlatformInput& platformInput) const;
		const MathLib::Vector2 GetResizedWindow() const;

		void DrawTransformationWidgetInput();

	private:
		Engine::FrameBuffer* m_frameBuffer;
		Engine::FixedRuntimeBuffer<Engine::DefaultAllocator> m_sceneViewEntityIDs;

		TransformationWidget m_transformationWidget;

		MathLib::Vector2 m_windowPosition;
		MathLib::Vector2 m_windowSize;
		float m_windowBarSpacing;
		
		MathLib::Vector2 m_prevMousePos;
		MathLib::Vector2 m_currMousePos;
		MathLib::Vector2 m_mouseScroll;
		
		Engine::Entity m_prevSelectedEntity;
		bool m_open, m_focused, m_entityWidgetChanged;
	};
}