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
			T& component = entity.GetComponent<T>();
			func(component);
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


	static void DrawEntity(const Engine::Entity& entity)
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

				component.name = nameOutput;
			});

		// Draw Transform 3D Component.
		DrawComponent<Engine::Transform3DComponent>(entity,
			[=](Engine::Transform3DComponent& component) -> void
			{
				ImGui::BeginGroup();

				MathLib::Vector3 pos = component.GetPosition();
				if (ImGui::InputFloat3("Position", reinterpret_cast<float*>(&pos), "%.3f",
					ImGuiInputTextFlags_EnterReturnsTrue))
				{
					component.SetPosition(pos);
				}

				MathLib::Quaternion quaternion = component.GetRotation();
				MathLib::Vector3 eulers = quaternion.ToEuler(true);
				if (ImGui::InputFloat3("Rotation", reinterpret_cast<float*>(&eulers), "%.3f",
					ImGuiInputTextFlags_EnterReturnsTrue))
				{
					MathLib::Quaternion quaternionFromEulers = MathLib::Quaternion::FromEuler(eulers, true);
					if (quaternionFromEulers != quaternion)
					{
						component.SetRotation(quaternionFromEulers);
					}
				}

				MathLib::Vector3 scale = component.GetScale();
				if (ImGui::InputFloat3("Scale", reinterpret_cast<float*>(&scale), "%.3f",
					ImGuiInputTextFlags_EnterReturnsTrue))
				{
					component.SetScale(scale);
				}

				ImGui::EndGroup();
			});

		// Draw Transform 2D Component.
		DrawComponent<Engine::Transform2DComponent>(entity,
			[=](Engine::Transform2DComponent& component) -> void
			{
				ImGui::BeginGroup();

				MathLib::Vector2 pos = component.GetPosition();
				ImGui::InputFloat2("Position", reinterpret_cast<float*>(&pos));
				component.SetPosition(pos);

				float rotation = component.GetRotation(true);
				ImGui::InputFloat("Rotation", reinterpret_cast<float*>(&rotation));
				component.SetRotation(rotation, true);

				MathLib::Vector2 scale = component.GetScale();
				ImGui::InputFloat2("Scale", reinterpret_cast<float*>(&scale));
				component.SetScale(scale);

				ImGui::EndGroup();
			});

		// Draw Sprite Component.
		DrawComponent<Engine::SpriteComponent>(entity,
			[=](Engine::SpriteComponent& component) -> void
			{
				ImGui::BeginGroup();

				ImGui::Checkbox("Enabled", &component.enabled);
				ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&component.color));

				ImGui::EndGroup();
			});


		DrawComponent<Engine::SceneCameraComponent>(entity,
			[=](Engine::SceneCameraComponent& component) -> void
			{
				ImGui::BeginGroup();

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

				switch (camera.GetSceneCameraType())
				{
				case Engine::TYPE_PERSPECTIVE:
				{
					ImGui::InputFloat("FOV", &cameraProperties.perspFOV);
					ImGui::InputFloat("Aspect Ratio", &cameraProperties.perspAspectRatio);
					break;
				}
				case Engine::TYPE_ORTHOGRAPHIC:
				{
					ImGui::InputFloat("Width", &cameraProperties.orthoWidth);
					ImGui::InputFloat("Height", &cameraProperties.orthoHeight);
					break;
				}
				}

				ImGui::EndGroup();
			});
	}


	EntityInspector::EntityInspector()
		: m_open(true)
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
						DrawComponentInMenu<Engine::SceneCameraComponent>("Camera", entity);

						ImGui::EndMenu();
					}
					ImGui::EndMenuBar();
				}
			}

			DrawEntity(entity);
		}
		ImGui::End();
	}
}