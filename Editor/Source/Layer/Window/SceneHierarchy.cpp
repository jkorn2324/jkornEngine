#include "EditorPCH.h"

#include "SceneHierarchy.h"
#include "Application.h"
#include "EditorUtils.h"

#include <imgui.h>
#include <sstream>

#include "EditorUtils.h"

namespace Editor
{

	enum EntityCreationType
	{
		TYPE_EMPTY
	};

	static void CreateEntity(EntityCreationType type)
	{
		Engine::Scene& scene = Engine::SceneManager::GetActiveScene();

		switch (type)
		{
			case TYPE_EMPTY:
			{
				EditorSelection::SetSelectedEntity(scene.CreateEntity());
				break;
			}
		}
	}

	SceneHierarchy::SceneHierarchy()
		: m_open(true)
	{ 
	}

	SceneHierarchy::~SceneHierarchy()
	{
	}
	
	void SceneHierarchy::OnEvent(Engine::Event& event)
	{
		Engine::EventDispatcher eventDispatcher(event);
		eventDispatcher.Invoke<Engine::EntityDestroyedEvent>(
			BIND_EVENT_FUNCTION(SceneHierarchy::OnEntityDestroyed));
	}

	void SceneHierarchy::Draw()
	{
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;
		if (!ImGui::Begin("Scene Hierarchy", &m_open, windowFlags))
		{
			ImGui::End();
			return;
		}

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left)
			&& ImGui::IsWindowHovered())
		{
			EditorSelection::SetSelectedEntity(Engine::Entity());
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Create Entity"))
			{
				if (ImGui::MenuItem("Empty"))
				{
					CreateEntity(TYPE_EMPTY);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Entities Display
		{
			// Scene Name Tree Node.
			std::string sceneName;
			Engine::WStringToString(Engine::SceneManager::GetActiveScene().GetSceneName(),
				sceneName);

			if (ImGui::TreeNode(sceneName.c_str()))
			{
				std::vector<Engine::Entity>& entities = EditorSceneManager::GetEntities();
				for (uint32_t i = 0; i < entities.size(); i++)
				{
					DrawEntity(entities[i]);
				}
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
	
	bool SceneHierarchy::OnEntityDestroyed(Engine::EntityDestroyedEvent& event)
	{
		return true;
	}
	
	void SceneHierarchy::DrawEntity(const Engine::Entity& entity)
	{
		ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

		ImGuiTreeNodeFlags nodeFlags = 0;
		if (EditorSelection::GetSelectedEntity() == entity)
		{
			nodeFlags |= ImGuiTreeNodeFlags_Selected;
		}
		else
		{
			nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
		}
		nodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		// TODO: Detect if the entity has children.
		bool hasChildren = false;
		if (!hasChildren)
		{
			nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		}

		Engine::NameComponent& nameComponent
			= entity.GetComponent<Engine::NameComponent>();
		bool isOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.GetID(), nodeFlags,
			nameComponent.name.c_str());

		if (ImGui::IsItemClicked())
		{
			EditorSelection::SetSelectedEntity(entity);
			m_selectionType = TYPE_NONE;
		}
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			EditorSelection::SetSelectedEntity(entity);
			m_selectionType = TYPE_VIEW_POPUP;
		}

		bool deleted = false;
		switch (m_selectionType)
		{
		case TYPE_VIEW_POPUP:
		{
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete"))
					deleted = true;

				ImGui::EndPopup();
			}
			break;
		}
		}

		if (deleted)
		{
			Engine::SceneManager::GetActiveScene().DestroyEntity(
				EditorSelection::GetSelectedEntity());
		}
#if 0
		// TODO: Draw Children.
		if (isOpened && hasChildren)
		{
		}
#endif
		ImGui::Unindent();
	}
}