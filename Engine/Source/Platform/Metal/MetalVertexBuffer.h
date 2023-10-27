#pragma once

#include "VertexBuffer.h"
#include "MetalFwdDecl.h"

namespace Engine
{

    /**
     * @brief Represents the vertex buffer as defined by metal.
     */
    class MetalVertexBuffer : public VertexBuffer
    {
    public:
        explicit MetalVertexBuffer(const void* buffer,
            uint32_t numVertices, uint32_t stride);
        ~MetalVertexBuffer();

        bool IsValid() const override;

        void SetData(const void* buffer,
            uint32_t numVertices, uint32_t stride) override;
        void Bind() const override;
        
    private:
        MTLBufferPtr m_bufferPtr;
    };
}
