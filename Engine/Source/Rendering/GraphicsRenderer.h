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

	public:
		static bool Init();
		static void Release();

		static void SwapBuffers();
		static void Draw(class VertexBuffer* vertexBuffer,
			class IndexBuffer* indexBuffer);


		static class RenderingAPI& GetRenderingAPI();

	private:
		static RenderingAPI* s_renderingAPI;
	};
}