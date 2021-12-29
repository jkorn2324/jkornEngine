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
	class Scene;
	class Entity;
	class Camera;
}

struct EntityConstants
{
	MathLib::Matrix4x4 c_objectToWorld;
};

struct CameraConstants
{
	MathLib::Matrix4x4 c_viewProjection;
	MathLib::Vector3 c_cameraPosition;

private:
	float pad;
};

struct SpriteConstants
{
	MathLib::Vector4 c_spriteColor;
};


struct VertexPositionColor
{
	MathLib::Vector3 position;
	MathLib::Vector4 color;
};

struct SpriteShaderVertex
{
	MathLib::Vector3 position;
	MathLib::Vector2 uv;
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

		Engine::VertexBuffer* m_spriteVertexBuffer;
		Engine::IndexBuffer* m_spriteIndexBuffer;

		Engine::Entity* m_cameraEntity;
		Engine::Entity* m_spriteEntity;

		Engine::Scene* m_scene;

		Engine::ConstantBuffer* m_cameraConstantBuffer;
		CameraConstants m_cameraConstants;

		Engine::ConstantBuffer* m_entityConstantBuffer;
		EntityConstants m_entityConstants;

		Engine::ConstantBuffer* m_spriteConstantBuffer;
		SpriteConstants m_spriteConstants;
	};
}