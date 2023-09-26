#include "EnginePCH.h"
#include "RenderingAPI.h"

#ifdef GRAPHICS_API_DIRECTX11
#include "DirectX11RenderingAPI.h"
#endif

namespace Engine
{

    constexpr RenderingAPIType RenderingAPI::GetRenderingAPIType()
    {
#if defined(GRAPHICS_API_DIRECTX11)
        return RenderingAPIType::DIRECTX11;
#elif defined(GRAPHICS_API_METAL)
        return RenderingAPIType::METAL;
#else
        return RenderingAPIType::NONE
#endif
    }

	RenderingAPI* RenderingAPI::Create()
	{
#if defined(GRAPHICS_API_DIRECTX11)
        return new DirectX11RenderingAPI();
#else
        DebugAssert(false, "Unsupported rendering api.");
        return nullptr;
#endif
	}

}
