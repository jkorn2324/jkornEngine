#pragma once

#include "ShaderTypes.h"

#include <filesystem>

namespace Engine
{

	namespace ShaderUtility
	{

		bool CompileHlslShaderFileToSpirv(const std::filesystem::path& hlslShaderFilePath, wchar_t* entryPoint, ShaderStageType stageType);
	}
}
