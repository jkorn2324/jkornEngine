#pragma once

#include "Texture.h"
#include "MetalFwdDecl.h"

#include <Metal/Metal.h>

namespace Engine
{

    namespace Utilities::Metal
    {
        static MTLTextureUsage GetTextureUsage(TextureReadWriteFlags rwFlags, bool reinterpretToDifferentPixelFormats, bool isRenderTarget)
        {
            MTLTextureUsage usage = 0;
            if (rwFlags & Flag_GPU_WriteTexture)
            {
                usage |= MTLTextureUsageShaderWrite;
            }
            if (rwFlags & Flag_GPU_ReadTexture)
            {
                usage |= MTLTextureUsageShaderRead;
            }
            if (reinterpretToDifferentPixelFormats)
            {
                usage |= MTLTextureUsagePixelFormatView;
            }
            if (isRenderTarget)
            {
                usage |= MTLTextureUsageRenderTarget;
            }
            // TODO: Add Atomic Operations
            return usage;
        }
    
        static MTLResourceOptions GetTextureResourceOptions(TextureReadWriteFlags rwFlags, bool temporary)
        {
            // Checks if the CPU Can Read & Write.
            if (rwFlags & Flag_CPU_WriteTexture)
            {
                if (rwFlags & Flag_GPU_ReadTexture
                    || rwFlags & Flag_GPU_WriteTexture)
                {
                    return MTLResourceStorageModeShared;
                }
                
                if (rwFlags & Flag_CPU_ReadTexture)
                {
                    return MTLResourceCPUCacheModeDefaultCache;
                }
                return MTLResourceCPUCacheModeWriteCombined;
            }
            
            if (rwFlags & Flag_CPU_ReadTexture)
            {
                return MTLStorageModeShared;
            }
            return temporary ? MTLResourceStorageModeMemoryless : MTLResourceStorageModePrivate;
        }
    
        static MTLPixelFormat ToMTLTextureFormat(TextureFormat textureFormat)
        {
            switch (textureFormat)
            {
                case TextureFormat_Int8:
                    return MTLPixelFormatR8Sint;
                case TextureFormat_Int16:
                    return MTLPixelFormatR16Sint;
                case TextureFormat_Int32:
                    return MTLPixelFormatR32Sint;
                case TextureFormat_Float32:
                    return MTLPixelFormatR32Float;
                    
                case TextureFormat_RGBA32_SInt:
                    return MTLPixelFormatRGBA32Sint;
                case TextureFormat_RGBA32_UInt:
                case TextureFormat_RGBA32_Typeless:
                    return MTLPixelFormatRGBA32Uint;
                case TextureFormat_RGBA32_Float:
                    return MTLPixelFormatRGBA32Float;
                    
                case TextureFormat_RGBA8_UInt:
                case TextureFormat_RGBA8_Typeless:
                    return MTLPixelFormatRGBA8Uint;
                case TextureFormat_RGBA8_SInt:
                    return MTLPixelFormatRGBA8Sint;
                default: break;
            }
            JKORN_ENGINE_ASSERT(false, "Invalid Texture Format.");
            return MTLPixelFormatInvalid;
        }
    }

    class MetalTexture : public Texture
    {
    public:
        MetalTexture();
        MetalTexture(uint32_t width, uint32_t height, const TextureSpecifications& specifications);
        ~MetalTexture();
        
        bool IsValid() const override;
        const void* GetTextureID() const override { return m_texture; }
        
        void Bind(uint32_t textureSlot) const override;
        
        bool GetPixel(uint32_t x, uint32_t y, MathLib::Vector4& pixel) const override;
        
        void SetPixel(uint32_t x, uint32_t y, const MathLib::Vector4& pixel) override;
        
        void CopyPixels(BufferModifier& view) const override;
        
    protected:
        bool LoadFromFile_Internal(const wchar_t* texturePath) override;
        bool CopyTo(Texture& b) override;

    private:
        MTLTexturePtr m_texture;
    };
}
