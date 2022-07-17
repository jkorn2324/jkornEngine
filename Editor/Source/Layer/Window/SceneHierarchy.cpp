#include "EditorPCH.h"

#include "SceneHierarchy.h"
#include "Application.h"
#include "EditorSceneManager.h"
#include "EditorSelection.h"

#include <imgui.h>
#include <sstream>

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
				Engine::Entity duplicatedEntity;
				for (uint32_t i = 0; i < entities.size(); i++)
				{
					DrawEntity((Engine::Entity&)entities[i], duplicatedEntity);
				}
				ImGui::TreePop();

				if (duplicatedEntity.IsValid())
				{
					Engine::Entity createdEntity =
						Engine::SceneManager::GetActiveScene().CreateEntity(
							duplicatedEntity.GetComponent<Engine::NameComponent>().name + " (Clone)");
					Engine::CopyEntity(duplicatedEntity, createdEntity, false);
				}
			}
		}
		ImGui::End();
	}
	
	bool SceneHierarchy::OnEntityDestroyed(Engine::EntityDestroyedEvent& event)
	{
		return true;
	}
	
	void SceneHierarchy::DrawEntity(Engine::Entity& entity, Engine::Entity& duplicatedEntity)
	{
		enum EntitySelectionMode
		{
			None,
			Delete,
			Duplicate
		};

		if (!entity.IsValid())
		{
			return;
		}

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

		EntitySelectionMode selectionMode = None;
		switch (m_selectionType)
		{
		case TYPE_VIEW_POPUP:
		{
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete"))
					selectionMode = Delete;
				if (ImGui::MenuItem("Duplicate"))
					selectionMode = Duplicate;

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
				for (size_t i = 0; i < children.size(); i++)
				{
					DrawEntity(children[i], duplicatedEntity);
				}
			}
			ImGui::TreePop();
		}

		ImGui::Unindent();

		switch (selectionMode)
		{
		case Delete:
			Engine::SceneManager::GetActiveScene().DestroyEntity(
				entity);
			break;
		case Duplicate:
		{
			duplicatedEntity = entity;
			break;
		}
		}
	}
}