#include "EnginePCH.h"
#include "GraphicsRenderer.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "RenderingAPI.h"

#include "Source\Vector.h"

namespace Engine
{

	GraphicsRenderer* GraphicsRenderer::s_graphicsRenderer = nullptr;


	GraphicsRenderer* GraphicsRenderer::Get()
	{
		return s_graphicsRenderer;
	}

	GraphicsRenderer::GraphicsRenderer()
		: m_renderingAPI(nullptr),
		m_activeIndexBuffer(nullptr),
		m_activeVertexBuffer(nullptr),
		m_activeShader(nullptr),
		m_frameBuffer(nullptr)
	{
		DebugAssert(s_graphicsRenderer == nullptr, 
			"There should only be one graphics renderer.");
		s_graphicsRenderer = this;
		m_renderingAPI = RenderingAPI::Create();
	}

	GraphicsRenderer::~GraphicsRenderer()
	{
		if (s_graphicsRenderer == this)
		{
			s_graphicsRenderer = nullptr;
		}
		delete m_frameBuffer;
		delete m_renderingAPI;
	}

	bool GraphicsRenderer::Initialize(Window* window)
	{
		if (!m_renderingAPI->Initialize(window))
		{
			return false;
		}

		FrameBufferSpecification frameBufferSpecification({
			{ FrameBufferAttachmentType::DEPTH_STENCIL }
		});
		frameBufferSpecification.width = m_renderingAPI->GetWidth();
		frameBufferSpecification.height = m_renderingAPI->GetHeight();
		m_frameBuffer = FrameBuffer::Create(frameBufferSpecification);
		return true;
	}

	RenderingAPI* GraphicsRenderer::GetRenderingAPI() const
	{
		return m_renderingAPI;
	}

	void GraphicsRenderer::BeginFrame()
	{
		// Clears the render target color & depth buffer.
		m_frameBuffer->Bind();
	}

	void GraphicsRenderer::EndFrame()
	{
		m_renderingAPI->SwapBuffers();
	}

	void GraphicsRenderer::SetTexture(std::uint32_t slot, Texture* texture)
	{
		if (texture != nullptr)
		{
			texture->Bind(slot);
		}
	}

	void GraphicsRenderer::SetActiveIndexBuffer(IndexBuffer* indexBuffer)
	{
		if (m_activeIndexBuffer != indexBuffer)
		{
			m_activeIndexBuffer = indexBuffer;
			if (m_activeIndexBuffer != nullptr)
			{
				m_activeIndexBuffer->Bind();
			}
		}
	}

	void GraphicsRenderer::SetActiveVertexBuffer(VertexBuffer* vertexBuffer)
	{
		if (m_activeVertexBuffer != vertexBuffer)
		{
			m_activeVertexBuffer = vertexBuffer;
			if (m_activeVertexBuffer != nullptr)
			{
				m_activeVertexBuffer->Bind();
			}
		}
	}

	void GraphicsRenderer::SetConstantBuffer(const std::size_t& slot, ConstantBuffer* constantBuffer)
	{
		int flags = ConstantBufferFlags::COMPUTE_SHADER | ConstantBufferFlags::HULL_SHADER
			| ConstantBufferFlags::PIXEL_SHADER | ConstantBufferFlags::VERTEX_SHADER;
		SetConstantBuffer(slot, constantBuffer, flags);
	}

	void GraphicsRenderer::SetConstantBuffer(const std::size_t& slot, ConstantBuffer* constantBuffer, int flags)
	{
		if (constantBuffer != nullptr)
		{
			constantBuffer->Bind(slot, flags);
		}
	}

	void GraphicsRenderer::SetShader(class Shader* shader)
	{
		if (m_activeShader != shader)
		{
			m_activeShader = shader;
			
			if (m_activeShader != nullptr)
			{
				m_activeShader->Bind();
			}
		}
	}

	void GraphicsRenderer::DrawActiveElements()
	{
		if (m_activeVertexBuffer != nullptr
			&& m_activeShader != nullptr)
		{
			// Issues a draw call.
			m_renderingAPI->Draw(m_activeVertexBuffer,
				m_activeIndexBuffer);
		}
	}
}