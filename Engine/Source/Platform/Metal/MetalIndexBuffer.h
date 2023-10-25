#pragma once

#include "IndexBuffer.h"
#include "MetalFwdDecl.h"

namespace Engine
{

    class MetalIndexBuffer : public IndexBuffer
    {
    public:
        explicit MetalIndexBuffer(const void* buffer, uint32_t numIndices, uint32_t stride);
        
        ~MetalIndexBuffer();

        bool IsValid() const override;

        void SetData(const void* buffer,
            uint32_t numIndices, uint32_t stride) override;
        
        void Bind() const override;
        
    private:
        MTLBufferPtr m_bufferPtr;
    };
}
