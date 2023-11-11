#include "EnginePCH.h"
#include "MetalTexture.h"

#include "GraphicsRenderer.h"
#include "MetalRenderingAPI.h"
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

        static TextureSpecifications GetTextureSpecification(MTLTextureUsage usage, MTLResourceOptions resourceOptions, MTLPixelFormat pixelFormat)
        {
            uint32_t textureFlags = 0;
            // Get the texture flags.
            {
                if (usage & MTLTextureUsageRenderTarget)
                {
                    textureFlags |= TextureFlags_IsRenderTarget;
                }
                if (resourceOptions == MTLResourceStorageModeMemoryless)
                {
                    textureFlags |= TextureFlags_Temporary;
                }
            }
            GraphicsFormat graphicsFormat = Utility::Metal::ToGraphicsFormat(pixelFormat);
            
            uint32_t rwFlags = 0;
            if (usage & MTLTextureUsageShaderRead)
            {
                rwFlags |= Flag_GPU_ReadTexture;
            }
            if (usage & MTLTextureUsageShaderWrite)
            {
                rwFlags |= Flag_GPU_WriteTexture;
            }
            
            if (textureFlags & TextureFlags_Temporary
                || resourceOptions == MTLResourceStorageModePrivate)
            {
                return TextureSpecifications(rwFlags, graphicsFormat, textureFlags);
            }
            
            if (resourceOptions == MTLResourceCPUCacheModeDefaultCache)
            {
                rwFlags |= Flag_CPU_WriteTexture | Flag_CPU_ReadTexture;
            }
            else if (resourceOptions == MTLResourceCPUCacheModeWriteCombined)
            {
                rwFlags |= Flag_CPU_WriteTexture;
            }
            else if (resourceOptions == MTLResourceStorageModeShared)
            {
                rwFlags |= Flag_CPU_ReadTexture;
            }
            return TextureSpecifications(rwFlags, graphicsFormat, textureFlags);
        }

        static MTLResourceOptions GetTextureResourceOptions(TextureReadWriteFlags rwFlags, bool temporary)
        {
            if (temporary)
            {
                return MTLResourceStorageModeMemoryless;
            }
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
                return MTLResourceStorageModeShared;
            }
            return MTLResourceStorageModePrivate;
        }
    }

    MetalTexture::MetalTexture()
        : Texture()
    {
        
    }

    MetalTexture::MetalTexture(uint32_t width, uint32_t height, const TextureSpecifications& specifications)
        : Texture(width, height, specifications)
    {
        MTLTextureDescriptor* textureDescriptor = [[MTLTextureDescriptor alloc] init];
        textureDescriptor.width = (NSUInteger)width;
        textureDescriptor.height = (NSUInteger)height;
        textureDescriptor.pixelFormat = Utility::Metal::ToMTLPixelFormat(specifications.textureFormat);
        textureDescriptor.mipmapLevelCount = 1;
        textureDescriptor.resourceOptions = Utilities::Metal::GetTextureResourceOptions(specifications.readWriteFlags, specifications.textureFlags & TextureFlags_Temporary);
        textureDescriptor.usage = Utilities::Metal::GetTextureUsage(specifications.readWriteFlags, false, specifications.textureFlags & TextureFlags_IsRenderTarget);
        // Sets the current texture type.
        // TODO: Allow this to change.
        textureDescriptor.textureType = MTLTextureType2D;
        
        MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
        m_texture = [renderingAPI.GetDevice() newTextureWithDescriptor:textureDescriptor];
        
        JKORN_ENGINE_ASSERT(m_texture != nullptr, "The texture must be valid.");
    }

    MetalTexture::MetalTexture(MTLTexturePtr texturePtr)
        : Texture(), m_texture(texturePtr)
    {
        JKORN_ENGINE_ASSERT(m_texture != nullptr, "The metal texture pointer must be valid.");
        m_width = (uint32_t)m_texture.width;
        m_height = (uint32_t)m_texture.height;
        m_specifications = Utilities::Metal::GetTextureSpecification(m_texture.usage, m_texture.resourceOptions, m_texture.pixelFormat);
    }

    MetalTexture::~MetalTexture()
    {
        if (m_texture)
        {
            [m_texture release];
        }
        m_texture = nullptr;
    }

    bool MetalTexture::IsValid() const
    {
        return m_texture != nullptr;
    }

    void MetalTexture::Bind(uint32_t textureSlot) const
    {
        // Don't bind texture at slot if it isn't valid.
        if (!IsValid())
        {
            return;
        }
        // Sets the fragment texture.
        MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
        MTLRenderCommandEncoderPtr commandEncoder = renderingAPI.GetRenderCommandEncoder();
        JKORN_ENGINE_ASSERT(commandEncoder != nil, "There must be an active command encoder");
        [commandEncoder setFragmentTexture:m_texture atIndex: (NSInteger)textureSlot];
    }

    bool MetalTexture::GetPixel(uint32_t x, uint32_t y, MathLib::Vector4 &pixel) const
    {
        if (!IsValid())
        {
            return false;
        }
        JKORN_ENGINE_ASSERT(false, "Implement GetPixel Function in MetalTexture.");
        return false;
    }

    void MetalTexture::SetPixel(uint32_t x, uint32_t y, const MathLib::Vector4 &pixel)
    {
        JKORN_ENGINE_ASSERT(false, "Implement SetPixel Function in MetalTexture.");
    }

    void MetalTexture::CopyPixels(BufferModifier &view) const
    {
        // TODO: Implementation
        JKORN_ENGINE_ASSERT(false, "Implement CopyPixels Function in Metal Texture.");
    }

    bool MetalTexture::LoadFromFile_Internal(const wchar_t *texturePath)
    {
        JKORN_ENGINE_ASSERT(false, "Implement LoadFromFile_Internal in Metal Texture.");
        return false;
    }

    bool MetalTexture::CopyTo(Texture &b)
    {
        JKORN_ENGINE_ASSERT(false, "Implement the CopyTo Function in Metal.");
        return false;
    }
}
