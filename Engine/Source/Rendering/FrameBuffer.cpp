#include "EnginePCH.h"
#include "FrameBuffer.h"

#include "GraphicsRenderer.h"

namespace Engine
{
#pragma region d3d11_funcs

	static ID3D11DepthStencilState* CreateDepthStencilState(D3D11_COMPARISON_FUNC func,
		ID3D11Device*& device)
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

	static bool CreateDepthStencil(std::uint32_t width, std::uint32_t height, ID3D11Device*& device,
		ID3D11Texture2D** depthTexture, ID3D11DepthStencilView** depthStencilView)
	{
		D3D11_TEXTURE2D_DESC depthTextureDesc;
		ZeroMemory(&depthTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		depthTextureDesc.Width = width;
		depthTextureDesc.Height = height;
		depthTextureDesc.MipLevels = 1;
		depthTextureDesc.ArraySize = 1;
		depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTextureDesc.SampleDesc.Count = 1;
		depthTextureDesc.SampleDesc.Quality = 0;
		depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTextureDesc.CPUAccessFlags = 0;
		depthTextureDesc.MiscFlags = 0;
		HRESULT result = device->CreateTexture2D(
			&depthTextureDesc, nullptr, depthTexture);
		if (result != S_OK)
		{
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilDesc.Texture2D.MipSlice = 0;
		result = device->CreateDepthStencilView(
			*depthTexture, &depthStencilDesc, depthStencilView);
		if (result != S_OK)
		{
			return false;
		}
		return true;
	}

#pragma endregion

	FrameBuffer::FrameBuffer(const FrameBufferSpecification& specification)
		: m_frameBufferSpecification(),
		m_depthStencilSpecification(),
		m_depthStencilView(nullptr),
		m_depthTexture(nullptr)
	{
		m_frameBufferSpecification = specification;

		for (const auto& attachment : specification.attachments)
		{
			if (attachment.textureType == TYPE_DEPTH24_STENCIL8)
			{
				m_depthStencilSpecification = attachment;
				continue;
			}
			// TODO: create sampler
		}
		CreateBuffers();
	}

	FrameBuffer::~FrameBuffer()
	{
		if (m_depthTexture != nullptr)
		{
			m_depthTexture->Release();
		}

		if (m_depthStencilView != nullptr)
		{
			m_depthStencilView->Release();
		}
	}

	ID3D11DepthStencilView* FrameBuffer::GetDepthStencilView() const
	{
		return m_depthStencilView;
	}

	void FrameBuffer::CreateBuffers()
	{
		GraphicsRenderer* graphicsRenderer = GraphicsRenderer::Get();
		DebugAssert(graphicsRenderer != nullptr, "Graphics Renderer doesn't exist.");
		
		if (graphicsRenderer == nullptr)
		{
			return;
		}

		// Creates the depth stencil buffer.
		if (m_depthStencilSpecification.textureType != FrameBufferTextureType::TYPE_NONE)
		{
			{
				ID3D11DepthStencilState* depthStencilState = CreateDepthStencilState(
					D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS, graphicsRenderer->m_device);
				graphicsRenderer->m_deviceContext->OMSetDepthStencilState(depthStencilState, 0);
				depthStencilState->Release();
			}
			{
				bool result = CreateDepthStencil(m_frameBufferSpecification.width, m_frameBufferSpecification.height,
					graphicsRenderer->m_device, &m_depthTexture, &m_depthStencilView);
				DebugAssert(result, "Depth Stencil Failed to be created.");
			}
		}
	}

	void FrameBuffer::ClearDepthBuffer()
	{
		GraphicsRenderer* graphics = GraphicsRenderer::Get();

		if (m_depthStencilView != nullptr)
		{
			graphics->m_deviceContext->ClearDepthStencilView(m_depthStencilView,
				D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
	}
	
	void FrameBuffer::Bind() const
	{
		GraphicsRenderer* graphics = GraphicsRenderer::Get();
		graphics->SetViewport(0.0f, 0.0f,
			m_frameBufferSpecification.width, m_frameBufferSpecification.height);
	}
}