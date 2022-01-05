#include "EnginePCH.h"
#include "FrameBuffer.h"

#include "GraphicsRenderer.h"
#include "RenderingAPI.h"
#include "DirectX11FrameBuffer.h"

namespace Engine
{

	FrameBuffer::FrameBuffer(const FrameBufferSpecification& specification)
		: m_frameBufferSpecification(),
		m_depthStencilSpecification()
	{
		m_frameBufferSpecification = specification;

		for (const auto& attachment : specification.attachments)
		{
			if (attachment.textureType == TYPE_DEPTH24_STENCIL8)
			{
				m_depthStencilSpecification = attachment;
				continue;
			}
		}
	}

	FrameBuffer* FrameBuffer::Create(const FrameBufferSpecification& specification)
	{
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