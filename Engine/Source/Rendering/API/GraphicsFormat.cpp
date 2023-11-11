#include "EnginePCH.h"
#include "GraphicsFormat.h"

#if defined(GRAPHICS_API_DIRECTX11)
#include <d3d11.h>
#endif

namespace Engine
{
    constexpr GraphicsFormat Graphics::GetDefaultDepthStencilFormat()
    {
        // Gets the system default depth format.
#if defined(PLATFORM_MACOS)
        return GraphicsFormat_D32Float_S8UInt;
#elif defined(PLATFORM_WINDOWS)
        return GraphicsFormat_D24UNorm_S8UInt;
#else
        return GraphicsFormat_Unknown;
#endif
    }

    constexpr size_t Graphics::GetMaxRenderTargets()
    {
#if defined(PLATFORM_MACOS)
        return 8;
#elif defined(PLATFORM_WINDOWS)
#if defined(GRAPHICS_API_DIRECTX11)
        return D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;
#else
        return 0;
#endif
#else
        return 0;
#endif
    }
}
