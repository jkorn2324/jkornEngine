#pragma once

#include "Layer.h"

#include <memory>

namespace Engine
{

	class PlatformImGuiLayer;

	class ImGuiLayer : public Layer
	{

	public:
		explicit ImGuiLayer();
		~ImGuiLayer();

		void OnLayerAdded() override;
		void OnLayerRemoved() override;

		void BeginRender();
		void EndRender();

	private:
		std::unique_ptr<PlatformImGuiLayer> m_windowImGuiLayer;
		std::unique_ptr<PlatformImGuiLayer> m_graphicsImGuiLayer;
	};
}