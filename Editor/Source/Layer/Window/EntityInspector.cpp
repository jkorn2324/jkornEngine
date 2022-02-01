#include "EditorPCH.h"
#include "EntityInspector.h"

#include "EditorUtils.h"

#include <imgui.h>
#include <functional>

namespace Editor
{
	template<typename T>
	static void DrawComponent(const Engine::Entity& entity, 
		const std::function<void(T&)>& func)
	{
		if (entity.HasComponent<T>())
		{
			ImGui::BeginGroup();
			
			T& component = entity.GetComponent<T>();
			func(component);

			ImGui::EndGroup();
		}
	}

	template<typename T>
	static void DrawTreeNodeComponent(Engine::Entity& entity,
		const char* name, ComponentSelectionType& selectionType, const std::function<void(T&)>& func)
	{
		if (entity.HasComponent<T>())
		{
			ImGui::BeginGroup();

			static ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow
				| ImGuiTreeNodeFlags_SpanAvailWidth
				| ImGuiTreeNodeFlags_OpenOnArrow;
			bool open = ImGui::TreeNodeEx(name, nodeFlags);

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				selectionType = ComponentSelectionType::TYPE_POPUP;
			}

			bool deleted = false;
			// Popup based on the selection.
			switch (selectionType)
			{
			case ComponentSelectionType::TYPE_POPUP:
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete"))
					{
						deleted = true;
					}
					ImGui::EndPopup();
				}
				break;
			}
			}

			if (open)
			{
				T& component = entity.GetComponent<T>();
				func(component);
				ImGui::TreePop();
			}
			ImGui::EndGroup();

			if (deleted) entity.RemoveComponent<T>();
		}
	}

	template<typename T>
	static void DrawComponentInMenu(const char* name, Engine::Entity& entity,
		const std::function<void(T&)>& func)
	{
		if (!entity.HasComponent<T>())
		{
			if (ImGui::MenuItem(name))
			{
				T& newComponent = entity.AddComponent<T>();
				if (func != nullptr) func(newComponent);
			}
		}
	}

	template<typename T>
	static void DrawComponentInMenu(const char* name, Engine::Entity& entity)
	{
		if (!entity.HasComponent<T>())
		{
			if (ImGui::MenuItem(name))
			{
				entity.AddComponent<T>();
			}
		}
	}


	static void DrawEntity(Engine::Entity& entity, ComponentSelectionType& selectionType)
	{
		// Draw Name Component.
		DrawComponent<Engine::NameComponent>(entity,
			[=](Engine::NameComponent& component) -> void 
			{
				char nameOutput[30];
				std::memcpy(nameOutput, component.name.c_str(), 30);

				ImGui::BeginGroup();
				ImGui::InputText("Name", nameOutput, sizeof(nameOutput) / sizeof(char));
				ImGui::EndGroup();
				ImGui::Separator();

				component.name = nameOutput;
			});

		// Draw Transform 3D Component.
		DrawTreeNodeComponent<Engine::Transform3DComponent>(entity, "Transform 3D Component",  selectionType,
			[=](Engine::Transform3DComponent& component) -> void
			{
				MathLib::Vector3 pos = component.GetLocalPosition();
				if (ImGui::InputFloat3("Position", reinterpret_cast<float*>(&pos), "%.3f",
					ImGuiInputTextFlags_EnterReturnsTrue))
				{
					component.SetLocalPosition(pos);
				}

				MathLib::Quaternion quaternion = component.GetLocalRotation();
				MathLib::Vector3 eulers = quaternion.ToEuler(true);
				if (ImGui::InputFloat3("Rotation", reinterpret_cast<float*>(&eulers), "%.3f",
					ImGuiInputTextFlags_EnterReturnsTrue))
				{
					MathLib::Quaternion quaternionFromEulers = MathLib::Quaternion::FromEuler(eulers, true);
					if (quaternionFromEulers != quaternion)
					{
						component.SetLocalRotation(quaternionFromEulers);
					}
				}

				MathLib::Vector3 scale = component.GetLocalScale();
				if (ImGui::InputFloat3("Scale", reinterpret_cast<float*>(&scale), "%.3f",
					ImGuiInputTextFlags_EnterReturnsTrue))
				{
					component.SetLocalScale(scale);
				}
			});

		// Draw Transform 2D Component.
		DrawTreeNodeComponent<Engine::Transform2DComponent>(entity, "Transform 2D Component", selectionType,
			[=](Engine::Transform2DComponent& component) -> void
			{
				MathLib::Vector2 pos = component.GetLocalPosition();
				ImGui::InputFloat2("Position", reinterpret_cast<float*>(&pos));
				component.SetLocalPosition(pos);

				float rotation = component.GetLocalRotation(true);
				if (ImGui::InputFloat("Rotation", reinterpret_cast<float*>(&rotation)))
				{
					component.SetLocalRotation(rotation, true);
				}

				MathLib::Vector2 scale = component.GetLocalScale();
				ImGui::InputFloat2("Scale", reinterpret_cast<float*>(&scale));
				component.SetLocalScale(scale);
			});

		// Draw Sprite Component.
		DrawTreeNodeComponent<Engine::SpriteComponent>(entity, "Sprite Component", selectionType,
			[=](Engine::SpriteComponent& component) -> void
			{
				ImGui::Checkbox("Enabled", &component.enabled);
				ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&component.color));
			});


		DrawTreeNodeComponent<Engine::SceneCameraComponent>(entity, "Scene Camera Component", selectionType,
			[=](Engine::SceneCameraComponent& component) -> void
			{
				ImGui::Checkbox("Enabled", &component.enabled);
				ImGui::Checkbox("Main Camera", &component.mainCamera);

				Engine::SceneCamera& camera = component.camera;
				Engine::CameraProperties& cameraProperties = camera.GetProperties();

				static const char* cameraTypes[] = {"Perspective", "Orthographic"};

				int cameraType = (int)camera.GetSceneCameraType();
				if (ImGui::Combo("Type", &cameraType, cameraTypes, 2))
				{
					camera.SetSceneCameraType((Engine::SceneCameraType)cameraType);
				}
				ImGui::InputFloat("Near Plane", &cameraProperties.nearPlane);
				ImGui::InputFloat("Far Plane", &cameraProperties.farPlane);
			});

		DrawTreeNodeComponent<Engine::MeshComponent>(entity, "Mesh Component", selectionType,
			[=](Engine::MeshComponent& component) -> void
			{
				ImGui::Checkbox("Enabled", &component.enabled);
				
				// TODO: Implementation
			});
	}


	EntityInspector::EntityInspector()
		: m_open(true),
		m_selectionType(ComponentSelectionType::TYPE_NONE)
	{
	}
	
	EntityInspector::~EntityInspector()
	{
		// TODO: Implementation
	}
	
	void EntityInspector::OnEvent(Engine::Event& event)
	{
		// TODO: Implementation
	}

	void EntityInspector::Draw()
	{
		if (!m_open)
		{
			return;
		}
		ImGui::Begin("Entity Inspector", &m_open, ImGuiWindowFlags_MenuBar);

		Engine::Entity entity = EditorSelection::GetSelectedEntity();
		if (entity.IsValid())
		{
			// Component Options Menu
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("Add Component"))
					{
						if (!entity.HasComponent<Engine::Transform2DComponent>())
						{
							DrawComponentInMenu<Engine::Transform3DComponent>("Transform 3D", entity);
						}

						if (!entity.HasComponent<Engine::Transform3DComponent>())
						{
							DrawComponentInMenu<Engine::Transform2DComponent>("Transform 2D", entity);
						}

						DrawComponentInMenu<Engine::SpriteComponent>("Sprite", entity);
						DrawComponentInMenu<Engine::MeshComponent>("Mesh", entity,
							[=](Engine::MeshComponent& meshComponent) -> void
							{
								// TODO: Remove this, as this is temporary.
								Engine::AssetCache<Engine::Mesh>& meshes
									= Engine::AssetManager::GetMeshes();
								meshComponent.mesh = meshes.Get(L"DefaultCube");

								Engine::AssetCache<Engine::Material>& materials
									= Engine::AssetManager::GetMaterials();
								meshComponent.material = materials.Get(L"Unlit-ColorUV");
								Engine::MaterialConstants& constants
									= meshComponent.material->GetMaterialConstants();
								constants.SetMaterialConstant(
									"c_materialColor", MathLib::Vector4::One);
							});

						DrawComponentInMenu<Engine::SceneCameraComponent>("Camera", entity);

						ImGui::EndMenu();
					}
					ImGui::EndMenuBar();
				}
			}

			DrawEntity(entity, m_selectionType);
		}
		ImGui::End();
	}
}