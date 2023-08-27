#pragma once

#include "Source\Matrix.h"
#include "Source\Vector.h"

#include "Engine.h"

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
		void OnEvent(Engine::IEvent& event) override;

		void OnImGuiRender() override;

	private:
		void InitializeRenderBuffers();
		void InitializeSceneComponents();
		void Render();

		bool OnWindowResize(Engine::WindowResizedEvent& resizedEvent);

	private:
		Engine::VertexBuffer* m_vertexBuffer;
		Engine::IndexBuffer* m_indexBuffer;

		Engine::SubTexture* m_subTexture;

		Engine::ConstantBuffer* m_entityConstantBuffer;
		EntityConstants m_entityConstants;

		Engine::FrameBuffer* m_frameBuffer;
		// TODO: Find a way to load in a mesh or just create one.
	};
}