#include "EnginePCH.h"
#include "PlatformImGuiLayer.h"

#include "RenderingAPI.h"
#include "Window.h"

#include "DirectX11ImGuiLayer.h"
#include "GlfwImGuiLayer.h"

namespace Engine
{

	std::unique_ptr<PlatformImGuiLayer> PlatformImGuiLayer::Create(const LayerType& type)
	{
		switch (GetPlatformImGuiLayer(type))
		{
		case ImGuiLayerPlatform::PLATFORM_GRAPHICS_DIRECTX11: return std::make_unique<DirectX11ImGuiLayer>();
		case ImGuiLayerPlatform::PLATFORM_WINDOW_GLFW: return std::make_unique<GlfwImGuiLayer>();
		}
		DebugAssert(false, "Editor Layer doesn't have a platform.");
		return nullptr;
	}

	PlatformImGuiLayer::ImGuiLayerPlatform PlatformImGuiLayer::GetPlatformImGuiLayer(const LayerType& type)
	{
		if (type == LayerType::TYPE_GRAPHICS)
		{
			switch (RenderingAPI::GetRenderingAPIType())
			{
			case RenderingAPIType::DIRECTX11:	return ImGuiLayerPlatform::PLATFORM_GRAPHICS_DIRECTX11;
			}
		}
		else
		{
			switch (Window::GetWindowType())
			{
			case WindowType::GLFW_WINDOWS_WINDOW: return ImGuiLayerPlatform::PLATFORM_WINDOW_GLFW;
			}
		}
		DebugAssert(false, "Platform Layer is not supported.");
		return ImGuiLayerPlatform::NONE;
	}
}