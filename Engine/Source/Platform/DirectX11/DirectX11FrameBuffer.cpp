#include "EnginePCH.h"
#include "DirectX11FrameBuffer.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"
#include "DirectX11Texture.h"
#include "DirectX11Utils.h"

namespace Engine
{
	namespace
	{
		void CreateTextureWithShaderResource(DirectX11RenderingAPI* api, UINT bindFlags,
			DXGI_FORMAT textureFormat, UINT quality, ID3D11Texture2D** outTexture, ID3D11ShaderResourceView** outShaderResource, DXGI_FORMAT shaderResourceFormat, UINT width, UINT height)
		{
			*outTexture = DirectX11Utils::CreateTexture2D(api->GetDevice(), width,
				height, bindFlags, textureFormat, quality);
			*outShaderResource = DirectX11Utils::CreateTextureShaderResourceView(
				api->GetDevice(), *outTexture, shaderResourceFormat);
		}

		template<typename T>
		void CreateViewTexture(DirectX11ViewTexture* outViewTexture, DirectX11RenderingAPI* api, const FrameBufferAttachment<T>& attachment,
			UINT width, UINT height)
		{
			static_assert(false);
		}

		template<>
		void CreateViewTexture<FrameBufferDepthStencilContext>(DirectX11ViewTexture* outViewTexture, DirectX11RenderingAPI* api, 
			const FrameBufferAttachment<FrameBufferDepthStencilContext>& attachment, UINT width, UINT height)
		{
			ID3D11Texture2D* texture;
			ID3D11ShaderResourceView* shaderResourceView = nullptr;
			DXGI_FORMAT depthStencilFormat{};

			// If we are reading a texture, make sure that the format is like so.
			if (attachment.format.readTexture)
			{
				CreateTextureWithShaderResource(api,
					D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32_TYPELESS,
					0, &texture, &shaderResourceView, DXGI_FORMAT_R32_FLOAT, width, height);
				depthStencilFormat = DXGI_FORMAT_D32_FLOAT;
			}
			else
			{
				DXGI_FORMAT graphicsFormat = Utility::DirectX11::ToDXGIFormat(attachment.format.format);
				texture = DirectX11Utils::CreateTexture2D(api->GetDevice(),
					width, height, D3D11_BIND_DEPTH_STENCIL,
					graphicsFormat, 0);
				depthStencilFormat = graphicsFormat;
			}

			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
			ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
			depthStencilDesc.Format = depthStencilFormat;
			depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilDesc.Texture2D.MipSlice = 0;

			ID3D11DepthStencilView* depthStencilView;
			HRESULT result = api->GetDevice()->CreateDepthStencilView(
				texture, &depthStencilDesc, &depthStencilView);
			JKORN_ENGINE_ASSERT(result == S_OK, "Failed to create the depth stencil view.");

			outViewTexture->m_view = depthStencilView;
			outViewTexture->texture = new DirectX11Texture(texture, shaderResourceView);
		}

		template<>
		void CreateViewTexture<ColorFormat>(DirectX11ViewTexture* outViewTexture, DirectX11RenderingAPI* api,
			const FrameBufferAttachment<ColorFormat>& attachment, UINT width, UINT height)
		{
			DXGI_FORMAT format = Utility::DirectX11::ToDXGIFormat(attachment.format);
			ID3D11Texture2D* texture;
			ID3D11ShaderResourceView* shaderResource;
			CreateTextureWithShaderResource(api,
				D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
				format, 0, &texture, &shaderResource, format, width, height);
			ID3D11RenderTargetView* renderTargetView;
			HRESULT result = api->GetDevice()->CreateRenderTargetView(texture,
				nullptr, &renderTargetView);
			JKORN_ENGINE_ASSERT(result == S_OK, "Render target view was unsuccessfully created.");
			outViewTexture->m_view = renderTargetView;
			outViewTexture->texture = new DirectX11Texture(texture, shaderResource);
		}
	}

	DirectX11FrameBuffer::DirectX11FrameBuffer(const FrameBufferSpecification& specification)
		: FrameBuffer(specification),
		m_depthTexture(),
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
			Engine::DirectX11ViewTexture& viewTexture = m_renderTargetTextures[i];
			viewTexture.Deallocate();
		}
		if (m_depthStencilState != nullptr)
		{
			m_depthStencilState->Release();
		}
	}

	void DirectX11FrameBuffer::CreateBuffers()
	{
		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)(
			GraphicsRenderer::GetRenderingAPI());

		// Creates the depth stencil buffer if the format is not none.
		Engine::FrameBufferAttachment<Engine::FrameBufferDepthStencilContext>& depthStencil = 
			m_frameBufferSpecification.attachments.depthStencilAttachment;
		if (depthStencil.format.format != DepthFormat_None)
		{
			{
				m_depthStencilState = DirectX11Utils::CreateDepthStencilState(
					D3D11_COMPARISON_LESS, renderingAPI.m_device);
				renderingAPI.m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);
			}
			// Creates the view texture for the frame buffer.
			CreateViewTexture(&m_depthTexture, &renderingAPI, m_frameBufferSpecification.attachments.depthStencilAttachment,
				m_frameBufferSpecification.width, m_frameBufferSpecification.height);
		}

		// The number of render targets.
		size_t numRenderTargets = (size_t)m_frameBufferSpecification.attachments.colorAttachments.size();
		if (numRenderTargets > 0)
		{
			for (uint32_t i = 0; i < numRenderTargets; i++)
			{
				Engine::DirectX11ViewTexture& renderTargetTexture = m_renderTargetTextures[i];
				CreateViewTexture(&renderTargetTexture, &renderingAPI, m_frameBufferSpecification.attachments.colorAttachments[i],
					m_frameBufferSpecification.width, m_frameBufferSpecification.height);
			}
		}
	}

	void DirectX11FrameBuffer::Bind() const
	{
		static ID3D11RenderTargetView* s_renderTargetViews[DirectX11FrameBuffer::MaxRenderTextures];

		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)
			GraphicsRenderer::GetRenderingAPI();

		uint32_t numRenderTextures = (uint32_t)GetNumRenderTargets();
		// If there are more than one render target, bind them, otherwise bind it to the back buffer.
		if (numRenderTextures > 0)
		{
			// Updates the render targets.
			for (uint32_t i = 0; i < numRenderTextures; i++)
			{
				const DirectX11ViewTexture& texture = m_renderTargetTextures[i];
				s_renderTargetViews[i] = reinterpret_cast<ID3D11RenderTargetView*>(texture.m_view);
			}
			renderingAPI.SetRenderTargets(numRenderTextures,
				s_renderTargetViews, (ID3D11DepthStencilView*)m_depthTexture.m_view);
		}
		else
		{
			renderingAPI.SetRenderTarget(renderingAPI.m_backBufferRenderTargetView,
				(ID3D11DepthStencilView*)m_depthTexture.m_view);
		}
		
		renderingAPI.ClearRenderTargetViewColors();
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
		// Clears the render target view colors.
		renderingAPI.ClearRenderTargetViewColors();
	}

	void DirectX11FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		FrameBuffer::Resize(width, height);

		bool shouldRegenerate = false;
		size_t numRenderTargets = GetNumRenderTargets();
		if (numRenderTargets > 0)
		{
			for (size_t i = 0; i < numRenderTargets; i++)
			{
				const auto& renderTargetTexture = m_renderTargetTextures[i];
				if (!renderTargetTexture)
				{
					continue;
				}
				shouldRegenerate |= height > renderTargetTexture.texture->GetHeight()
					|| width > renderTargetTexture.texture->GetHeight();
				if (shouldRegenerate)
				{
					break;
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
				for (size_t i = 0; i < numRenderTargets; i++)
				{
					Engine::DirectX11ViewTexture& renderTarget = m_renderTargetTextures[i];
					renderTarget.Deallocate();
				}
			}

			DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();
			for (uint16_t i = 0; i < m_frameBufferSpecification.attachments.colorAttachments.size(); i++)
			{
				const Engine::FrameBufferAttachment<ColorFormat>& renderTargetSpecification 
					= m_frameBufferSpecification.attachments.colorAttachments[i];
				if (renderTargetSpecification.format != ColorFormat_None)
				{
					DirectX11ViewTexture& viewTexture = m_renderTargetTextures[i];
					CreateViewTexture(&viewTexture, &renderingAPI, renderTargetSpecification, 
						m_frameBufferSpecification.width, m_frameBufferSpecification.height);
				}
			}
		}

		// Regenerate Depth Texture.
		{
			m_depthTexture.Deallocate();

			DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();
			const Engine::FrameBufferAttachment<FrameBufferDepthStencilContext>& depthStencil
				= m_frameBufferSpecification.attachments.depthStencilAttachment;
			if (depthStencil.format.format != DepthFormat_None)
			{
				CreateViewTexture(&m_depthTexture, &renderingAPI, depthStencil, 
					m_frameBufferSpecification.width, m_frameBufferSpecification.height);
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
