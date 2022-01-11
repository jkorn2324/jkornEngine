#pragma once

#include "Source\Vector.h"
#include "Source\Matrix.h"

namespace Engine
{

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
		static void Draw(class VertexBuffer* vertexBuffer,
			class IndexBuffer* indexBuffer);


		static class RenderingAPI& GetRenderingAPI();

	private:
		static RenderingAPI* s_renderingAPI;
	};
}