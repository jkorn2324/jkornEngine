#pragma once

#include "Texture.h"
#include "MetalUtility.h"

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
