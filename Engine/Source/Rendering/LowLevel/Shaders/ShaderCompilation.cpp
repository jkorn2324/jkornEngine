#include "EnginePCH.h"
#include "ShaderCompilation.h"

#include <string_view>
#include <string>
#include <sstream>

// Include the directx shader compiler api
#include <dxcapi.h>
// Include the spirv-cross metal shading language api.
#include <spirv_msl.hpp>

#if defined(GRAPHICS_API_DIRECTX11) && defined(PLATFORM_WINDOWS)
#include <d3d11.h>
#endif

namespace Engine
{
#if defined(_WIN32) && !defined(USE_ATL_SUPPORT)
	/**
	 * @brief The shader pointer.
	 */
	template<typename T>
	struct CShaderPtr
	{
	public:
		CShaderPtr()
			: m_ptr(nullptr) { }

		CShaderPtr(T* ptr)
			: m_ptr(ptr) { }

		~CShaderPtr()
		{
			if (m_ptr)
			{
				m_ptr->Release();
				m_ptr = nullptr;
			}
		}

		T** operator&()
		{
			return &m_ptr;
		}

		const T** operator&() const
		{
			return &m_ptr;
		}

		T* operator->()
		{
			return m_ptr;
		}

		T* operator->() const
		{
			return m_ptr;
		}

		friend bool operator==(const CShaderPtr<T>& a, const CShaderPtr<T>& b)
		{
			return a.m_ptr != b.m_ptr;
		}

		friend bool operator==(const CShaderPtr<T>& a, T* b)
		{
			return a.m_ptr == b;
		}

		friend bool operator!=(const CShaderPtr<T>& a, const CShaderPtr<T>& b)
		{
			return a.m_ptr != b.m_ptr;
		}

		friend bool operator!=(const CShaderPtr<T>& a, T* b)
		{
			return a.m_ptr != b;
		}

		operator bool() const
		{
			return m_ptr != nullptr;
		}

	private:
		T* m_ptr = nullptr;
	};

#else
	template<typename T>
	using CShaderPtr = CComPtr<T>;
#endif

	// ----------------------------------------------- BEGIN SHADER FILE COMPILATION CONTEXT DEFINES -------------------------------------------


	const std::wstring ShaderStageCompilationContext::ToDirectXTargetProfileString() const
	{
		std::wstringstream stringStream;
		switch (shaderStage)
		{
		case ShaderStageType_PixelShader:
		{
			stringStream << L"ps_";
			break;
		}
		case ShaderStageType_VertexShader:
		{
			stringStream << L"vs_";
			break;
		}
		case ShaderStageType_ComputeShader:
		{
			stringStream << L"cs_";
			break;
		}
		}
		stringStream << versionMajor << L'_' << versionMinor;
		return stringStream.str();
	}

	// ----------------------------------------------- END SHADER FILE COMPILATION CONTEXT DEFINES -------------------------------------------

	// ----------------------------------------------- BEGIN HLSL SHADER FILE COMPILATION -------------------------------------------


	template<>
	struct SpirvOutputBlob<ShaderLanguageType_Hlsl>
	{
		static constexpr bool IsValid = true;

	public:
		SpirvOutputBlob()
			: m_blob(nullptr)
		{

		}

		SpirvOutputBlob(const CShaderPtr<IDxcBlob>& blob)
			: m_blob(blob)
		{

		}

		void* GetBufferPointer() const { return m_blob->GetBufferPointer(); }

		size_t GetBufferSize() const { return m_blob->GetBufferSize(); }

	private:
		CShaderPtr<IDxcBlob> m_blob = nullptr;
	};

	namespace
	{

		CShaderPtr<IDxcBlob> CompileHlslBlobToSpirv_Dxc(CShaderPtr<IDxcBlobEncoding>& shaderBlob, UINT blobEncoding,
			std::vector<LPCWSTR>& arguments)
		{
			JKORN_ENGINE_ASSERT(shaderBlob != nullptr, "The shader blob must exist.");

			CShaderPtr<IDxcCompiler3> compiler;
			// Creates a dxc compiler.
			{
				HRESULT hresult = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
				if (!SUCCEEDED(hresult))
				{
					JKORN_ENGINE_ASSERT(false, "Failed to compile hlsl to spirv - failed to create compiler.");
					return {};
				}
			}

			DxcBuffer dxcBuffer;
			dxcBuffer.Ptr = shaderBlob->GetBufferPointer();
			dxcBuffer.Size = shaderBlob->GetBufferSize();
			dxcBuffer.Encoding = blobEncoding;

			CShaderPtr<IDxcResult> compileResult;
			HRESULT hr = compiler->Compile(&dxcBuffer, (LPCWSTR*)arguments.data(),
				(UINT32)arguments.size(), nullptr, IID_PPV_ARGS(&compileResult));
			if (SUCCEEDED(hr))
			{
				compileResult->GetStatus(&hr);
			}

			if (FAILED(hr) && compileResult)
			{
#if DEBUG
				CShaderPtr<IDxcBlobEncoding> errorBlob;
				hr = compileResult->GetErrorBuffer(&errorBlob);
				if (SUCCEEDED(hr) && errorBlob)
				{
					// TODO: Output the error.
				}
#endif
				JKORN_ENGINE_ASSERT(false, "Failed to generate spirv output for the shader utility.");
				return nullptr;
			}

			CShaderPtr<IDxcBlob> spirvOutput;
			compileResult->GetResult(&spirvOutput);
			return spirvOutput;
		}
	}


	template<>
	SpirvOutputBlob<ShaderLanguageType_Hlsl> ShaderCompilation::CompileSourceToSpirv<ShaderLanguageType_Hlsl>(const std::string& shaderSource,
		wchar_t* entryPoint, const ShaderStageCompilationContext& stage)
	{
		CShaderPtr<IDxcUtils> dxcUtils;
		{
			HRESULT result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
			if (!SUCCEEDED(result))
			{
				JKORN_ENGINE_ASSERT(false, "Failed to compile hlsl to spir-v.");
				return {};
			}
		}

		CShaderPtr<IDxcBlobEncoding> shaderBlob;
		{
			HRESULT result = dxcUtils->CreateBlob(&shaderSource[0], shaderSource.size(), DXC_CP_ACP, &shaderBlob);
			if (!SUCCEEDED(result))
			{
				JKORN_ENGINE_ASSERT(false, "Failed to compile hlsl to spir-v.");
				return {};
			}
		}

		std::wstring targetProfile = stage.ToDirectXTargetProfileString();
		std::vector<LPCWSTR> arguments =
		{
			L"-E", entryPoint,
			L"-T", targetProfile.c_str(),
			L"-spirv"
		};

		CShaderPtr<IDxcBlob> compiledBlob = CompileHlslBlobToSpirv_Dxc(
			shaderBlob, DXC_CP_WIDE, arguments);
		if (!compiledBlob)
		{
			// Failed to compile the blob, print error.
			return {};
		}
		return SpirvOutputBlob<ShaderLanguageType_Hlsl>(compiledBlob);
	}

	template<>
	SpirvOutputBlob<ShaderLanguageType_Hlsl> ShaderCompilation::CompileShaderFileToSpirv<ShaderLanguageType_Hlsl>(const std::filesystem::path& hlslShaderFilePath,
		wchar_t* entryPoint, const ShaderStageCompilationContext& stage)
	{
		CShaderPtr<IDxcUtils> dxcUtils;
		{
			// Create an instance of a utility.
			HRESULT result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
			if (!SUCCEEDED(result))
			{
				JKORN_ENGINE_ASSERT(false, "Failed to compile hlsl to spir-v");
				return {};
			}
		}

		// The shader blob
		CShaderPtr<IDxcBlobEncoding> shaderBlob;
		{
			std::wstring wStrFilePath = hlslShaderFilePath.wstring();
			LPWSTR wstr = wStrFilePath.data();
			HRESULT result = dxcUtils->LoadFile(wstr, nullptr, &shaderBlob);
			if (!SUCCEEDED(result))
			{
				JKORN_ENGINE_ASSERT(false, "Failed to compile hlsl to spir-v.");
				return {};
			}
		}

		// The arguments for the shader.
		std::wstring targetProfile = stage.ToDirectXTargetProfileString();
		std::vector<LPCWSTR> arguments =
		{
			L"-E", entryPoint,
			L"-T", targetProfile.c_str(),
			L"-spirv"
		};

		CShaderPtr<IDxcBlob> compiledBlob = CompileHlslBlobToSpirv_Dxc(
			shaderBlob, DXC_CP_WIDE, arguments);
		if (!compiledBlob)
		{
			// Failed to compile the blob, print error.
			return {};
		}
		// Saves the compiled spirv output blob.
		return SpirvOutputBlob<ShaderLanguageType_Hlsl>(compiledBlob);
	}

	// ----------------------------------------------- END HLSL SHADER FILE COMPILATION -------------------------------------------

	// ----------------------------------------------- BEGIN MSL SHADER FILE COMPILATION -------------------------------------------

	template<>
	bool ShaderCompilation::DecompileSpirvVectorBlobToSource<ShaderLanguageType_Msl>(
		const std::vector<uint32_t>& sourceBlob, std::string& dst)
	{
		// Compiles to the metal shading language using spirv-cross.
		spirv_cross::CompilerMSL mslCompiler(std::move(sourceBlob));
		dst = mslCompiler.compile();
		return true;
	}

	// ----------------------------------------------- END MSL SHADER FILE COMPILATION -------------------------------------------


	// ----------------------------------------------- BEGIN NORMAL DEFINES  -------------------------------------------

	bool ShaderCompilation::CompileSourceToSpirv(
		const std::string& shaderSourceCode, wchar_t* entryPoint,
		const ShaderStageCompilationContext& stageCompilationContext,
		ShaderLanguageType shaderLanguageType, std::vector<uint32_t>& dst)
	{
		// Compiles the shader source code to spirv.
		switch (shaderLanguageType)
		{
		case ShaderLanguageType_Hlsl:
		{
			SpirvOutputBlob<ShaderLanguageType_Hlsl> outputBlob
				= CompileSourceToSpirv<ShaderLanguageType_Hlsl>(shaderSourceCode, entryPoint, stageCompilationContext);
			return PopulateVectorWithSpirvOutput(outputBlob, dst);
		}
		}
		return false;
	}

	// ----------------------------------------------- END NORMAL DEFINES -------------------------------------------
}
