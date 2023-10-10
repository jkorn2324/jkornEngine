#include "EnginePCH.h"
#include "PlatformImGuiLayer.h"

#include "RenderingAPI.h"
#include "Window.h"

#include "GlfwImGuiLayer.h"

#if defined(GRAPHICS_API_DIRECTX11)
#include "DirectX11ImGuiLayer.h"
#endif

namespace Engine
{

	std::unique_ptr<PlatformImGuiLayer> PlatformImGuiLayer::Create(const LayerType& type)
	{
		switch (GetPlatformImGuiLayer(type))
		{
#if defined(GRAPHICS_API_DIRECTX11)
		case ImGuiLayerPlatform::PLATFORM_GRAPHICS_DIRECTX11: return std::make_unique<DirectX11ImGuiLayer>();
#endif
		case ImGuiLayerPlatform::PLATFORM_WINDOW_GLFW: return std::make_unique<GlfwImGuiLayer>();
		}
        JKORN_ENGINE_ASSERT(false, "Editor Layer doesn't have a platform.");
		return nullptr;
	}

	PlatformImGuiLayer::ImGuiLayerPlatform PlatformImGuiLayer::GetPlatformImGuiLayer(const LayerType& type)
	{
		if (type == LayerType::TYPE_GRAPHICS)
		{
			switch (RenderingAPI::GetRenderingAPIType())
			{
#if defined(GRAPHICS_API_DIRECTX11)
			case RenderingAPIType::DIRECTX11:	return ImGuiLayerPlatform::PLATFORM_GRAPHICS_DIRECTX11;
#endif
			}
		}
		else
		{
			switch (Window::GetWindowType())
			{
			case WindowType::GLFW_WINDOW: return ImGuiLayerPlatform::PLATFORM_WINDOW_GLFW;
			}
		}
        JKORN_ENGINE_ASSERT(false, "Platform Layer is not supported.");
		return ImGuiLayerPlatform::NONE;
	}
}
