#pragma once

#include "Layer.h"
#include "Timestep.h"
#include "Source\Matrix.h"
#include "Source\Vector.h"

#include "Event.h"
#include "ApplicationEvent.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif

namespace Engine
{
	class GraphicsRenderer;
	class Entity;
	class Scene;
	class VertexBuffer;
	class IndexBuffer;
	class ConstantBuffer;
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

namespace GlfwSandbox
{

	class GlfwGame : public Engine::Layer
	{
	public:
		explicit GlfwGame();
		~GlfwGame();

		void OnUpdate(const Engine::Timestep& ts) override;

	private:
		void InitializeRenderBuffers();
		void InitializeSceneComponents();
		void Render();

	private:
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