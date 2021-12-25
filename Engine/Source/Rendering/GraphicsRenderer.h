#pragma once

#include <d3d11.h>

namespace MathLib
{
	class Vector4;
}

namespace Engine
{
	class GraphicsRenderer
	{

#pragma region members

	public:
		explicit GraphicsRenderer();
		~GraphicsRenderer();

		bool Initialize(const HWND& window);

		void BeginFrame();
		void EndFrame();

		void SetRenderTarget(ID3D11RenderTargetView* currentRenderTarget);
		void SetViewport(float x, float y, float width, float height);

		void SetShader(class Shader* shader);

		// Sets the active index buffer.
		void SetActiveIndexBuffer(class IndexBuffer* indexBuffer);
		// Sets the active vertex buffer.
		void SetActiveVertexBuffer(class VertexBuffer* vertexBuffer);

		// Sets the active constant buffer.
		void SetConstantBuffer(const std::size_t& slot, class ConstantBuffer* constantBuffer);
		// Sets the acive constant buffer.
		void SetConstantBuffer(const std::size_t& slot, class ConstantBuffer* constantBuffer, int flags);

		// Draws the active elements.
		void DrawActiveElements();

	private:
		void ClearRenderTarget(const MathLib::Vector4& color);

		bool CreateBackBuffer();
		bool CreateDeviceAndSwapChain(const HWND& window, 
			std::uint32_t resolutionWidth, std::uint32_t resolutionHeight);

	private:
		IDXGISwapChain* m_swapChain;
		ID3D11DeviceContext* m_deviceContext;
		ID3D11Device* m_device;

		// TODO: Need to set the depth texture + sampler state shennanigans

		ID3D11RenderTargetView* m_backBufferRenderTarget;
		ID3D11RenderTargetView* m_currentRenderTarget;

		class VertexBuffer* m_activeVertexBuffer;
		class IndexBuffer* m_activeIndexBuffer;
		class Shader* m_activeShader;

		std::uint32_t m_screenWidth;
		std::uint32_t m_screenHeight;

#pragma endregion

#pragma region statics

	public:
		static GraphicsRenderer* Get();

	private:
		static GraphicsRenderer* s_graphicsRenderer;

#pragma endregion

		friend class VertexBuffer;
		friend class IndexBuffer;
		friend class Shader;
		friend class ConstantBuffer;
		friend class Texture;
	};
}