#pragma once

#include "MetalFwdDecl.h"
#include "Texture.h"

namespace Engine
{

    class MetalTexture : public Texture
    {
    public:
        MetalTexture();
        MetalTexture(uint32_t width, uint32_t height, const TextureSpecifications& specifications);
        MetalTexture(MTLTexturePtr texturePtr);
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
