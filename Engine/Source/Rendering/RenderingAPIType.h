#pragma once

namespace Engine
{

    /**
     * @brief The rendering API type.
     */
	enum class RenderingAPIType
	{
		NONE,
        // The directx graphics rendering api.
		DIRECTX11,
        // The metal graphics rendering api.
        METAL
	};

// Defines the Active Rendering API Type.

#if defined(GRAPHICS_API_METAL)
constexpr RenderingAPIType g_ActiveRenderingAPIType = RenderingAPIType::METAL;
#elif defined(GRAPHICS_API_DIRECTX11)
constexpr RenderingAPIType g_ActiveRenderingAPIType = RenderingAPIType::DIRECTX11;
#else
constexpr RenderingAPIType g_ActiveRenderingAPIType = RenderingAPIType::NONE;
#endif

}
