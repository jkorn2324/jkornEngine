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

#include "GraphicsRenderer3D.h"

namespace Engine
{
	RenderingAPI* GraphicsRenderer::s_renderingAPI = nullptr;

	ConstantBuffer* s_cameraConstantBuffer = nullptr;
	
	bool GraphicsRenderer::Init()
	{
		DebugAssert(s_renderingAPI == nullptr, 
			"Graphics Rendering API has already been initialized.");
		s_renderingAPI = RenderingAPI::Create();
		if (!s_renderingAPI->Initialize(&Application::Get().GetWindow()))
		{
			return false;
		}

		CameraConstants placeholder;
		s_cameraConstantBuffer = ConstantBuffer::Create(&placeholder,
			sizeof(CameraConstants));
		return true;
	}

	void GraphicsRenderer::Release()
	{
		delete s_cameraConstantBuffer;
		delete s_renderingAPI;
	}

	void GraphicsRenderer::BeginScene(const CameraConstants& cameraConstants)
	{
		DebugAssert(s_cameraConstantBuffer != nullptr, 
			"Camera Constant buffer doesn't exist.");
		s_cameraConstantBuffer->SetData(&cameraConstants,
			sizeof(cameraConstants));
		s_cameraConstantBuffer->Bind(0,
			Engine::ConstantBufferFlags::VERTEX_SHADER | Engine::ConstantBufferFlags::PIXEL_SHADER);

		// Binds the lights in the 3D Graphics Renderer.
		GraphicsRenderer3D::BindLights();
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

	void GraphicsRenderer::OnWindowResized(uint32_t x, uint32_t y)
	{
		GetRenderingAPI().SetViewport(
			0.0f, 0.0f, (float)x, float(y));
		GetRenderingAPI().SetResolution(x, y);
	}
}