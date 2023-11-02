#include "EnginePCH.h"
#include "VertexArray.h"

#include "GraphicsRenderer.h"
#include "RenderingAPI.h"

#if defined(GRAPHICS_API_DIRECTX11)
#include "DirectX11VertexArray.h"
#endif

namespace Engine
{
	bool VertexArray::Create(VertexArray** outVertexArray)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        *outVertexArray = new DirectX11VertexArray();
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Failed to create Vertex Array.");
        return false;
#endif
	}

	bool VertexArray::Create(std::shared_ptr<VertexArray>& outVertexArray)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        outVertexArray = std::make_shared<DirectX11VertexArray>();
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Failed to Create Vertex Array.");
        return false;
#endif
	}

	bool VertexArray::Create(std::unique_ptr<VertexArray>& outVertexArray)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        outVertexArray = std::make_unique<DirectX11VertexArray>();
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Failed to Create Vertex Array.");
        return false;
#endif
	}
}
