#pragma once

#include "EditorCamera.h"

namespace Engine
{
	class FrameBuffer;
	class Timestep;
	class Event;
}

namespace Editor
{

	class SceneView
	{
	public:
		SceneView();
		~SceneView();

		void OnEvent(Engine::Event& event);

		void RenderScene();
		void Draw();

		void SetOpen(bool open) { m_open = open; }
		bool IsOpen() const { return m_open; }

	private:
		Engine::FrameBuffer* m_frameBuffer;
		bool m_open;
	};
}