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

		bool Initialize(class Window* window);

		class RenderingAPI* GetRenderingAPI() const;

		void BeginFrame();
		void EndFrame();

		void SetTexture(std::uint32_t slot, class Texture* texture);
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
		class RenderingAPI* m_renderingAPI;
		class FrameBuffer* m_frameBuffer;
		class VertexBuffer* m_activeVertexBuffer;
		class IndexBuffer* m_activeIndexBuffer;
		class Shader* m_activeShader;

		friend class ConstantBuffer;

#pragma endregion

#pragma region statics

	public:
		static GraphicsRenderer* Get();

	private:
		static GraphicsRenderer* s_graphicsRenderer;

#pragma endregion
	};
}