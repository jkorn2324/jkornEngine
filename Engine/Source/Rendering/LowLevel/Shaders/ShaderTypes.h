#pragma once

#include <string>

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

	namespace ShaderTypesUtilities
	{
		/**
		 * @brief Get the Shader Language Type From the file extension.
		 *
		 * @param ext - the file extension.
		 * @return ShaderLanguageType
		 */
		constexpr ShaderLanguageType GetShaderLanguageTypeFromExt(const wchar_t* filePath)
		{
			std::wstring_view view(filePath);
			if (view == L"hlsl")
			{
				return ShaderLanguageType_Hlsl;
			}
			if (view == L"msl")
			{
				return ShaderLanguageType_Msl;
			}
			return ShaderLanguageType_None;
		}
	}
}