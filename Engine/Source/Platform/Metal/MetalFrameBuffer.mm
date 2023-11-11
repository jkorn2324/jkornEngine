#include "EnginePCH.h"
#include "MetalFrameBuffer.h"

#include "GraphicsRenderer.h"
#include "MetalRenderingAPI.h"
#include "MetalUtility.h"
#include "MetalTexture.h"

#include <Metal/Metal.h>

namespace Engine
{

    namespace
    {
    
        template<typename TContext>
        void CreateViewTexture()
        {
            static_assert(false);
        }
    }
    

    MetalFrameBuffer::MetalFrameBuffer(const FrameBufferSpecification& specification)
        : FrameBuffer(specification), m_depthStencilState(nullptr), m_renderPassDescriptor(nullptr)
    {
        CreateBuffer();
    }
    
    MetalFrameBuffer::~MetalFrameBuffer()
    {
        // Deallocates render target view textures.
        {
            size_t renderTargetTextures = GetNumRenderTargets();
            for (size_t index = 0; index < renderTargetTextures; ++index)
            {
                MetalViewTexture& texture = m_renderTargetTextures[index];
                texture.ReleaseTexture();
            }
        }

        // Deallocates the depth textures.
        {
            m_depthTexture.ReleaseTexture();
        }
        
        if (m_depthStencilState)
        {
            [m_depthStencilState release];
            m_depthStencilState = nullptr;
        }
        
        if (m_renderPassDescriptor)
        {
            [m_renderPassDescriptor release];
            m_renderPassDescriptor = nullptr;
        }
    }

    void MetalFrameBuffer::CreateBuffer()
    {
        MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
        
        // Create a new render pass descriptor
        m_renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
        
        // Create Depth Stencil Buffer
        {
            Engine::FrameBufferAttachment<Engine::FrameBufferDepthStencilContext>& depthStencil = m_frameBufferSpecification.attachments.depthStencilAttachment;
            if (depthStencil.format.format != DepthFormat_None)
            {
                // Creates the depth stencil state.
                {
                    m_depthStencilState = Utility::Metal::CreateDepthStencilState(MTLCompareFunctionLess, renderingAPI.GetDevice());
                }
                
                // Update depth attachment for a texture.
                m_renderPassDescriptor.depthAttachment.loadAction = MTLLoadActionClear;
                m_renderPassDescriptor.depthAttachment.storeAction = MTLStoreActionStore;
                
                // Create the frame buffer texture.
                MTLTextureDescriptor* desc = [[MTLTextureDescriptor alloc] init];
                desc.width = (NSInteger)m_frameBufferSpecification.width;
                desc.height = (NSInteger)m_frameBufferSpecification.height;
                desc.mipmapLevelCount = 1;
                desc.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
                desc.resourceOptions = depthStencil.format.readTexture ? MTLResourceStorageModeShared : MTLResourceStorageModePrivate;
                
                // Ensures that the depth stencil format is updated accordingly.
                if (depthStencil.format.readTexture)
                {
                    depthStencil.format.format = DepthFormat_D32Float;
                }

                desc.pixelFormat = Utility::Metal::ToMTLPixelFormat(Graphics::ToGraphicsFormat(depthStencil.format.format));
                // Sets the depth texture.
                m_renderPassDescriptor.depthAttachment.texture = m_renderPassDescriptor.stencilAttachment.texture = [renderingAPI.GetDevice() newTextureWithDescriptor: desc];
                m_depthTexture.texture = new MetalTexture(m_renderPassDescriptor.depthAttachment.texture);
            }
        }
        
        // Create the Render Texture
        uint32_t numRenderTargets = (uint32_t)GetNumRenderTargets();
        if (numRenderTargets > 0)
        {
            for (uint32_t slot = 0; slot < numRenderTargets; ++slot)
            {
                // Create a new texture.
                MTLRenderPassColorAttachmentDescriptor* colorAttachment = m_renderPassDescriptor.colorAttachments[slot];
                
                FrameBufferAttachment<ColorFormat>& attachment = m_frameBufferSpecification.attachments.colorAttachments[slot];
                
                MTLTextureDescriptor* descriptor = [[MTLTextureDescriptor alloc] init];
                descriptor.width = (NSInteger)m_frameBufferSpecification.width;
                descriptor.height = (NSInteger)m_frameBufferSpecification.height;
                descriptor.mipmapLevelCount = 1;
                descriptor.pixelFormat =  Utility::Metal::ToMTLPixelFormat(Graphics::ToGraphicsFormat(attachment.format));
                descriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
                descriptor.resourceOptions = MTLResourceStorageModePrivate;
                
                colorAttachment.texture = [renderingAPI.GetDevice() newTextureWithDescriptor:descriptor];
                // TODO: Deallocate texture.
                m_renderTargetTextures[slot].texture = new MetalTexture(colorAttachment.texture);
            }
        }
    }

    void MetalFrameBuffer::Bind() const
    {
        MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
        renderingAPI.OnBeginRender(m_renderPassDescriptor);
    }

    void MetalFrameBuffer::UnBind() const
    {
        MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
        renderingAPI.OnEndRender();
    }

    void MetalFrameBuffer::ReGenerateTextures()
    {
        JKORN_ENGINE_ASSERT(false, "Metal Frame Buffer hasn't regenerated the textures.");
    }

    void MetalFrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        FrameBuffer::Resize(width, height);
        JKORN_ENGINE_ASSERT(false, "Implement resize for frame buffers.");
    }

    Texture* MetalFrameBuffer::GetDepthTexture() const
    {
        return m_depthTexture.texture;
    }

    Texture* MetalFrameBuffer::GetRenderTargetTexture(uint32_t index) const
    {
        if (index >= 0 && index < MaxRenderTargets)
        {
            return m_renderTargetTextures[index].texture;
        }
        return nullptr;
    }
}
