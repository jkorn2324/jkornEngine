#pragma once


namespace Engine
{

	class GlfwWindowEditorLayer : public PlatformEditorLayer
	{
	public:
		void OnLayerAdded() override;
		void OnShutdown() override;

		void BeginFrame() override;
		void EndFrame() override;
	};
}