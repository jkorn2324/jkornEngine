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
        
        /**
         * Gets the index type.
         */
        template<typename T>
        T GetIndexType() const 
        {
            uint8_t indexType = GetIndexTypeInternal();
            return *reinterpret_cast<T*>(&indexType);
        }
        
        MTLBufferPtr GetMTLBufferPtr() const { return m_bufferPtr; }
        
    private:
        uint8_t GetIndexTypeInternal() const;
        
    private:
        MTLBufferPtr m_bufferPtr;
    };
}
