#include "EditorPCH.h"

#include "SceneHierarchy.h"
#include "Application.h"
#include "EditorUtils.h"

#include <imgui.h>
#include <sstream>

#include "EditorUtils.h"

namespace Editor
{

	static const char* ENTITY_SELECTION_DRAG_DROP = "_sceneHierarchyEntitySelection";

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
		if (!m_open)
		{
			return;
		}
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;
		ImGui::Begin("Scene Hierarchy", &m_open, windowFlags);

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

			bool sceneNodeOpened = ImGui::TreeNode(sceneName.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* dragAndDropPayload
					= ImGui::AcceptDragDropPayload(ENTITY_SELECTION_DRAG_DROP))
				{
					Engine::Entity* selectedEntityNode = (Engine::Entity*)dragAndDropPayload->Data;
					if (selectedEntityNode != nullptr)
					{
						Engine::EntityHierarchyComponent& hierarchy
							= selectedEntityNode->GetComponent<Engine::EntityHierarchyComponent>();
						hierarchy.SetParent(Engine::Entity::None);
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (sceneNodeOpened)
			{
				const std::vector<Engine::Entity>& entities = 
					Engine::SceneManager::GetActiveScene().GetRootEntities();
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

		bool isEntitySameAsSelection = EditorSelection::GetSelectedEntity()
			== entity;
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
		if (isEntitySameAsSelection)
		{
			nodeFlags |= ImGuiTreeNodeFlags_Selected;
		}
		nodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		Engine::EntityHierarchyComponent& ehc
			= entity.GetComponent<Engine::EntityHierarchyComponent>();
		if (!ehc.HasChildren())
		{
			nodeFlags |= ImGuiTreeNodeFlags_Leaf;
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

		if (!isEntitySameAsSelection
			&& ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* dragAndDropPayload
				= ImGui::AcceptDragDropPayload(ENTITY_SELECTION_DRAG_DROP))
			{
				Engine::Entity* selectedEntityNode = (Engine::Entity*)dragAndDropPayload->Data;
				if (selectedEntityNode != nullptr
					&& *selectedEntityNode != entity)
				{
					Engine::EntityHierarchyComponent& hierarchy
						= selectedEntityNode->GetComponent<Engine::EntityHierarchyComponent>();
					hierarchy.SetParent(entity);
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Drag and Drop Source
		if (isEntitySameAsSelection
			&& ImGui::BeginDragDropSource())
		{
			void* selectedEntityNode = &EditorSelection::GetSelectedEntity();
			ImGui::SetDragDropPayload(ENTITY_SELECTION_DRAG_DROP, selectedEntityNode,
				sizeof(Engine::Entity));
			ImGui::Text(nameComponent.name.c_str());
			ImGui::EndDragDropSource();
		}

		if (isOpened)
		{
			if (ehc.HasChildren())
			{
				auto children = ehc.GetChildren();
				for (int i = 0; i < children.size(); i++)
				{
					DrawEntity(children[i]);
				}
			}
			ImGui::TreePop();
		}

		ImGui::Unindent();

		if (deleted)
		{
			Engine::SceneManager::GetActiveScene().DestroyEntity(
				entity);
		}
	}
}