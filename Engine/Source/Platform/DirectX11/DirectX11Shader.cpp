#include "EnginePCH.h"
#include "DirectX11Shader.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"
#include "BufferLayout.h"

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

	DirectX11Shader::DirectX11Shader()
		: Shader(),
		m_vertexShader(nullptr),
		m_pixelShader(nullptr),
		m_inputLayout(nullptr)
	{
	}

	DirectX11Shader::~DirectX11Shader()
	{
		if (m_vertexShader != nullptr)
		{
			m_vertexShader->Release();
		}
		if (m_pixelShader != nullptr)
		{
			m_pixelShader->Release();
		}
		if (m_inputLayout != nullptr)
		{
			m_inputLayout->Release();
		}
	}

	bool DirectX11Shader::IsValid() const
	{
		return m_pixelShader != nullptr
			&& m_vertexShader != nullptr
			&& m_inputLayout != nullptr;
	}
	
	void DirectX11Shader::Bind() const
	{
		if (!IsValid())
		{
			return;
		}
		GraphicsRenderer* renderer = GraphicsRenderer::Get();
		DirectX11RenderingAPI* renderingAPI = dynamic_cast<DirectX11RenderingAPI*>(
			renderer->GetRenderingAPI());
		renderingAPI->m_deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
		renderingAPI->m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);
		renderingAPI->m_deviceContext->IASetInputLayout(m_inputLayout);
	}
	
	bool DirectX11Shader::Load(const wchar_t* fileName, const BufferLayout& bufferLayout)
	{
		ID3DBlob* vertexShader = nullptr;
		if (LoadShader(fileName, "VS", "vs_4_0", vertexShader))
		{
			ID3DBlob* pixelShader = nullptr;
			if (LoadShader(fileName, "PS", "ps_4_0", pixelShader))
			{
				GraphicsRenderer* graphics = GraphicsRenderer::Get();
				DebugAssert(graphics != nullptr, "Graphics Renderer doesn't exist.");
				DirectX11RenderingAPI* renderingAPI = dynamic_cast<DirectX11RenderingAPI*>(
					graphics->GetRenderingAPI());

				HRESULT result = renderingAPI->m_device->CreateVertexShader(
					vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), nullptr, &m_vertexShader);
				DebugAssert(result == S_OK, "Failed to load vertex shader.");
				result = renderingAPI->m_device->CreatePixelShader(
					pixelShader->GetBufferPointer(), pixelShader->GetBufferSize(), nullptr, &m_pixelShader);
				DebugAssert(result == S_OK, "Failed to load pixel shader.");
				if (m_vertexShader != nullptr
					&& m_pixelShader != nullptr)
				{
					result = renderingAPI->m_device->CreateInputLayout(
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
}