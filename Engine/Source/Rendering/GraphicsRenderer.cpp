#include "EnginePCH.h"
#include "GraphicsRenderer.h"

#include "Rendering\IndexBuffer.h"
#include "Rendering\VertexBuffer.h"
#include "Rendering\ConstantBuffer.h"
#include "Rendering\Shader.h"
#include "Rendering\FrameBuffer.h"

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
		if (s_graphicsRenderer == this)
		{
			s_graphicsRenderer = nullptr;
		}

		if (m_backBufferRenderTarget != nullptr)
		{
			m_backBufferRenderTarget->Release();
		}

		delete m_frameBuffer;

		if (m_deviceContext != nullptr)
		{
			m_deviceContext->Release();
		}
		if (m_device != nullptr)
		{
			m_device->Release();
		}
		if (m_swapChain != nullptr)
		{
			m_swapChain->Release();
		}
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

		SetViewport(0.0f, 0.0f, WIDTH, HEIGHT);

		if (!CreateBackBuffer())
		{
			return false;
		}

		FrameBufferSpecification frameBufferSpecification({
			{ FrameBufferTextureType::DEPTH_STENCIL }
		});
		frameBufferSpecification.width = m_screenWidth;
		frameBufferSpecification.height = m_screenHeight;
		m_frameBuffer = new FrameBuffer(frameBufferSpecification);

		// Creates the texture sampler.
		{
			D3D11_SAMPLER_DESC samplerDesc;
			ZeroMemory(&samplerDesc, sizeof(samplerDesc));
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			HRESULT output = m_device->CreateSamplerState(
				&samplerDesc, &m_samplerState);
			DebugAssert(output == S_OK, "Something went wrong with creating sampler.");
			m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
		}

		// Sets the primitive topology.
		m_deviceContext->IASetPrimitiveTopology(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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

	void GraphicsRenderer::SetViewport(float x, float y, float width, float height)
	{
		D3D11_VIEWPORT viewport;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = x;
		viewport.TopLeftY = y;
		m_deviceContext->RSSetViewports(1, &viewport);
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
		swapChainDesc.BufferDesc.Width = resolutionWidth;
		swapChainDesc.BufferDesc.Height = resolutionHeight;
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
			0,
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
			(void**)&backBuffer);
		DebugAssert(result == S_OK, "Failed to create the back buffer.");
		if (result != S_OK)
		{
			return false;
		}
		result = m_device->CreateRenderTargetView((ID3D11Resource*)backBuffer,
			nullptr, &m_backBufferRenderTarget);
		DebugAssert(result == S_OK, "Failed to set the render target view.");
		if (result != S_OK)
		{
			backBuffer->Release();
			return false;
		}
		SetRenderTarget(m_backBufferRenderTarget, nullptr);
		backBuffer->Release();
		return true;
	}
}