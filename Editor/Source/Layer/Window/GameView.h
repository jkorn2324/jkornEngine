#pragma once

namespace Engine
{
	class FrameBuffer;
	class Timestep;
}

namespace Editor
{

	class GameView
	{
	public:
		GameView();
		~GameView();

		void SetOpen(bool open) { m_open = open; }
		bool IsOpen() const { return m_open; }

		void RenderScene();
		void Draw();

	private:
		Engine::FrameBuffer* m_frameBuffer;
		bool m_open;
	};
}