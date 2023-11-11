#pragma once

#include "Shader.h"

namespace Engine
{

    class MetalShader : public Shader
    {
    public:
        explicit MetalShader();
        explicit MetalShader(const BufferLayout& bufferLayout);
        ~MetalShader();
        
        bool IsValid() const override;
        void Bind() const override;
    };
}
