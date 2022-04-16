#include "EnginePCH.h"
#include "DirectX11Shader.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"
#include "DirectX11BufferLayoutParser.h"
#include "DirectX11Utils.h"

#include <d3dcompiler.h>
#include <fstream>

namespace Engine
{

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
		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)
			GraphicsRenderer::GetRenderingAPI();
		renderingAPI.m_deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
		renderingAPI.m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);
		renderingAPI.m_deviceContext->IASetInputLayout(m_inputLayout);
	}
	
	bool DirectX11Shader::Load(const wchar_t* fileName, const BufferLayout& bufferLayout)
	{
		ID3DBlob* vertexShader = nullptr;
		if (DirectX11Utils::CompileShader(fileName, "VS", "vs_4_0", vertexShader))
		{
			ID3DBlob* pixelShader = nullptr;
			if (DirectX11Utils::CompileShader(fileName, "PS", "ps_4_0", pixelShader))
			{
				DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)
					GraphicsRenderer::GetRenderingAPI();

				HRESULT result = renderingAPI.m_device->CreateVertexShader(
					vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), nullptr, &m_vertexShader);
				DebugAssert(result == S_OK, "Failed to load vertex shader.");
				result = renderingAPI.m_device->CreatePixelShader(
					pixelShader->GetBufferPointer(), pixelShader->GetBufferSize(), nullptr, &m_pixelShader);
				DebugAssert(result == S_OK, "Failed to load pixel shader.");
				if (m_vertexShader != nullptr
					&& m_pixelShader != nullptr)
				{
					DirectX11BufferLayoutParser parsedBufferLayout(bufferLayout);
					result = renderingAPI.m_device->CreateInputLayout(
						parsedBufferLayout.GetD3D11InputElementDesc(), parsedBufferLayout.GetNumElements(),
						vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), &m_inputLayout);
					DebugAssert(result == S_OK, "Failed to load input layout.");
					if (result == S_OK)
					{
						m_bufferLayout = bufferLayout;
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