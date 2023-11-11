#include "EnginePCH.h"
#include "FrameBuffer.h"

#include "Profiler.h"
#include "GraphicsRenderer.h"
#include "RenderingAPI.h"

#if defined(GRAPHICS_API_DIRECTX11)
#include "DirectX11FrameBuffer.h"
#endif

#if defined(GRAPHICS_API_METAL)
#include "MetalFrameBuffer.h"
#endif

namespace Engine
{

	FrameBuffer::FrameBuffer(const FrameBufferSpecification& specification)
		: m_frameBufferSpecification()
	{
		m_frameBufferSpecification = specification;
	}

	void FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_frameBufferSpecification.width = width;
		m_frameBufferSpecification.height = height;
	}

	FrameBuffer* FrameBuffer::Create(const FrameBufferSpecification& specification)
	{
		PROFILE_SCOPE(CreateFrameBuffer, Rendering);

#if defined(GRAPHICS_API_DIRECTX11)
        return new DirectX11FrameBuffer(specification);
#elif defined(GRAPHICS_API_METAL)
        return new MetalFrameBuffer(specification);
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported Frame Buffer Type.");
        return nullptr;
#endif
	}
}
