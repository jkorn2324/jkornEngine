#include "EditorPCH.h"
#include "EditorLayer.h"

#include <imgui.h>

namespace Editor
{


	static void DrawSceneHierarchy()
	{
		ImGui::ShowDemoWindow();
	}

	EditorLayer::EditorLayer()
		: Layer("Editor")
	{
	}

	EditorLayer::~EditorLayer()
	{
		// TODO: Implementation
	}

	void EditorLayer::OnLayerAdded()
	{
		// TODO: Implementation
	}

	void EditorLayer::OnLayerRemoved()
	{
		// TODO: Implementation
	}

	void EditorLayer::OnUpdate(const Engine::Timestep& timestep)
	{
		// TODO: Implementation
	}

	void EditorLayer::OnImGuiRender()
	{
		// TODO: Implementation
	}
}