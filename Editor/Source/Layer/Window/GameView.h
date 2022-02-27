#pragma once

namespace Engine
{
	class FrameBuffer;
	class Timestep;
}

namespace Editor
{
	enum GameViewAspectRatioType
	{
		ASPECT_FREE,
		ASPECT_16_9,
		ASPECT_16_10,
		ASPECT_21_9,
		ASPECT_4_3,
		ASPECT_3_2
	};

	struct GameViewAspectRatio
	{
		float m_aspectWidth;
		float m_aspectHeight;
	};

	class GameView
	{
	public:
		GameView();
		~GameView();

		void SetOpen(bool open) { m_open = open; }
		bool IsOpen() const { return m_open; }

		const MathLib::Vector2& GetWindowSize() const { return m_windowSize; }

		void OnUpdate(const Engine::Timestep& ts);
		void RenderScene();
		void Draw();

	private:
		void UpdateGameAspectRatio();
		void ConstrainToAspectRatio(float x, float y);

		void DrawMenuBar();

	private:
		Engine::FrameBuffer* m_frameBuffer;
		MathLib::Vector2 m_windowSize;
		float m_windowMenuBarSpacing;
		GameViewAspectRatio m_aspectRatio;
		GameViewAspectRatioType m_aspectRatioType = ASPECT_FREE;
		Engine::SceneCameraType m_sceneCameraType = Engine::SceneCameraType::TYPE_PERSPECTIVE;
		bool m_open, m_focused;
	};
}