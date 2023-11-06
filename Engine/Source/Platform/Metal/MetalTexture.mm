#include "EnginePCH.h"
#include "MetalTexture.h"

#include "GraphicsRenderer.h"
#include "MetalRenderingAPI.h"
#include "MetalUtility.h"

namespace Engine
{

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
        textureDescriptor.resourceOptions = Utilities::Metal::GetTextureResourceOptions(specifications.readWriteFlags, specifications.temporary);
        textureDescriptor.usage = Utilities::Metal::GetTextureUsage(specifications.readWriteFlags, false, false);
        // Sets the current texture type.
        // TODO: Allow this to change.
        textureDescriptor.textureType = MTLTextureType2D;
        
        MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
        m_texture = [renderingAPI.GetDevice() newTextureWithDescriptor:textureDescriptor];
        
        JKORN_ENGINE_ASSERT(m_texture != nullptr, "The texture must be valid.");
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
        JKORN_ENGINE_ASSERT(false, "Implement Bind Function in MetalTexture.")
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
