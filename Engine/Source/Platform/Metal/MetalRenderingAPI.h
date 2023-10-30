#pragma once

#include "RenderingAPI.h"
#include "Vector.h"

#include "MetalFwdDecl.h"

namespace Engine
{
    
/**
 * The metal rendering api definition.
 */
class MetalRenderingAPI : public RenderingAPI
{
public:
    explicit MetalRenderingAPI();
    ~MetalRenderingAPI();
    
    bool Initialize(Window* windowPtr) override;
    void SetViewport(float x, float y, float width, float height) override;
    void SetResolution(uint32_t width, uint32_t height) override;
    void SetClearColor(const MathLib::Vector4& vector4) override;
    
    void OnBeginRender();
    void OnEndRender();
    
    void Present() override;
    
    void Draw(VertexArray* vertexArray) override;
    void Draw(VertexBuffer* vertexBuffer,
        IndexBuffer* indexBuffer = nullptr) override;
    
    void SetWireframe(bool wireframeMode) override;
    bool IsWireframe() const override;

    void ClearTexture(uint32_t slot) override;

    uint32_t GetWidth() const override;
    uint32_t GetHeight() const override;
    
    MTLDevicePtr GetDevice() const { return m_device; }
    MTLRenderPassDescriptorPtr GetRenderPassDescriptor() const { return m_renderDescriptor; }
    MTLCommandQueuePtr GetCommandQueue() const { return m_commandQueue; }
        
private:
    MTLDevicePtr m_device;
    CAMetalLayerPtr m_swapChain;
    MTLCommandQueuePtr m_commandQueue;
    
    MTLRenderPassDescriptorPtr m_renderDescriptor;
    MTLCommandBufferPtr m_activeCommandBuf;
    MTLRenderCommandEncoderPtr m_renderEncoder;
    
    MathLib::Vector4 m_clearColor;
    uint32_t m_width;
    uint32_t m_height;
    
    friend class MetalVertexBuffer;
    friend class MetalIndexBuffer;
};

}