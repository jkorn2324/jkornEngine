#include "EnginePCH.h"
#include "GraphicsRenderer.h"

#include "Rendering\IndexBuffer.h"
#include "Rendering\VertexBuffer.h"
#include "Rendering\ConstantBuffer.h"
#include "Rendering\Shader.h"
#include "Rendering\FrameBuffer.h"
#include "Rendering\Texture.h"

#include "Source\Vector.h"

namespace Engine
{

	GraphicsRenderer* GraphicsRenderer::s_graphicsRenderer = nullptr;


	GraphicsRenderer* GraphicsRenderer::Get()
	{
		return s_graphicsRenderer;
	}

	GraphicsRenderer::GraphicsRenderer()
		: m_deviceContext(nullptr),
		m_device(nullptr),
		m_swapChain(nullptr),
		m_screenHeight(0),
		m_screenWidth(0),
		m_backBufferRenderTarget(nullptr),
		m_currentRenderTarget(nullptr),
		m_activeIndexBuffer(nullptr),
		m_activeVertexBuffer(nullptr),
		m_activeShader(nullptr),
		m_frameBuffer(nullptr),
		m_samplerState(nullptr)
	{
		s_graphicsRenderer = this;
	}

	GraphicsRenderer::~GraphicsRenderer()
	{
#if _DEBUG
		ID3D11Debug* debug = nullptr;
		{
			HRESULT result = m_device->QueryInterface(__uuidof(ID3D11Debug),
				reinterpret_cast<void**>(&debug));
			DebugAssert(result == S_OK, "Unable to create debug device.");
		}
#endif
		if (s_graphicsRenderer == this)
		{
			s_graphicsRenderer = nullptr;
		}

		delete m_frameBuffer;

		m_samplerState->Release();
		m_backBufferRenderTarget->Release();
		m_swapChain->Release();
		m_deviceContext->Release();
		ULONG ref = m_device->Release();

#ifdef _DEBUG
		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
		debug->Release();
#endif
	}

	bool GraphicsRenderer::Initialize(const HWND& window)
	{
		const float WIDTH = 800.0f, HEIGHT = 600.0f;

		if (!CreateDeviceAndSwapChain(window,
			static_cast<std::uint32_t>(WIDTH), 
			static_cast<std::uint32_t>(HEIGHT)))
		{
			return false;
		}

		SetViewport(0.0f, 0.0f, (float)m_screenWidth, (float)m_screenHeight);

		if (!CreateBackBuffer())
		{
			return false;
		}

		FrameBufferSpecification frameBufferSpecification({
			{ FrameBufferAttachmentType::DEPTH_STENCIL }
		});
		frameBufferSpecification.width = m_screenWidth;
		frameBufferSpecification.height = m_screenHeight;
		m_frameBuffer = new FrameBuffer(frameBufferSpecification);

		// Creates the texture sampler.
		{
			D3D11_SAMPLER_DESC samplerDesc;
			ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			HRESULT output = m_device->CreateSamplerState(
				&samplerDesc, &m_samplerState);
			DebugAssert(output == S_OK, "Something went wrong with creating sampler.");
			m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
		}

		// Sets the primitive topology.
		m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return true;
	}

	void GraphicsRenderer::BeginFrame()
	{
		// Sets the render target to the back buffer.
		SetRenderTarget(m_backBufferRenderTarget, m_frameBuffer->GetDepthStencilView());
		
		{
			// Clears the render target color.
			MathLib::Vector4 color = 
				MathLib::Vector4(0.0f, 0.0f, 1.0f, 1.0f);
			ClearRenderTarget(color);
			m_frameBuffer->ClearDepthBuffer();
		}
	}

	void GraphicsRenderer::EndFrame()
	{
		m_swapChain->Present(1, 0);
	}

	void GraphicsRenderer::SetRenderTarget(ID3D11RenderTargetView* currentRenderTarget, ID3D11DepthStencilView* stencilView)
	{
		m_currentRenderTarget = currentRenderTarget;
		m_deviceContext->OMSetRenderTargets(1, &currentRenderTarget, stencilView);
	}

	void GraphicsRenderer::ClearRenderTarget(const MathLib::Vector4& color)
	{
		m_deviceContext->ClearRenderTargetView(m_currentRenderTarget, 
			reinterpret_cast<const float*>(&color));
	}

	void GraphicsRenderer::SetResolution(std::uint32_t width, std::uint32_t height)
	{
		m_screenWidth = width;
		m_screenHeight = height;

		DXGI_SWAP_CHAIN_DESC scDesc;
		HRESULT res = m_swapChain->GetDesc(&scDesc);
		DebugAssert(res == S_OK, "Failed to get the descriptor flags.");

		DXGI_MODE_DESC desc;
		std::memcpy(&desc, &(scDesc.BufferDesc), sizeof(DXGI_MODE_DESC));
		desc.Width = width;
		desc.Height = height;
		m_swapChain->ResizeTarget(&desc);
	}

	void GraphicsRenderer::SetViewport(float x, float y, float width, float height)
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = x;
		viewport.TopLeftY = y;
		m_deviceContext->RSSetViewports(1, &viewport);
	}

	void GraphicsRenderer::SetTexture(std::uint32_t slot, Texture* texture)
	{
		if (texture != nullptr)
		{
			texture->Bind(slot);
		}
	}

	void GraphicsRenderer::SetActiveIndexBuffer(IndexBuffer* indexBuffer)
	{
		if (m_activeIndexBuffer != indexBuffer)
		{
			m_activeIndexBuffer = indexBuffer;
			if (m_activeIndexBuffer != nullptr)
			{
				m_activeIndexBuffer->Bind();
			}
		}
	}

	void GraphicsRenderer::SetActiveVertexBuffer(VertexBuffer* vertexBuffer)
	{
		if (m_activeVertexBuffer != vertexBuffer)
		{
			m_activeVertexBuffer = vertexBuffer;
			if (m_activeVertexBuffer != nullptr)
			{
				m_activeVertexBuffer->Bind();
			}
		}
	}

	void GraphicsRenderer::SetConstantBuffer(const std::size_t& slot, ConstantBuffer* constantBuffer)
	{
		int flags = ConstantBufferFlags::COMPUTE_SHADER | ConstantBufferFlags::HULL_SHADER
			| ConstantBufferFlags::PIXEL_SHADER | ConstantBufferFlags::VERTEX_SHADER;
		SetConstantBuffer(slot, constantBuffer, flags);
	}

	void GraphicsRenderer::SetConstantBuffer(const std::size_t& slot, ConstantBuffer* constantBuffer, int flags)
	{
		if (constantBuffer != nullptr)
		{
			constantBuffer->Bind(slot, flags);
		}
	}

	void GraphicsRenderer::SetShader(class Shader* shader)
	{
		if (m_activeShader != shader)
		{
			m_activeShader = shader;
			
			if (m_activeShader != nullptr)
			{
				m_activeShader->Bind();
			}
		}
	}

	void GraphicsRenderer::DrawActiveElements()
	{
		if (m_activeVertexBuffer != nullptr
			&& m_activeShader != nullptr)
		{
			if (m_activeIndexBuffer != nullptr)
			{
				// Issues a draw call from an index buffer.
				m_deviceContext->DrawIndexed(
					m_activeIndexBuffer->GetNumIndices(), 0, 0);
			}
			else
			{
				// Issues a draw call from a vertex buffer.
				m_deviceContext->Draw(
					m_activeVertexBuffer->GetNumVerts(), 0);
			}
		}
	}

	bool GraphicsRenderer::CreateDeviceAndSwapChain(const HWND& window, 
		std::uint32_t resolutionWidth, std::uint32_t resolutionHeight)
	{
		m_screenWidth = resolutionWidth;
		m_screenHeight = resolutionHeight;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = static_cast<UINT>(resolutionWidth);
		swapChainDesc.BufferDesc.Height = static_cast<UINT>(resolutionHeight);
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = window;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		D3D_FEATURE_LEVEL inFeatureLevels = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL outFeatureLevel;
		HRESULT result = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
#ifdef _DEBUG
			D3D11_CREATE_DEVICE_DEBUG,
#else
			0,
#endif
			&inFeatureLevels,
			1,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&m_swapChain,
			&m_device,
			&outFeatureLevel,
			&m_deviceContext);

		DebugAssert(result == S_OK, "Failed to create the device and swap chain.");
		return result == S_OK;
	}

	bool GraphicsRenderer::CreateBackBuffer()
	{
		ID3D11Texture2D* backBuffer;
		HRESULT result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			(LPVOID*)&backBuffer);
		DebugAssert(result == S_OK, "Failed to create the back buffer.");
		result = m_device->CreateRenderTargetView(backBuffer,
			nullptr, &m_backBufferRenderTarget);
		DebugAssert(result == S_OK, "Failed to set the render target view.");
		backBuffer->Release();
		return true;
	}
}