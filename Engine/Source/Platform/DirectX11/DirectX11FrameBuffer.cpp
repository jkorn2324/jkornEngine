#include "EnginePCH.h"
#include "DirectX11FrameBuffer.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"
#include "DirectX11Texture.h"

namespace Engine
{
#pragma region d3d11_funcs

	static ID3D11DepthStencilState* CreateDepthStencilState(D3D11_COMPARISON_FUNC func,
		ID3D11Device* device)
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = func;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xff;
		depthStencilDesc.StencilWriteMask = 0xff;

		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		ID3D11DepthStencilState* outDepthStencilState;
		device->CreateDepthStencilState(&depthStencilDesc, &outDepthStencilState);
		return outDepthStencilState;
	}

#pragma endregion


	DirectX11FrameBuffer::DirectX11FrameBuffer(const FrameBufferSpecification& specification)
		: FrameBuffer(specification),
		m_depthTexture(),
		m_renderTargetTexture()
	{
		CreateBuffers();
	}

	DirectX11FrameBuffer::~DirectX11FrameBuffer() 
	{
	}

	void DirectX11FrameBuffer::CreateBuffers()
	{
		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)(
			GraphicsRenderer::GetRenderingAPI());

		// Creates the depth stencil buffer.
		if (m_depthStencilSpecification.textureType != FrameBufferAttachmentType::TYPE_NONE)
		{
			{
				ID3D11DepthStencilState* depthStencilState = CreateDepthStencilState(
					D3D11_COMPARISON_LESS_EQUAL, renderingAPI.m_device);
				renderingAPI.m_deviceContext->OMSetDepthStencilState(depthStencilState, 0);
				depthStencilState->Release();
			}
			// Creates the view texture for the frame buffer.
			CreateViewTexture(&m_depthTexture, &renderingAPI, m_depthStencilSpecification);
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
			case FrameBufferAttachmentType::RENDER_TARGET:
			{
				ID3D11Texture2D* texture;
				ID3D11ShaderResourceView* shaderResource;
				CreateTextureWithShaderResource(api,
					D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
					DXGI_FORMAT_R32G32B32A32_FLOAT, 1, &texture, &shaderResource);
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

		// If there is a render target view than we should bind it to that otherwise bind it to the back buffer.
		if (m_renderTargetTexture.m_view != nullptr)
		{
			renderingAPI.SetRenderTarget((ID3D11RenderTargetView*)m_renderTargetTexture.m_view,
				(ID3D11DepthStencilView*)m_depthTexture.m_view);
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
		renderingAPI.SetViewport(0.0f, 0.0f,
			(float)m_frameBufferSpecification.width, (float)m_frameBufferSpecification.height);
	}
	
	Texture* DirectX11FrameBuffer::GetTexture(FrameBufferAttachmentType type) const
	{
		switch (type)
		{
			case FrameBufferAttachmentType::DEPTH_STENCIL:	return m_depthTexture.texture;
			case FrameBufferAttachmentType::RENDER_TARGET:	return m_renderTargetTexture.texture;
		}
		return nullptr;
	}
}