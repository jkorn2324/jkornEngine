#include "EnginePCH.h"
#include "RenderingAPI.h"

#ifdef GRAPHICS_API_DIRECTX11
#include "DirectX11RenderingAPI.h"
#endif

namespace Engine
{
	RenderingAPI* RenderingAPI::Create()
	{
#if defined(GRAPHICS_API_DIRECTX11)
        return new DirectX11RenderingAPI();
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported rendering api.");
        return nullptr;
#endif
	}

}
