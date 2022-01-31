#pragma once

#include "EditorCamera.h"

namespace Engine
{
	class FrameBuffer;
	class Timestep;
	class Event;
	class Texture;
}

namespace Editor
{

	class SceneView
	{
	public:
		SceneView();
		~SceneView();

		void OnEvent(Engine::Event& event);

		void OnUpdate(const Engine::Timestep& ts);
		void RenderScene();
		void Draw();

		void SetOpen(bool open) { m_open = open; }
		bool IsOpen() const { return m_open; }

		bool IsFocused() const { return m_focused; }

		const MathLib::Vector2& GetWindowSize() const { return m_windowSize; }

	private:
		void HandleCameraInput(const Engine::Timestep& ts);
		bool GetCameraDirection(MathLib::Vector3& cameraDirection,
			const EditorCamera& editorCamera, const Engine::PlatformInput& platformInput) const;

	private:
		Engine::FrameBuffer* m_frameBuffer;
		MathLib::Vector2 m_windowSize;
		float m_windowBarSpacing;
		MathLib::Vector2 m_prevMousePos;
		MathLib::Vector2 m_mouseScroll;
		bool m_open, m_focused;

		// TODO: Separate into an editor input system.
	};
}