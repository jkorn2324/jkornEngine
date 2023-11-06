#pragma once

#include "MetalFwdDecl.h"
#include "GraphicsFormat.h"

#include <Metal/Metal.h>

namespace Engine
{

    namespace Utility::Metal
    {
        constexpr MTLPixelFormat ToMTLPixelFormat(GraphicsFormat graphicsFormat)
        {
            switch (graphicsFormat)
            {
                case GraphicsFormat_RGBA8_Typeless:
                case GraphicsFormat_RGBA8_UInt:
                    return MTLPixelFormatRGBA8Uint;
                case GraphicsFormat_RGBA8_SInt:
                    return MTLPixelFormatRGBA8Sint;
                    
                case GraphicsFormat_RGBA32_UInt:
                case GraphicsFormat_RGBA32_Typeless:
                    return MTLPixelFormatRGBA32Uint;
                case GraphicsFormat_RGBA32_SInt:
                    return MTLPixelFormatRGBA32Sint;
                case GraphicsFormat_RGBA32_Float:
                    return MTLPixelFormatRGBA32Float;
                    
                case GraphicsFormat_UInt32:
                case GraphicsFormat_Typeless32:
                    return MTLPixelFormatR32Uint;
                case GraphicsFormat_SInt32:
                    return MTLPixelFormatR32Sint;
                case GraphicsFormat_Float32:
                    return MTLPixelFormatR32Float;
                    
                case GraphicsFormat_UInt16:
                case GraphicsFormat_Typeless16:
                    return MTLPixelFormatR16Uint;
                case GraphicsFormat_SInt16:
                    return MTLPixelFormatR16Sint;
                    
                case GraphicsFormat_UInt8:
                case GraphicsFormat_Typeless8:
                    return MTLPixelFormatR8Uint;
                case GraphicsFormat_SInt8:
                    return MTLPixelFormatR8Sint;
                    
                case GraphicsFormat_D24UNorm_S8UInt:
                    return MTLPixelFormatDepth24Unorm_Stencil8;
                case GraphicsFormat_D32Float_S8UInt:
                    return MTLPixelFormatDepth32Float_Stencil8;
                case GraphicsFormat_D32Float:
                    return MTLPixelFormatDepth32Float;
            }
            JKORN_ENGINE_ASSERT(false, "Invalid Metal Graphics Format");
            return MTLPixelFormatInvalid;
        }
    }
}
