#pragma once

#include "PlatformImGuiLayer.h"

namespace Engine
{

	class DirectX11ImGuiLayer : public PlatformImGuiLayer
	{
	public:
		void OnLayerAdded() override;
		void OnShutdown() override;

		void BeginFrame() override;
		void EndFrame() override;
	};
}