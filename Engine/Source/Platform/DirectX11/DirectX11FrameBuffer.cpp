#include "EnginePCH.h"
#include "DirectX11FrameBuffer.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"
#include "DirectX11Texture.h"
#include "DirectX11Utils.h"

namespace Engine
{
	DirectX11FrameBuffer::DirectX11FrameBuffer(const FrameBufferSpecification& specification)
		: FrameBuffer(specification),
		m_depthTexture(),
		m_renderTargets(nullptr),
		m_renderTargetTextures(nullptr),
		m_depthStencilState(nullptr)
	{
		CreateBuffers();
	}

	DirectX11FrameBuffer::~DirectX11FrameBuffer() 
	{
		m_depthTexture.Deallocate();

		uint32_t numRenderTargets = (uint32_t)GetNumRenderTargets();
		for (uint32_t i = 0; i < numRenderTargets; i++)
		{
			auto& viewTexture = m_renderTargetTextures[i];
			viewTexture.Deallocate();
			m_renderTargets[i] = nullptr;
		}
		delete[] m_renderTargets;
		delete[] m_renderTargetTextures;

		if (m_depthStencilState != nullptr)
		{
			m_depthStencilState->Release();
		}
	}

	void DirectX11FrameBuffer::CreateBuffers()
	{
		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)(
			GraphicsRenderer::GetRenderingAPI());

		// Creates the depth stencil buffer.
		if (m_depthStencilSpecification.textureType != FrameBufferAttachmentType::FRAME_BUFFER_ATTACHMENT_TYPE_NONE)
		{
			{
				m_depthStencilState = DirectX11Utils::CreateDepthStencilState(
					D3D11_COMPARISON_LESS, renderingAPI.m_device);
				renderingAPI.m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);
			}
			// Creates the view texture for the frame buffer.
			CreateViewTexture(&m_depthTexture, &renderingAPI, m_depthStencilSpecification);
		}

		uint32_t numRenderTargets = (uint32_t)GetNumRenderTargets();
		if (numRenderTargets > 0)
		{
			m_renderTargets = new ID3D11RenderTargetView*[numRenderTargets];
			m_renderTargetTextures = new DirectX11ViewTexture[numRenderTargets];

			for (uint32_t i = 0; i < numRenderTargets; i++)
			{
				auto& renderTargetTexture = m_renderTargetTextures[i];
				CreateViewTexture(&renderTargetTexture, &renderingAPI, m_renderTargetSpecifications[i]);
				m_renderTargets[i] = reinterpret_cast<ID3D11RenderTargetView*>(renderTargetTexture.m_view);
			}
		}
	}

	void DirectX11FrameBuffer::CreateViewTexture(DirectX11ViewTexture* outViewTexture, DirectX11RenderingAPI* api, const FrameBufferAttachment& attachment)
	{
		switch (attachment.textureType)
		{
			case FrameBufferAttachmentType::DEPTH_STENCIL:
			{
				ID3D11Texture2D* texture;
				ID3D11ShaderResourceView* shaderResourceView = nullptr;
				DXGI_FORMAT depthStencilFormat{};

				if (attachment.exportAsTexture)
				{
					CreateTextureWithShaderResource(api,
						D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32_TYPELESS,
						0, &texture, &shaderResourceView, DXGI_FORMAT_R32_FLOAT);
					depthStencilFormat = DXGI_FORMAT_D32_FLOAT;
				}
				else
				{
					CreateTexture(api,
						D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT_D24_UNORM_S8_UINT, 0, &texture);
					depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
				}

				D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
				ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
				depthStencilDesc.Format = depthStencilFormat;
				depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				depthStencilDesc.Texture2D.MipSlice = 0;

				ID3D11DepthStencilView* depthStencilView;
				HRESULT result = api->m_device->CreateDepthStencilView(
					texture, &depthStencilDesc, &depthStencilView);
				DebugAssert(result == S_OK, "Failed to create the depth stencil view.");

				outViewTexture->m_view = depthStencilView;
				outViewTexture->texture = new DirectX11Texture(texture, shaderResourceView);
				break;
			}
			case FrameBufferAttachmentType::TYPE_RGBA_32:
			{
				ID3D11Texture2D* texture;
				ID3D11ShaderResourceView* shaderResource;
				CreateTextureWithShaderResource(api,
					D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
					DXGI_FORMAT_R32G32B32A32_FLOAT, 0, &texture, &shaderResource);
				ID3D11RenderTargetView* renderTargetView;
				HRESULT result = api->m_device->CreateRenderTargetView(texture,
					nullptr, &renderTargetView);
				DebugAssert(result == S_OK, "Render target view was unsuccessfully created.");
				outViewTexture->m_view = renderTargetView;
				outViewTexture->texture = new DirectX11Texture(texture, shaderResource);
				break;
			}
			case FrameBufferAttachmentType::TYPE_INT:
			{
				ID3D11Texture2D* texture;
				ID3D11ShaderResourceView* shaderResource;
				CreateTextureWithShaderResource(api,
					D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
					DXGI_FORMAT_R32_SINT, 0, &texture, &shaderResource, DXGI_FORMAT_R32_SINT);
				ID3D11RenderTargetView* renderTargetView;
				HRESULT result = api->m_device->CreateRenderTargetView(texture,
					nullptr, &renderTargetView);
				DebugAssert(result == S_OK, "Render target view was unsuccessfully created.");
				outViewTexture->m_view = renderTargetView;
				outViewTexture->texture = new DirectX11Texture(texture, shaderResource);
				break;
			}
		}
	}

	void DirectX11FrameBuffer::CreateTextureWithShaderResource(DirectX11RenderingAPI* api, UINT bindFlags, 
		DXGI_FORMAT textureFormat, UINT quality, ID3D11Texture2D** outTexture, ID3D11ShaderResourceView** outShaderResource, DXGI_FORMAT shaderResourceFormat)
	{
		CreateTexture(api, bindFlags, textureFormat, quality, outTexture);

		HRESULT result;
		if (shaderResourceFormat != DXGI_FORMAT_UNKNOWN)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
			ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			viewDesc.Texture2D.MipLevels = 1;
			viewDesc.Format = shaderResourceFormat;
			viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			result = api->m_device->CreateShaderResourceView(*outTexture,
				&viewDesc, outShaderResource);
		}
		else
		{
			result = api->m_device->CreateShaderResourceView(*outTexture,
				nullptr, outShaderResource);
		}
		DebugAssert(result == S_OK, "Shader Resource failed to be created when creating view target.");
	}

	void DirectX11FrameBuffer::CreateTexture(DirectX11RenderingAPI* api, UINT bindFlags, DXGI_FORMAT format, UINT quality, ID3D11Texture2D** texture2D)
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = (UINT)m_frameBufferSpecification.width;
		desc.Height = (UINT)m_frameBufferSpecification.height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = quality;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = bindFlags;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		HRESULT result = api->m_device->CreateTexture2D(&desc, nullptr, texture2D);
		DebugAssert(result == S_OK, "View Texture2D failed to be created when creating view target.");
	}

	void DirectX11FrameBuffer::Bind() const
	{
		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)
			GraphicsRenderer::GetRenderingAPI();

		uint32_t numRenderTextures = (uint32_t)GetNumRenderTargets();
		// If there are more than one render target, bind them, otherwise bind it to the back buffer.
		if (numRenderTextures > 0)
		{
			renderingAPI.SetRenderTargets(numRenderTextures,
				m_renderTargets, (ID3D11DepthStencilView*)m_depthTexture.m_view);
		}
		else
		{
			renderingAPI.SetRenderTarget(renderingAPI.m_backBufferRenderTargetView,
				(ID3D11DepthStencilView*)m_depthTexture.m_view);
		}
		
		renderingAPI.Clear();
		if (m_depthTexture.m_view != nullptr)
		{
			renderingAPI.m_deviceContext->ClearDepthStencilView((ID3D11DepthStencilView*)m_depthTexture.m_view,
				D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
		// Sets the depth stencil state.
		if (m_depthStencilState != nullptr)
		{
			renderingAPI.m_deviceContext->OMSetDepthStencilState(
				m_depthStencilState, 0);
		}
		renderingAPI.SetViewport(0.0f, 0.0f,
			(float)m_frameBufferSpecification.width, (float)m_frameBufferSpecification.height);
	}
	
	void DirectX11FrameBuffer::UnBind() const
	{
		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)
			GraphicsRenderer::GetRenderingAPI();
		renderingAPI.SetRenderTarget(renderingAPI.m_backBufferRenderTargetView, nullptr);
		renderingAPI.Clear();
	}

	void DirectX11FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		FrameBuffer::Resize(width, height);

		bool shouldRegenerate = false;
		auto numRenderTargets = GetNumRenderTargets();
		if (numRenderTargets > 0)
		{
			for (auto i = 0; i < numRenderTargets; i++)
			{
				const auto& renderTargetTexture = m_renderTargetTextures[i];
				if (renderTargetTexture)
				{
					shouldRegenerate |= height > renderTargetTexture.texture->GetHeight()
						|| width > renderTargetTexture.texture->GetHeight();
					if (shouldRegenerate) break;
				}
			}
		}

		if (m_depthTexture)
		{
			shouldRegenerate |= height > m_depthTexture.texture->GetHeight()
				|| width > m_depthTexture.texture->GetHeight();
		}
		if (shouldRegenerate)
		{
			ReGenerateTextures();
		}
	}

	void DirectX11FrameBuffer::ReGenerateTextures()
	{
		// Regenerate Render Texture.
		{
			// Deallocates the render targets.
			size_t numRenderTargets = GetNumRenderTargets();
			if (numRenderTargets > 0)
			{
				for (auto i = 0; i < numRenderTargets; i++)
				{
					auto& renderTarget = m_renderTargetTextures[i];
					m_renderTargets[i] = nullptr;
					renderTarget.Deallocate();
				}
			}

			DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();
			for (uint16_t i = 0; i < m_renderTargetSpecifications.size(); i++)
			{
				const auto& renderTargetSpecification = m_renderTargetSpecifications[i];
				if (renderTargetSpecification.textureType 
					!= FrameBufferAttachmentType::FRAME_BUFFER_ATTACHMENT_TYPE_NONE)
				{
					DirectX11ViewTexture& viewTexture = m_renderTargetTextures[i];
					CreateViewTexture(&viewTexture, &renderingAPI, renderTargetSpecification);
					m_renderTargets[i] = reinterpret_cast<ID3D11RenderTargetView*>(viewTexture.m_view);
				}
			}
		}

		// Regenerate Depth Texture.
		{
			m_depthTexture.Deallocate();

			DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();
			if (m_depthStencilSpecification.textureType
				!= FrameBufferAttachmentType::FRAME_BUFFER_ATTACHMENT_TYPE_NONE)
			{
				CreateViewTexture(&m_depthTexture, &renderingAPI, m_depthStencilSpecification);
			}
		}
	}

	Texture* DirectX11FrameBuffer::GetDepthTexture() const
	{
		return m_depthTexture.texture;
	}
	
	Texture* DirectX11FrameBuffer::GetRenderTargetTexture(uint32_t index) const
	{
		auto numRenderTargets = GetNumRenderTargets();
		if (index >= (size_t)numRenderTargets) return nullptr;
		return m_renderTargetTextures[index].texture;
	}
}