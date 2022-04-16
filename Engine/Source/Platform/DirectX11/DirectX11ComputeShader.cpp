#include "EnginePCH.h"
#include "DirectX11ComputeShader.h"

#include "GraphicsRenderer.h"
#include "ConstantBuffer.h"
#include "DirectX11RenderingAPI.h"
#include "DirectX11Utils.h"
#include "Texture.h"

#include "AssetReferenceManager.h"

#include <d3d11.h>
#include <d3dcompiler.h>


namespace Engine
{
	void DirectX11ComputeBuffer::Deallocate()
	{
		if (buffer != nullptr)
		{
			buffer->Release();
			buffer = nullptr;
		}
	}

	DirectX11ComputeShader::DirectX11ComputeShader()
		: m_computeShader(nullptr), m_constantBuffers(20),
		m_inputResources(20), m_outputResources(20),
		m_textures(8)
	{
	}
	
	DirectX11ComputeShader::~DirectX11ComputeShader()
	{
		for (size_t i = 0; i < m_computeBuffers.size(); i++)
		{
			auto& computeBuffer = m_computeBuffers[i];
			switch (computeBuffer.computeBuffer->GetBufferType())
			{
				case Type_ConstantBuffer:
				{
					m_constantBuffers[computeBuffer.viewIndex]->Release();
					break;
				}
				case Type_StructuredBuffer:
				{
					m_inputResources[computeBuffer.viewIndex]->Release();
					break;
				}
				case Type_RWStructuredBuffer:
				{
					m_outputResources[computeBuffer.viewIndex]->Release();
					break;
				}
			}
			computeBuffer.Deallocate();
		}

		m_outputResources.clear();
		m_inputResources.clear();
		m_constantBuffers.clear();
		m_textures.clear();

		if (m_computeShader != nullptr)
		{
			m_computeShader->Release();
		}
	}

	bool DirectX11ComputeShader::IsValid() const
	{
		return m_computeShader != nullptr;
	}

	void DirectX11ComputeShader::SetTexture(const AssetRef<Texture>& texture)
	{
		static DirectX11RenderingAPI& s_renderingAPI
			= (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();

		if (texture)
		{
			const ID3D11ShaderResourceView* constShaderRes = reinterpret_cast<const ID3D11ShaderResourceView*>(
				texture->GetTextureID());
			ID3D11ShaderResourceView* srv = const_cast<ID3D11ShaderResourceView*>(constShaderRes);
			m_textures.push_back({ texture, (uint32_t)m_inputResources.size() });
			m_inputResources.push_back(srv);
		}
	}

	void DirectX11ComputeShader::SetBuffer(const std::shared_ptr<ComputeBuffer>& buffer)
	{
		static DirectX11RenderingAPI& s_renderingAPI 
			= (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();

		DirectX11ComputeBuffer computeBuffer;
		computeBuffer.computeBuffer = buffer;

		switch (buffer->GetBufferType())
		{
			case Type_ConstantBuffer:
			{
				computeBuffer.buffer = DirectX11Utils::CreateConstantBuffer(
					s_renderingAPI.m_device, (const void*)buffer->GetData(), buffer->GetStride(), buffer->GetNumElements());
				computeBuffer.viewIndex = (uint32_t)m_constantBuffers.size();
				m_constantBuffers.push_back(computeBuffer.buffer);
				break;
			}
			case Type_RWStructuredBuffer:
			{
				computeBuffer.buffer = DirectX11Utils::CreateStructuredBuffer(
					s_renderingAPI.m_device, (const void*)buffer->GetData(), buffer->GetStride(),
					buffer->GetNumElements(), (Engine::DirectX11BufferType)buffer->GetBufferType());
				computeBuffer.viewIndex = (uint32_t)m_outputResources.size();
				auto uav = DirectX11Utils::CreateBufferUnorderedAccessView(
					s_renderingAPI.m_device, computeBuffer.buffer);
				m_outputResources.push_back(uav);
				break;
			}
			case Type_StructuredBuffer:
			{
				computeBuffer.buffer = DirectX11Utils::CreateStructuredBuffer(
					s_renderingAPI.m_device, (const void*)buffer->GetData(), buffer->GetStride(),
					buffer->GetNumElements(), (Engine::DirectX11BufferType)buffer->GetBufferType());
				computeBuffer.viewIndex = (uint32_t)m_inputResources.size();
				auto srv = DirectX11Utils::CreateBufferShaderResourceView(
					s_renderingAPI.m_device, computeBuffer.buffer);
				m_inputResources.push_back(srv);
				break;
			}
		}
		m_computeBuffers.push_back(computeBuffer);
	}

	void DirectX11ComputeShader::Dispatch(uint32_t x, uint32_t y, uint32_t z)
	{
		if (!IsValid()) return;

		static ID3D11UnorderedAccessView* s_nullUAV[1] = { nullptr };
		static ID3D11ShaderResourceView* s_nullSRV[2] = { nullptr, nullptr };
		static ID3D11Buffer* s_nullCBuffer[1] = { nullptr };

		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();

		// Bind Shader Resources & Output Resources.
		renderingAPI.m_deviceContext->CSSetShader(m_computeShader, nullptr, 0);
		renderingAPI.m_deviceContext->CSSetShaderResources(0,
			(uint32_t)m_inputResources.size(), &m_inputResources[0]);
		renderingAPI.m_deviceContext->CSSetUnorderedAccessViews(0,
			(uint32_t)m_outputResources.size(), &m_outputResources[0], nullptr);

		if (!m_constantBuffers.empty())
		{
			renderingAPI.m_deviceContext->CSSetConstantBuffers(
				0, (UINT)m_constantBuffers.size(), &m_constantBuffers[0]);
		}
		renderingAPI.m_deviceContext->Dispatch(x, y, z);

		// Disable everything.
		renderingAPI.m_deviceContext->CSGetShader(nullptr, nullptr, 0);
		renderingAPI.m_deviceContext->CSSetUnorderedAccessViews(0, 1, s_nullUAV, nullptr);
		renderingAPI.m_deviceContext->CSSetShaderResources(0, 2, s_nullSRV);
		renderingAPI.m_deviceContext->CSSetConstantBuffers(0, 1, s_nullCBuffer);
	}

	bool DirectX11ComputeShader::Load(const wchar_t* fileName)
	{
		ID3DBlob* shaderBlob = nullptr;
		if (DirectX11Utils::CompileShader(fileName, "CSMain", "cs_5_0", shaderBlob))
		{
			DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();
			HRESULT result = renderingAPI.m_device->CreateComputeShader(shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(), nullptr, &m_computeShader);
			bool success = S_OK == result;
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