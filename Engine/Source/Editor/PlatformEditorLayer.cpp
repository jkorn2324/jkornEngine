#include "EnginePCH.h"
#include "PlatformEditorLayer.h"

#include "RenderingAPI.h"
#include "Window.h"

#include "DirectX11EditorLayer.h"
#include "GlfwWindowEditorLayer.h"

namespace Engine
{

	std::unique_ptr<PlatformEditorLayer> PlatformEditorLayer::Create(const LayerType& type)
	{
		switch (GetPlatformEditorLayer(type))
		{
		case EditorLayerPlatform::PLATFORM_GRAPHICS_DIRECTX11: return std::make_unique<DirectX11EditorLayer>();
		case EditorLayerPlatform::PLATFORM_WINDOW_GLFW: return std::make_unique<GlfwWindowEditorLayer>();
		}
		DebugAssert(false, "Editor Layer doesn't have a platform.");
		return nullptr;
	}

	PlatformEditorLayer::EditorLayerPlatform PlatformEditorLayer::GetPlatformEditorLayer(const LayerType& type)
	{
		if (type == LayerType::TYPE_GRAPHICS)
		{
			switch (RenderingAPI::GetRenderingAPIType())
			{
			case RenderingAPIType::DIRECTX11:	return EditorLayerPlatform::PLATFORM_GRAPHICS_DIRECTX11;
			}
		}
		else
		{
			switch (Window::GetWindowType())
			{
			case WindowType::GLFW_WINDOWS_WINDOW: return EditorLayerPlatform::PLATFORM_WINDOW_GLFW;
			}
		}
		DebugAssert(false, "Platform Layer is not supported.");
		return EditorLayerPlatform::NONE;
	}
}