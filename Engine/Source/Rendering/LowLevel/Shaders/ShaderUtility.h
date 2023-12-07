#pragma once

#include <filesystem>

namespace Engine
{
    /**
     * The shader file type.
     */
    enum ShaderLanguageType
    {
        // Invalid shader language.
        ShaderLanguageType_None,
        ShaderLanguageType_Invalid = ShaderLanguageType_None,
        
        // Hlsl shading language type
        ShaderLanguageType_Hlsl,
        // Metal Shading language type
        ShaderLanguageType_Msl
    };

    /**
     * The shader stage type.
     */
    enum ShaderStageType
    {
        // The vertex shader stage.
        ShaderStageType_VertexShader,
        // The pixel shader stage.
        ShaderStageType_PixelShader,
        // The compute shader stage.
        ShaderStageType_ComputeShader
    };

    namespace ShaderUtility
    {
        
        /**
         * @brief Get the Shader Language Type From the file extension.
         * 
         * @param ext - the file extension.
         * @return ShaderLanguageType 
         */
        constexpr ShaderLanguageType GetShaderLanguageTypeFromExt(const wchar_t* ext);
        
        bool CompileHlslShaderFileToSpirv(const std::filesystem::path& hlslShaderFilePath, wchar_t* entryPoint, ShaderStageType stageType);
    }
}
