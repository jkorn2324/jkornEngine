#pragma once

#include "PlatformEditorLayer.h"

namespace Engine
{

	class DirectX11EditorLayer : public PlatformEditorLayer
	{
	public:
		void OnLayerAdded() override;
		void OnShutdown() override;

		void BeginFrame() override;
		void EndFrame() override;
	};
}