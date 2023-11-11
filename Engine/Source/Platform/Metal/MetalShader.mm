#include "EnginePCH.h"
#include "MetalShader.h"

namespace Engine
{
    MetalShader::MetalShader()
        : Shader()
    {
        
    }
    
    MetalShader::MetalShader(const BufferLayout& bufferLayout)
        : Shader(bufferLayout)
    {
        
    }

    MetalShader::~MetalShader()
    {
        // TODO: Implementation
    }

    bool MetalShader::IsValid() const
    {
        // TODO: Implementation;
        JKORN_ENGINE_ASSERT(false, "Metal Shader is not implemented.");
        return false;
    }

    void MetalShader::Bind() const
    {
        
    }
}
