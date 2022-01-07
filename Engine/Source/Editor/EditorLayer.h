#pragma once

#include "Layer.h"

#include <memory>

namespace Engine
{

	class PlatformEditorLayer;

	class EditorLayer : public Layer
	{

	public:
		explicit EditorLayer();
		~EditorLayer();

		void OnLayerAdded() override;
		void OnLayerRemoved() override;

		void BeginRender();
		void EndRender();

	private:
		std::unique_ptr<PlatformEditorLayer> m_windowEditorLayer;
		std::unique_ptr<PlatformEditorLayer> m_graphicsEditorLayer;
	};
}