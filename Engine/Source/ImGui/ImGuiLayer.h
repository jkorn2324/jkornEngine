#pragma once

#include "Layer.h"
#include "Source\Vector.h"

#include <memory>

namespace Engine
{

	class PlatformImGuiLayer;
	class Event;

	class ImGuiLayer : public Layer
	{

	public:
		explicit ImGuiLayer();
		~ImGuiLayer();

		void OnEvent(Event& event) override;

		void OnLayerAdded() override;
		void OnLayerRemoved() override;

		void BeginRender();
		void EndRender();

	private:
		bool OnWindowResized(class WindowResizedEvent& event);

	private:
		std::unique_ptr<PlatformImGuiLayer> m_windowImGuiLayer;
		std::unique_ptr<PlatformImGuiLayer> m_graphicsImGuiLayer;
	};
}