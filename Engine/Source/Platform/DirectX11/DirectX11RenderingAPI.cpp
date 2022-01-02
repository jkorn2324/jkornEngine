#include "EnginePCH.h"
#include "DirectX11RenderingAPI.h"

#include "PlatformDetector.h"
#include "Window.h"

#include "DirectX11VertexBuffer.h"
#include "DirectX11IndexBuffer.h"

namespace Engine
{

	static bool CreateDeviceAndSwapChain(
		const HWND& window, std::uint32_t width, std::uint32_t height,
		ID3D11Device** device, ID3D11DeviceContext** context, IDXGISwapChain** swapChain)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = static_cast<UINT>(width);
		swapChainDesc.BufferDesc.Height = static_cast<UINT>(height);
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
			& inFeatureLevels,
			1,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			swapChain,
			device,
			&outFeatureLevel,
			context);

		DebugAssert(result == S_OK, "Failed to create the device and swap chain.");
		return result == S_OK;
	}


	static bool CreateBackBuffer(IDXGISwapChain* swapChain,
		ID3D11Device* device, ID3D11RenderTargetView** renderTargetView)
	{
		ID3D11Texture2D* backBuffer;
		HRESULT result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			(LPVOID*)&backBuffer);
		DebugAssert(result == S_OK, "Failed to create the back buffer.");
		result = device->CreateRenderTargetView(backBuffer,
			nullptr, renderTargetView);
		DebugAssert(result == S_OK, "Failed to set the render target view.");
		backBuffer->Release();
		return result == S_OK;
	}

	static bool CreateSamplerState(ID3D11Device* device,
		ID3D11DeviceContext* context, ID3D11SamplerState** samplerState,
		UINT startSlot)
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

		HRESULT output = device->CreateSamplerState(
			&samplerDesc, samplerState);
		DebugAssert(output == S_OK, "Something went wrong with creating sampler.");
		context->PSSetSamplers(startSlot, 1, samplerState);
		return output == S_OK;
	}

	DirectX11RenderingAPI::DirectX11RenderingAPI()
		: m_swapChain(nullptr),
		m_deviceContext(nullptr),
		m_device(nullptr),
		m_backBufferRenderTargetView(nullptr),
		m_currentRenderTargetView(nullptr),
		m_samplerState(nullptr),
		m_width(0), m_height(0),
		m_clearColor(0.0f, 0.0f, 1.0f, 1.0f)
	{
	}

	DirectX11RenderingAPI::~DirectX11RenderingAPI()
	{
#if _DEBUG
		ID3D11Debug* debug = nullptr;
		{
			HRESULT result = m_device->QueryInterface(__uuidof(ID3D11Debug),
				reinterpret_cast<void**>(&debug));
			DebugAssert(result == S_OK, "Unable to create debug device.");
		}
#endif
		m_currentRenderTargetView = nullptr;
		m_samplerState->Release();
		m_backBufferRenderTargetView->Release();
		m_swapChain->Release();
		m_device->Release();
		m_deviceContext->Release();

#ifdef _DEBUG
		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
		debug->Release();
#endif
	}
	
	bool DirectX11RenderingAPI::Initialize(Window* window)
	{
		m_width = window->GetWidth();
		m_height = window->GetHeight();

		HWND hwnd = window->GetHWND();
		if (!CreateDeviceAndSwapChain(hwnd,
			window->GetWidth(), window->GetHeight(), &m_device,
			&m_deviceContext, &m_swapChain))
		{
			return false;
		}

		SetViewport(0.0f, 0.0f, m_width, m_height);

		if (!CreateBackBuffer(m_swapChain, m_device,
			&m_backBufferRenderTargetView))
		{
			return false;
		}

		if (!CreateSamplerState(m_device,
			m_deviceContext, &m_samplerState, 0))
		{
			return false;
		}
		SetRenderTarget(m_backBufferRenderTargetView, nullptr);
		m_deviceContext->IASetPrimitiveTopology(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return true;
	}
	
	void DirectX11RenderingAPI::SetViewport(float x, float y, float width, float height)
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
	
	void DirectX11RenderingAPI::SetResolution(std::uint32_t width, std::uint32_t height)
	{
		if (m_width == width
			&& m_height == height)
		{
			return;
		}
		m_width = width;
		m_height = height;

		DXGI_SWAP_CHAIN_DESC scDesc;
		HRESULT res = m_swapChain->GetDesc(&scDesc);
		DebugAssert(res == S_OK, "Failed to get the descriptor flags.");

		DXGI_MODE_DESC desc;
		std::memcpy(&desc, &(scDesc.BufferDesc), sizeof(DXGI_MODE_DESC));
		desc.Width = width;
		desc.Height = height;
		m_swapChain->ResizeTarget(&desc);
	}

	void DirectX11RenderingAPI::SetRenderTarget(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView)
	{
		m_currentRenderTargetView = renderTargetView;
		m_deviceContext->OMSetRenderTargets(1, &m_currentRenderTargetView, depthStencilView);
	}

	void DirectX11RenderingAPI::SetClearColor(const MathLib::Vector4& clearColor)
	{
		m_clearColor = clearColor;
	}

	void DirectX11RenderingAPI::Clear()
	{
		m_deviceContext->ClearRenderTargetView(m_currentRenderTargetView,
			reinterpret_cast<const float*>(&m_clearColor));
	}

	void DirectX11RenderingAPI::Draw(VertexBuffer* vertexbuffer, IndexBuffer* indexBuffer)
	{
		DirectX11VertexBuffer* vBuffer = dynamic_cast<DirectX11VertexBuffer*>(
			vertexbuffer);
		DirectX11IndexBuffer* iBuffer = dynamic_cast<DirectX11IndexBuffer*>(
			indexBuffer);

		vBuffer->Bind();
		if (iBuffer != nullptr)
		{
			iBuffer->Bind();
			m_deviceContext->DrawIndexed(
				iBuffer->GetNumIndices(), 0, 0);
		}
		else
		{
			m_deviceContext->Draw(
				vBuffer->GetNumVerts(), 0);
		}
	}

	void DirectX11RenderingAPI::SwapBuffers()
	{
		m_swapChain->Present(1, 0);
	}
	
	std::uint32_t DirectX11RenderingAPI::GetWidth() const
	{
		return m_width;
	}
	
	std::uint32_t DirectX11RenderingAPI::GetHeight() const
	{
		return m_height;
	}
}