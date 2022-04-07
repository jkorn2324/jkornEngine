#include "EnginePCH.h"
#include "DirectX11ComputeShader.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"
#include "ConstantBuffer.h"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace Engine
{

	static bool CompileShader(const WCHAR* fileName, const char* entryPoint, const char* model, ID3DBlob*& blob)
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
		dwShaderFlags |= D3DCOMPILE_DEBUG;
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3DBlob* errorBlob = nullptr;
		HRESULT result = D3DCompileFromFile(fileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint, model, dwShaderFlags, 0, &blob, &errorBlob);
		if (result != S_OK)
		{
			if (errorBlob != nullptr)
			{
				static wchar_t buffer[4096];
				_snwprintf_s(buffer, 4096, _TRUNCATE,
					L"%hs", (char*)errorBlob->GetBufferPointer());
				OutputDebugString(buffer);

#ifdef PLATFORM_WINDOWS
				MessageBox(nullptr, buffer, L"Error", MB_OK);
#endif
				errorBlob->Release();
			}
			return false;
		}
		// Releases the error blob.
		if (errorBlob != nullptr)
		{
			errorBlob->Release();
		}
		return true;
	}

	DirectX11ComputeShader::DirectX11ComputeShader()
		: m_computeShader(nullptr), m_constantBuffer(nullptr)
	{
	}
	
	DirectX11ComputeShader::~DirectX11ComputeShader()
	{
		if (m_computeShader != nullptr)
		{
			m_computeShader->Release();
		}
		delete m_constantBuffer;
	}

	bool DirectX11ComputeShader::IsValid() const
	{
		return m_computeShader != nullptr;
	}

	void DirectX11ComputeShader::Dispatch(uint32_t x, uint32_t y, uint32_t z)
	{
		if (!IsValid()) return;

		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();
		renderingAPI.m_deviceContext->CSSetShader(m_computeShader, nullptr, 0);

		// TODO: Set Shader Resources, Set RWStructuredBuffers, RWTexture2D, etc...
		if (m_constantBuffer != nullptr)
		{
			// Constant Buffer Slot for a compute shader should always be zero.
			static int s_constantBufferFlags = ConstantBufferFlags::COMPUTE_SHADER;
			m_constantBuffer->Bind(0, s_constantBufferFlags);
		}

		renderingAPI.m_deviceContext->Dispatch(x, y, z);
	}

	void DirectX11ComputeShader::InternalSetConstants(void* bufferData, size_t stride)
	{
		if (m_constantBuffer == nullptr)
		{
			m_constantBuffer = ConstantBuffer::Create(bufferData, stride);
			return;
		}
		m_constantBuffer->SetData(bufferData, stride);
	}

	void DirectX11ComputeShader::SetInt(const std::string& name, int value)
	{
		// TODO: Implementation
	}

	void DirectX11ComputeShader::SetFloat(const std::string& name, float value)
	{
		// TODO: Implementation
	}

	void DirectX11ComputeShader::SetFloat2(const std::string& name, const MathLib::Vector2& vec)
	{
		// TODO: Implementation
	}

	void DirectX11ComputeShader::SetFloat3(const std::string& name, const MathLib::Vector3& vec)
	{
		// TODO: Implementation
	}

	void DirectX11ComputeShader::SetFloat4(const std::string& name, const MathLib::Vector4& vec)
	{
		// TODO: Implementation
	}

	void DirectX11ComputeShader::SetMatrix3x3(const std::string& name, const MathLib::Matrix3x3& mat)
	{
		// TODO: Implementation
	}

	void DirectX11ComputeShader::SetMatrix4x4(const std::string& name, const MathLib::Matrix4x4& mat)
	{
		// TODO: Implementation
	}

	void DirectX11ComputeShader::SetTexture(const std::string& name, const Texture& texture)
	{
		// TODO: Implementation
	}

	bool DirectX11ComputeShader::Load(const wchar_t* fileName)
	{
		ID3DBlob* shaderBlob = nullptr;
		if (CompileShader(fileName, "CSMain", "cs_5_0", shaderBlob))
		{
			DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();
			HRESULT result = renderingAPI.m_device->CreateComputeShader(shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(), nullptr, &m_computeShader);
			bool success = S_OK(result);
			DebugAssert(success == true, "Shader Failed to compile.");

			if (!success)
			{
				shaderBlob->Release();
				return false;
			}
			return true;
		}
		return false;
	}
}