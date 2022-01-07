#pragma once

#include "PlatformImGuiLayer.h"

namespace Engine
{

	class GlfwImGuiLayer : public PlatformImGuiLayer
	{
	public:
		void OnLayerAdded() override;
		void OnShutdown() override;

		void BeginFrame() override;
		void EndFrame() override;
	};
}