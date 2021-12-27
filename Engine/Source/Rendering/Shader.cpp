#include "EnginePCH.h"
#include "Shader.h"

#include "Rendering\GraphicsRenderer.h"
#include "Rendering\BufferLayout.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>

#pragma comment(lib, "d3dcompiler.lib")

namespace Engine
{

	static bool LoadShader(const WCHAR* fileName, const char* entryPoint, const char* model, ID3DBlob*& blob) 
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

#ifdef _WINDOWS
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

	Shader::Shader()
		: m_vertexShader(nullptr),
		m_pixelShader(nullptr),
		m_inputLayout(nullptr)
	{

	}

	Shader::~Shader()
	{
		if (m_pixelShader != nullptr)
		{
			m_pixelShader->Release();
		}
		if (m_vertexShader != nullptr)
		{
			m_vertexShader->Release();
		}
		if (m_inputLayout != nullptr)
		{
			m_inputLayout->Release();
		}
	}

	bool Shader::IsValid() const
	{
		return m_pixelShader != nullptr
			&& m_vertexShader != nullptr
			&& m_inputLayout != nullptr;
	}

	bool Shader::Load(const wchar_t* fileName, const BufferLayout& bufferLayout)
	{
		ID3DBlob* vertexShader = nullptr;
		if (LoadShader(fileName, "VS", "vs_4_0", vertexShader))
		{
			ID3DBlob* pixelShader = nullptr;
			if (LoadShader(fileName, "PS", "ps_4_0", pixelShader))
			{
				GraphicsRenderer* graphics = GraphicsRenderer::Get();
				DebugAssert(graphics != nullptr, "Graphics Renderer doesn't exist.");

				HRESULT result = graphics->m_device->CreateVertexShader(
					vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), nullptr, &m_vertexShader);
				DebugAssert(result == S_OK, "Failed to load vertex shader.");
				result = graphics->m_device->CreatePixelShader(
					pixelShader->GetBufferPointer(), pixelShader->GetBufferSize(), nullptr, &m_pixelShader);
				DebugAssert(result == S_OK, "Failed to load pixel shader.");
				if (m_vertexShader != nullptr
					&& m_pixelShader != nullptr)
				{
					result = graphics->m_device->CreateInputLayout(
						bufferLayout.GetD3D11InputElementDesc(), bufferLayout.GetNumElements(),
						vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), &m_inputLayout);
					DebugAssert(result == S_OK, "Failed to load input layout.");
					if (result == S_OK)
					{
						return true;
					}
				}
				return false;
			}
		}
		DebugAssert(false, "Failed to create Shader.");
		return false;
	}

	Shader* Shader::StaticLoad(const std::wstring& fileName, const BufferLayout& bufferLayout)
	{
		Shader* shader = new Shader();
		const wchar_t* cstrFileName = fileName.c_str();
		if (!shader->Load(cstrFileName, bufferLayout))
		{
			delete shader;
			return nullptr;
		}
		return shader;
	}

	void Shader::Bind() const
	{
		if (!IsValid())
		{
			return;
		}
		GraphicsRenderer* renderer = GraphicsRenderer::Get();
		renderer->m_deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
		renderer->m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);
		renderer->m_deviceContext->IASetInputLayout(m_inputLayout);
	}
}