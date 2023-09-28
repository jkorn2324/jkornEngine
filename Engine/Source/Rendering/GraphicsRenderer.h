#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace Engine
{
	class RenderingAPI;

	struct CameraConstants
	{
		MathLib::Matrix4x4 c_viewProjection;
		MathLib::Vector3 c_cameraPosition;

	private:
		float pad;
	};

	class GraphicsRenderer
	{

	public:
		static bool Init();
		static void Release();

		static void BeginScene(const CameraConstants& cameraConstants);
		static void EndScene() { }

		static void SwapBuffers();

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
