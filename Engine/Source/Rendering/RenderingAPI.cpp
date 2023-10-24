#include "EnginePCH.h"
#include "RenderingAPI.h"

#ifdef GRAPHICS_API_DIRECTX11
#include "DirectX11RenderingAPI.h"
#endif

#ifdef GRAPHICS_API_METAL
#include "MetalRenderingAPI.h"
#endif

namespace Engine
{
	RenderingAPI* RenderingAPI::Create()
	{
#if defined(GRAPHICS_API_DIRECTX11)
        return new DirectX11RenderingAPI();
#elif defined(GRAPHICS_API_METAL)
        return new MetalRenderingAPI();
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported rendering api.");
        return nullptr;
#endif
	}

}
