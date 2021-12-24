#pragma once

#include "Source\Vector.h"
#include "Source\Matrix.h"

namespace Engine
{
	class GraphicsRenderer;
	class Shader;
	class VertexBuffer;
	class IndexBuffer;
	class ConstantBuffer;
	class Entity;
	class CameraComponent;
}

struct VertexPositionColor
{
	MathLib::Vector3 position;
	MathLib::Vector4 color;
};

const float MAX_TIME_BETWEEN_FRAMES = 0.9f / 60.0f;

namespace DirectXTestProject
{

	class Game
	{
	public:
		Game();
		~Game();

		bool Initialize(const HWND& window);
		void Update(float deltaTime);
		void Render();

		void InitializeRenderBuffers();

	private:
		Engine::GraphicsRenderer* m_graphicsRenderer;
		Engine::VertexBuffer* m_vertexBuffer;
		Engine::IndexBuffer* m_indexBuffer;
		Engine::Shader* m_shader;

		Engine::Entity* m_cameraEntity;
	};
}