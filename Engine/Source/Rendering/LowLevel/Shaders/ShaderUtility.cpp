#include "EnginePCH.h"
#include "ShaderUtility.h"

#include <string_view>

// Include the directx shader compiler api
#include <dxcapi.h>

namespace Engine
{

#if defined(_WIN32) && !defined(_ATL_VER)
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

	namespace
	{
		bool CompileHlslBlobToSpirv_Dxc(CShaderPtr<IDxcBlobEncoding>& shaderBlob, UINT blobEncoding, std::vector<LPCWSTR>& arguments, CShaderPtr<IDxcCompiler3>& compiler)
		{
			JKORN_ENGINE_ASSERT(shaderBlob != nullptr, "The shader blob must exist.");
			JKORN_ENGINE_ASSERT(compiler != nullptr, "The compiler must exist.");

			DxcBuffer dxcBuffer;
			dxcBuffer.Ptr = shaderBlob->GetBufferPointer();
			dxcBuffer.Size = shaderBlob->GetBufferSize();
			dxcBuffer.Encoding = blobEncoding;

			CShaderPtr<IDxcResult> compileResult;
			HRESULT hr = compiler->Compile(&dxcBuffer, (LPCWSTR*)arguments.data(), (UINT32)arguments.size(), nullptr, IID_PPV_ARGS(&compileResult));
			if (SUCCEEDED(hr))
			{
				CShaderPtr<IDxcBlob> dxcOutput;
				compileResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&dxcOutput), nullptr);
				// TODO: GET SPIR-V OUTPUT & DUMP INTO SHADER
				return true;
			}

			CShaderPtr<IDxcBlobUtf8> errors;
			compileResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
			if (errors && errors->GetStringLength() > 0)
			{
				// The error blob.
				return false;
			}
			return false;
		}
	}

	bool ShaderUtility::CompileHlslShaderFileToSpirv(const std::filesystem::path& hlslShaderFilePath, wchar_t* entryPoint, ShaderStageType shaderStageType)
	{
		CShaderPtr<IDxcCompiler3> compiler;
		// Creates a dxc compiler.
		{
			HRESULT hresult = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
			if (!SUCCEEDED(hresult))
			{
				return false;
			}
		}

		CShaderPtr<IDxcUtils> dxcUtils;
		{
			// Create an instance of a utility.
			HRESULT result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
			if (!SUCCEEDED(result))
			{
				JKORN_ENGINE_ASSERT(false, "Failed to compile hlsl to spir-v");
				return false;
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
				return false;
			}
		}

		std::vector<LPCWSTR> arguments;
		{
			// Entry point.
			arguments.push_back(L"-E");
			arguments.push_back(entryPoint);

			// Output to Spir-V
			arguments.push_back(L"-spirv");

			// TODO: Shader Stage
		}

		return CompileHlslBlobToSpirv_Dxc(shaderBlob, DXC_CP_WIDE, arguments, compiler);
	}

}
