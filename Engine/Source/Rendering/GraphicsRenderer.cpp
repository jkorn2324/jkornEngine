#include "EnginePCH.h"
#include "GraphicsRenderer.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "RenderingAPI.h"
#include "Profiler.h"
#include "Application.h"
#include "Window.h"

namespace Engine
{
	RenderingAPI* GraphicsRenderer::s_renderingAPI = nullptr;
	
	bool GraphicsRenderer::Init()
	{
		PROFILE_SCOPE(Init, GraphicsRenderer);

        JKORN_ENGINE_ASSERT(s_renderingAPI == nullptr,
			"Graphics Rendering API has already been initialized.");
		s_renderingAPI = RenderingAPI::Create();
		if (!s_renderingAPI->Initialize(&Application::Get().GetWindow()))
		{
			return false;
		}
		return true;
	}

	void GraphicsRenderer::Release()
	{
		delete s_renderingAPI;
	}

	void GraphicsRenderer::Draw(VertexArray* vertexArray)
	{
		JKORN_ENGINE_ASSERT(s_renderingAPI != nullptr, "Rendering API isn't initialized.");
		s_renderingAPI->Draw(vertexArray);
	}

	void GraphicsRenderer::Draw(VertexBuffer* vBuffer,
		IndexBuffer* iBuffer)
	{
		GetRenderingAPI().Draw(vBuffer, iBuffer);
	}

	void GraphicsRenderer::Present()
	{
		GetRenderingAPI().Present();
	}

	RenderingAPI& GraphicsRenderer::GetRenderingAPI()
	{
        JKORN_ENGINE_ASSERT(s_renderingAPI != nullptr, "RenderingAPI wasn't initialized.");
		return *s_renderingAPI;
	}

	void GraphicsRenderer::OnWindowResized(uint32_t x, uint32_t y)
	{
		GetRenderingAPI().SetViewport(
			0.0f, 0.0f, (float)x, float(y));
		GetRenderingAPI().SetResolution(x, y);
	}
}
