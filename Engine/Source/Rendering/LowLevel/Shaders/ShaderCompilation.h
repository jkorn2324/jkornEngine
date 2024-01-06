#pragma once

#include "ShaderTypes.h"
#include "Memory.h"

#include <string>
#include <filesystem>

namespace Engine
{
	/**
	 * The shader stage compilation context.
	 * The default shader version major = 6 & version minor = 0.
	 */
	struct ShaderStageCompilationContext
	{
		ShaderStageType shaderStage;
		uint8_t versionMajor = 6;
		uint8_t versionMinor = 0;

		/**
		 * Gets the dxc shader target profile.
		 */
		const std::wstring ToDirectXTargetProfileString() const;
	};

	/**
	 * The spir-v output blob.
	 */
	template<ShaderLanguageType TLanguageType>
	struct SpirvOutputBlob
	{
		static constexpr bool IsValid = false;

		inline void* GetBufferPointer() const { return nullptr; }

		inline size_t GetBufferSize() const { return 0; }
	};


	namespace ShaderCompilation
	{
		/**
		 * Populates a vector of uint32_t's with the spirv blob output.
		 */
		template<ShaderLanguageType TSourceLanguageType>
		bool PopulateVectorWithSpirvOutput(const SpirvOutputBlob<TSourceLanguageType>& blob, std::vector<uint32_t>& spirvBlobVector)
		{
			static_assert(SpirvOutputBlob<TSourceLanguageType>::IsValid, "Failed to populate spirv vector.");

			// Gets the buffer pointer.
			uint32_t* bufferPointer = reinterpret_cast<uint32_t*>(blob.GetBufferPointer());
			if (bufferPointer == nullptr)
			{
				return false;
			}
			// Modifies the buffer pointer size to fit.
			size_t bufferSizePointer = blob.GetBufferSize() / sizeof(uint32_t);
			spirvBlobVector.resize(bufferSizePointer);
			// Copies the memory over to the spirv vector.
			Memory::Memcpy(&spirvBlobVector[0], bufferPointer, bufferSizePointer);
			return true;
		}

		/**
		 * Compiles a source shader code file of a source language type to a source code file.
		 */
		template<ShaderLanguageType TSourceLanguageType>
		SpirvOutputBlob<TSourceLanguageType> CompileSourceToSpirv(const std::string& shaderSourceCode,
			wchar_t* entryPoint,
			const ShaderStageCompilationContext& compilationContext)
		{
			static_assert(false);
			return {};
		}

		/**
		 * Compiles a source file at a path to spirv.
		 */
		template<ShaderLanguageType TSourceLanguageType>
		SpirvOutputBlob<TSourceLanguageType> CompileShaderFileToSpirv(const std::filesystem::path& shaderFilePath, wchar_t* entryPoint,
			const ShaderStageCompilationContext& shaderStageCompilationContext)
		{
			static_assert(false);
			return {};
		}

		/**
		 * Compiles the spir-v vector blob to the destination.
		 */
		template<ShaderLanguageType TDestinationLanguageType>
		bool DecompileSpirvVectorBlobToSource(const std::vector<uint32_t>& sourceBlob, std::string& dst)
		{
			static_assert(false);
			return false;
		}


		/**
		 * Compiles the spir-v source vector blob to the destination.
		 */
		bool CompileSourceToSpirv(const std::string& shaderSourceCode, wchar_t* entryPoint,
			const ShaderStageCompilationContext& compilationContext, ShaderLanguageType shaderLanguageType,
			std::vector<uint32_t>& dst);
	}
}
