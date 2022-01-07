#pragma once

#include "Layer.h"
#include "Timestep.h"
#include "Source\Matrix.h"
#include "Source\Vector.h"

#include "Event.h"
#include "ApplicationEvent.h"

#include "Entity.h"

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

	template<typename T>
	class Material;
	class Mesh;
	class SubTexture;
}

struct EntityConstants
{
	MathLib::Matrix4x4 c_objectToWorld;
};

struct VertexPositionColor
{
	MathLib::Vector3 position;
	MathLib::Vector4 color;
};

namespace GlfwSandbox
{

	class GlfwGame : public Engine::Layer
	{
	public:
		explicit GlfwGame();
		~GlfwGame();

		void OnUpdate(const Engine::Timestep& ts) override;
		void OnEvent(Engine::Event& event) override;

		void OnImGuiRender() override;

	private:
		void InitializeRenderBuffers();
		void InitializeSceneComponents();
		void Render();

		bool OnWindowResize(Engine::WindowResizedEvent& resizedEvent);

	private:
		Engine::VertexBuffer* m_vertexBuffer;
		Engine::IndexBuffer* m_indexBuffer;

		Engine::Entity m_cameraEntity;
		Engine::Entity* m_spriteEntity;
		Engine::Scene* m_scene;

		Engine::SubTexture* m_subTexture;

		Engine::ConstantBuffer* m_entityConstantBuffer;
		EntityConstants m_entityConstants;
		// TODO: Find a way to load in a mesh or just create one.
	};
}