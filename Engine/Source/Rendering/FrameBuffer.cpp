#include "EnginePCH.h"
#include "FrameBuffer.h"

#include "Profiler.h"
#include "GraphicsRenderer.h"
#include "RenderingAPI.h"
#include "DirectX11FrameBuffer.h"

namespace Engine
{

	FrameBuffer::FrameBuffer(const FrameBufferSpecification& specification)
		: m_frameBufferSpecification(),
		m_depthStencilSpecification(),
		m_renderTargetSpecifications()
	{
		m_frameBufferSpecification = specification;

		bool setDepthStencilSpecification = false;
		for (const auto& attachment : specification.attachments)
		{
			if (attachment.textureType == TYPE_DEPTH24_STENCIL8
				&& !setDepthStencilSpecification)
			{
				m_depthStencilSpecification = attachment;
				setDepthStencilSpecification = true;
			}
			else if (attachment.textureType == TYPE_RGBA_32
				|| attachment.textureType == TYPE_INT)
			{
				m_renderTargetSpecifications.push_back(attachment);
			}
		}

	}

	void FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_frameBufferSpecification.width = width;
		m_frameBufferSpecification.height = height;
	}

	FrameBuffer* FrameBuffer::Create(const FrameBufferSpecification& specification)
	{
		PROFILE_SCOPE(CreateFrameBuffer, Rendering);

		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:	return new DirectX11FrameBuffer(specification);
		case RenderingAPIType::NONE:
		{
			DebugAssert(false, "Unsupported Frame Buffer Type.");
			return nullptr;
		}
		}
		return nullptr;
	}
}