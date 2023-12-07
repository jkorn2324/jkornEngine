#include "EnginePCH.h"
#include "ShaderUtility.h"

#include <string_view>

// Include the directx shader compiler api
#include <dxcapi.h>

namespace Engine
{

namespace
{
    bool CompileHlslBlobToSpirv_Dxc(CComPtr<IDxcBlobEncoding>& shaderBlob, UINT blobEncoding, std::vector<LPCWSTR>& arguments, CComPtr<IDxcCompiler3>& compiler)
    {
        JKORN_ENGINE_ASSERT(shaderBlob != nullptr, "The shader blob must exist.");
        JKORN_ENGINE_ASSERT(compiler, "The compiler must exist.");
        
        DxcBuffer dxcBuffer;
        dxcBuffer.Ptr = shaderBlob->GetBufferPointer();
        dxcBuffer.Size = shaderBlob->GetBufferSize();
        dxcBuffer.Encoding = blobEncoding;
        
        CComPtr<IDxcResult> compileResult;
        HRESULT hr = compiler->Compile(&dxcBuffer, (LPCWSTR*)arguments.data(), (UINT32)arguments.size(), nullptr, IID_PPV_ARGS(&compileResult));
        if (SUCCEEDED(hr))
        {
            CComPtr<IDxcBlob> dxcOutput;
            compileResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&dxcOutput), nullptr);
            // TODO: GET SPIR-V OUTPUT & DUMP INTO SHADER
            return true;
        }
        
        CComPtr<IDxcBlobUtf8> errors;
        compileResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
        if (errors && errors->GetStringLength() > 0)
        {
            // The error blob.
            return false;
        }
        return false;
    }
}

constexpr ShaderLanguageType ShaderUtility::GetShaderLanguageTypeFromExt(const wchar_t* filePath)
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

bool ShaderUtility::CompileHlslShaderFileToSpirv(const std::filesystem::path &hlslShaderFilePath, wchar_t* entryPoint, ShaderStageType shaderStageType)
{
    CComPtr<IDxcCompiler3> compiler;
    // Creates a dxc compiler.
    {
        HRESULT hresult = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
        if (!SUCCEEDED(hresult))
        {
            return false;
        }
    }
    
    CComPtr<IDxcUtils> dxcUtils;
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
    CComPtr<IDxcBlobEncoding> shaderBlob;
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
