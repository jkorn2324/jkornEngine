#pragma once

#include <memory>

namespace Engine
{

	enum LayerType
	{
		TYPE_NONE,
		TYPE_WINDOW,
		TYPE_GRAPHICS
	};

	class PlatformEditorLayer
	{
	private:
		enum class EditorLayerPlatform
		{
			NONE,

			PLATFORM_WINDOW_GLFW,
			PLATFORM_WINDOW_WIN32,

			PLATFORM_GRAPHICS_DIRECTX11
		};

	public:
		virtual void OnLayerAdded() =0;
		virtual void OnShutdown() =0;

		virtual void BeginFrame() =0;
		virtual void EndFrame() =0;

	public:
		static std::unique_ptr<PlatformEditorLayer> Create(const LayerType& type);
		
	private:
		static EditorLayerPlatform GetPlatformEditorLayer(const LayerType& type);
	};
}