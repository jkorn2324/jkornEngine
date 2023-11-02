#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace Engine
{
	class RenderingAPI;

	class GraphicsRenderer
	{

	public:
		static bool Init();
		static void Release();

		static void Present();

		static void Draw(class VertexArray* vertexArray);
		static void Draw(class VertexBuffer* vertexBuffer,
			class IndexBuffer* indexBuffer);

		static RenderingAPI& GetRenderingAPI();

	private:
		static void OnWindowResized(uint32_t x, uint32_t y);

	private:
		static RenderingAPI* s_renderingAPI;

		friend class Application;
	};
}
