#pragma once

#include "Vector.h"
#include "RenderingAPI.h"

struct IDXGISwapChain;
struct ID3D11DeviceContext;
struct ID3D11Device;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11SamplerState;
struct ID3D11RasterizerState;

namespace Engine
{

	class DirectX11RenderingAPI : public RenderingAPI
	{
	public:
		explicit DirectX11RenderingAPI();
		~DirectX11RenderingAPI();

		bool Initialize(class Window* window) override;
		void SetViewport(float x, float y, float width, float height) override;
		void SetResolution(std::uint32_t width, std::uint32_t height) override;

		void SetRenderTarget(ID3D11RenderTargetView* renderTargetView,
			ID3D11DepthStencilView* depthStencilView);
		void SetRenderTargets(uint32_t numRenderTargets, ID3D11RenderTargetView** renderTargetViews,
			ID3D11DepthStencilView* depthStencilView);

		void SetClearColor(const MathLib::Vector4& clearColor) override;
		void Clear() override;

		void ClearTexture(uint32_t slot) override;

		void Draw(class VertexArray* vertexArray) override;
		void Draw(class VertexBuffer* buffer, 
			class IndexBuffer* indexBuffer = nullptr) override;
		void SwapBuffers() override;

		std::uint32_t GetWidth() const override;
		std::uint32_t GetHeight() const override;

		bool IsWireframe() const override;
		void SetWireframe(bool wireframeMode) override;

	private:
		IDXGISwapChain* m_swapChain;
		ID3D11DeviceContext* m_deviceContext;
		ID3D11Device* m_device;

		ID3D11RenderTargetView** m_currentRenderTargetViews;
		ID3D11RenderTargetView* m_backBufferRenderTargetView;

		ID3D11SamplerState* m_samplerState;

		ID3D11RasterizerState* m_defaultRasterizerState;
		ID3D11RasterizerState* m_wireframeRasterizerState;

		MathLib::Vector4 m_clearColor;

		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_numRenderTargetViews;

		bool m_wireframeMode;

		friend class DirectX11VertexBuffer;
		friend class DirectX11IndexBuffer;
		friend class DirectX11FrameBuffer;
		friend class DirectX11VertexArray;
		friend class DirectX11Texture;
		friend class DirectX11Texture2D;
		friend class DirectX11ConstantBuffer;
		friend class DirectX11Shader;
		friend class DirectX11ImGuiLayer;
		friend class DirectX11ComputeShader;
		friend class DirectX11RenderTexture;
		friend class DirectX11Utils;
	};
}