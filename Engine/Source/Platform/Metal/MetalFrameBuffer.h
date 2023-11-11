#pragma once

#include "MetalFwdDecl.h"
#include "FrameBuffer.h"

namespace Engine
{

    class Texture;
    class MetalTexture;

    class MetalFrameBuffer : public FrameBuffer
    {
    private:
        /**
         * The metal view texture structure.
         */
        struct MetalViewTexture
        {
            MetalTexture* texture = nullptr;
            
            void ReleaseTexture()
            {
                // TODO: Find a better way to deallocate textures.
                if (texture)
                {
                    delete texture;
                    texture = nullptr;
                }
            }
        };
        
    public:
        static const size_t MaxRenderTargets = 8;
        
        explicit MetalFrameBuffer(const FrameBufferSpecification& specification);
        ~MetalFrameBuffer();
        
        void Bind() const override;
        void UnBind() const override;
        
        void Resize(uint32_t width, uint32_t height) override;
        void ReGenerateTextures() override;
        
        Texture* GetDepthTexture() const override;
        Texture* GetRenderTargetTexture(uint32_t index) const override;
        
    private:
        void CreateBuffer();
        
    private:
        MTLDepthStencilStatePtr m_depthStencilState;
        MTLRenderPassDescriptorPtr m_renderPassDescriptor;
        MetalViewTexture m_renderTargetTextures[MaxRenderTargets];
        MetalViewTexture m_depthTexture;
    };
}
