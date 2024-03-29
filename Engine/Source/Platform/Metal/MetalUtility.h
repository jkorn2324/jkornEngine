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
            return MTLPixelFormatInvalid;
        }
    
        constexpr GraphicsFormat ToGraphicsFormat(MTLPixelFormat pixelFormat)
        {
            switch (pixelFormat)
            {
                case MTLPixelFormatRGBA8Uint:
                    return GraphicsFormat_RGBA8_UInt;
                case MTLPixelFormatRGBA8Sint:
                    return GraphicsFormat_RGBA8_SInt;
                    
                case MTLPixelFormatRGBA32Uint:
                    return GraphicsFormat_RGBA32_UInt;
                case MTLPixelFormatRGBA32Float:
                    return GraphicsFormat_RGBA32_Float;
                case MTLPixelFormatRGBA32Sint:
                    return GraphicsFormat_RGBA32_SInt;
                    
                case MTLPixelFormatR32Uint:
                    return GraphicsFormat_UInt32;
                case MTLPixelFormatR32Sint:
                    return GraphicsFormat_SInt32;
                case MTLPixelFormatR32Float:
                    return GraphicsFormat_Float32;
                    
                case MTLPixelFormatR16Sint:
                    return GraphicsFormat_SInt16;
                case MTLPixelFormatR16Uint:
                    return GraphicsFormat_UInt16;
                    
                case MTLPixelFormatR8Uint:
                    return GraphicsFormat_UInt8;
                case MTLPixelFormatR8Sint:
                    return GraphicsFormat_SInt8;
                    
                case MTLPixelFormatDepth24Unorm_Stencil8:
                    return GraphicsFormat_D24UNorm_S8UInt;
                case MTLPixelFormatDepth32Float_Stencil8:
                    return GraphicsFormat_D32Float_S8UInt;
                case MTLPixelFormatDepth32Float:
                    return GraphicsFormat_D32Float;
            }
            return GraphicsFormat_Unknown;
        }
    
        inline MTLDepthStencilStatePtr CreateDepthStencilState(MTLCompareFunction compareFunc, MTLDevicePtr device)
        {
            MTLDepthStencilDescriptor* descriptor = [[MTLDepthStencilDescriptor alloc] init];
            descriptor.depthWriteEnabled = true;
            descriptor.depthCompareFunction = compareFunc;
            
            // The front face description.
            MTLStencilDescriptor* frontFaceDesc = [[MTLStencilDescriptor alloc] init];
            frontFaceDesc.stencilFailureOperation = MTLStencilOperationKeep;
            frontFaceDesc.depthFailureOperation = MTLStencilOperationIncrementClamp;
            frontFaceDesc.depthStencilPassOperation = MTLStencilOperationKeep;
            frontFaceDesc.stencilCompareFunction = MTLCompareFunctionAlways;
            frontFaceDesc.writeMask = MTLColorWriteMaskAll;
            frontFaceDesc.readMask = MTLColorWriteMaskAll;
            descriptor.frontFaceStencil = frontFaceDesc;
            
            // The back face description.
            MTLStencilDescriptor* backFaceDesc = [[MTLStencilDescriptor alloc] init];
            backFaceDesc.stencilFailureOperation = MTLStencilOperationKeep;
            backFaceDesc.depthFailureOperation = MTLStencilOperationDecrementClamp;
            backFaceDesc.depthStencilPassOperation = MTLStencilOperationKeep;
            backFaceDesc.stencilCompareFunction = MTLCompareFunctionAlways;
            backFaceDesc.writeMask = MTLColorWriteMaskAll;
            backFaceDesc.readMask = MTLColorWriteMaskAll;
            descriptor.backFaceStencil = backFaceDesc;
            
            return [device newDepthStencilStateWithDescriptor:descriptor];
        }
    }
}
