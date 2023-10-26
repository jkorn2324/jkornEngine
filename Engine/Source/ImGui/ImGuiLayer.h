#pragma once

#include "Layer.h"
#include "Vector.h"

#include <memory>

#include "PlatformImGuiLayer.h"

namespace Engine
{
	class IEvent;

	class ImGuiLayer : public Layer
	{

	public:
		explicit ImGuiLayer();
		~ImGuiLayer();

		void OnEvent(IEvent& event) override;

		void OnLayerAdded() override;
		void OnLayerRemoved() override;

		void BeginRender();
		void EndRender();

	private:
		bool OnWindowResized(class WindowResizedEvent& event);

	private:
        PlatformGraphicsImGuiLayer m_graphicsLayer;
        PlatformWindowImGuiLayer m_windowLayer;
	};
}
