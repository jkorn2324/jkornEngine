#include "EnginePCH.h"
#include "GraphicsRenderer.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "RenderingAPI.h"

#include "Application.h"
#include "Window.h"

#include "Source\Vector.h"

namespace Engine
{
	RenderingAPI* GraphicsRenderer::s_renderingAPI = nullptr;

	bool GraphicsRenderer::Init()
	{
		DebugAssert(s_renderingAPI == nullptr, 
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

	void GraphicsRenderer::Draw(VertexBuffer* vBuffer,
		IndexBuffer* iBuffer)
	{
		DebugAssert(s_renderingAPI != nullptr, "Rendering API isn't initialized.");
		s_renderingAPI->Draw(vBuffer, iBuffer);
	}

	void GraphicsRenderer::SwapBuffers()
	{
		s_renderingAPI->SwapBuffers();
	}

	RenderingAPI& GraphicsRenderer::GetRenderingAPI()
	{
		DebugAssert(s_renderingAPI != nullptr, "RenderingAPI wasn't initialized.");
		return *s_renderingAPI;
	}
}