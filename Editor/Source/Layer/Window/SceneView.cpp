#include "EditorPCH.h"

#include "SceneView.h"
#include "EditorSceneManager.h"
#include "EditorSelection.h"
#include "PlatformInput.h"
#include "GraphicsUtility.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

namespace Editor
{
	SceneView::SceneView()
		: m_frameBuffer(nullptr),
		m_open(true),
		m_focused(false),
		m_prevMousePos(),
		m_currMousePos(),
		m_mouseScroll(),
		m_windowSize(),
		m_windowPosition(),
		m_windowBarSpacing(0.0f),
		m_transformationWidget(),
		m_sceneViewEntityIDs()
	{
		m_transformationWidget.SetEnabled(false);

		Engine::FrameBufferDepthStencilContext depthStencil = { Engine::DepthFormat_D24UNorm_S8UInt };
		Engine::FrameBufferSpecification editorSpecifications ({
			{ depthStencil },
			{ 				
				// The Main Color Texture
				Engine::FrameBufferAttachment { Engine::ColorFormat_RGBA32_Float },
				// The Entity ID Texture
				Engine::FrameBufferAttachment { Engine::ColorFormat_SInt32 }
			}
		});

		Engine::Application& app = Engine::Application::Get();
		editorSpecifications.width = app.GetWindow().GetWidth();
		editorSpecifications.height = app.GetWindow().GetHeight();
		m_frameBuffer = Engine::FrameBuffer::Create(editorSpecifications);

		m_windowSize.x = (float)editorSpecifications.width;
		m_windowSize.y = (float)editorSpecifications.height;
	}

	SceneView::~SceneView()
	{
		delete m_frameBuffer;
	}

	void SceneView::OnEvent(Engine::IEvent& event)
	{
		// TODO: Implementation
	}

	void SceneView::OnUpdate(const Engine::Timestep& ts)
	{
		if (!m_open)
		{
			return;
		}

		UpdateTransformationWidget(ts);
		// Update the entity widget if entity IDs change.
		UpdateEntityIDs(ts);
		UpdateMousePosition(ts);

		if (!m_focused)
		{
			return;
		}
		HandleCameraInput(ts);
	}

	void SceneView::UpdateMousePosition(const Engine::Timestep& ts)
	{
        ImVec2 mousePos = ImGui::GetMousePos();
		MathLib::Vector2 currentPos = *reinterpret_cast<MathLib::Vector2*>(
			&mousePos);
		m_prevMousePos = m_currMousePos;
		m_currMousePos = currentPos;
	}

	void SceneView::HandleCameraInput(const Engine::Timestep& ts)
	{
		// Resets the camera.
		ImGuiIO& io = ImGui::GetIO();

		Engine::PlatformInput& platformInput = Engine::Input::GetPlatformInput();
		EditorCamera& editorCamera = EditorSceneManager::GetEditorCamera();
		// Focuses the camera.
		if (ImGui::IsKeyPressed(
			platformInput.FromKeyCode<ImGuiKey>(Engine::InputKeyCode::KEY_CODE_F)))
		{
			editorCamera.FocusCamera();
			return;
		}

		MathLib::Vector2 delta = m_currMousePos - m_prevMousePos;
		// Moves the camera based on mouse dragging.
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
		{
			MathLib::Vector3 dir = editorCamera.GetRight() * -delta.x
				+ editorCamera.GetUp() * delta.y;
			editorCamera.MoveCamera(dir * MOUSE_CAMERA_MULTIPLIER, true);
		}

		bool isLeftAltPressed = ImGui::IsKeyDown(
			platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_LEFT_ALT));
		m_transformationWidget.SetMovable(!isLeftAltPressed);

		bool handledZoom = false;
		if (isLeftAltPressed)
		{
			float altKeyHeld = io.KeysData[
				platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_LEFT_ALT)].DownDuration;
			// Only updates the rotation if the left mouse button is held.
			if (ImGui::IsMouseDown(
				platformInput.FromMouseButton(Engine::InputMouseButton::MOUSE_BUTTON_LEFT)))
			{
				float leftMouseButtonTime = io.MouseDownDuration[
					platformInput.FromMouseButton(Engine::InputMouseButton::MOUSE_BUTTON_LEFT)];
				if (altKeyHeld > leftMouseButtonTime)
				{
					editorCamera.RotateCamera(delta);
				}
			}

			if (ImGui::IsMouseDown(
				platformInput.FromMouseButton(Engine::InputMouseButton::MOUSE_BUTTON_RIGHT)))
			{
				float rightMouseButtonTime = io.MouseDownDuration[
					platformInput.FromMouseButton(Engine::InputMouseButton::MOUSE_BUTTON_RIGHT)];
				handledZoom = altKeyHeld > rightMouseButtonTime
					&& editorCamera.ZoomCamera(ts * delta.x * MOUSE_CAMERA_MULTIPLIER);
			}
		}
		else
		{
			if (ImGui::IsMouseDown(
				platformInput.FromMouseButton(Engine::InputMouseButton::MOUSE_BUTTON_RIGHT)))
			{
				MathLib::Vector3 camInputDirection;
				if (GetCameraDirection(camInputDirection, editorCamera, platformInput))
				{
					editorCamera.SetFocused(false);
					camInputDirection.Normalize();
					editorCamera.MoveCamera(camInputDirection *
						editorCamera.GetCameraProperties().cameraSpeed * ts.GetRawSeconds(), false);
				}
			}
		}

		if (!handledZoom)
		{
			editorCamera.ZoomCamera(ts * m_mouseScroll.y *
				(DEFAULT_SCROLL_DISTANCE * 0.25F));
		}
	}

	void SceneView::UpdateEntityIDs(const Engine::Timestep& ts)
	{
		if (m_entityWidgetChanged || m_transformationWidget.IsMouseOver()) return;

		// If an alt key is pressed, don't do anything.
		const auto& platformInput = Engine::Input::GetPlatformInput();
		ImGuiKey lAltKey = platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_LEFT_ALT);
		ImGuiKey rAltKey = platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_RIGHT_ALT);
		if (ImGui::IsKeyDown(lAltKey) || ImGui::IsKeyDown(rAltKey)) return;

		MathLib::Vector2 windowSize = GetResizedWindow();
		if (windowSize.x <= 0 || windowSize.y <= 0) return;
		Engine::Texture* entityIDsTexture = m_frameBuffer->GetRenderTargetTexture(1);
		if (entityIDsTexture != nullptr
			&& m_focused
			&& ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			MathLib::Vector2 localMousePos = m_currMousePos - m_windowPosition;
			if (localMousePos.x < 0.0f
				|| localMousePos.x > windowSize.x
				|| localMousePos.y < 0.0f
				|| localMousePos.y > windowSize.y)
			{
				return;
			}
			entityIDsTexture->CopyPixels(m_sceneViewEntityIDs);

			uint32_t pixelX = (uint32_t)localMousePos.x;
			uint32_t pixelY = (uint32_t)localMousePos.y;
			int32_t entityID;
			uint32_t index = pixelX + pixelY * entityIDsTexture->GetWidth();
			if (m_sceneViewEntityIDs.GetViaIndex((size_t)index, entityID))
			{
				int32_t currentEntityID = entityID - 1;
				if (currentEntityID < 0)
				{
					EditorSelection::SetSelectedEntity(Engine::Entity::None);
					return;
				}
				Engine::Scene* scenePtr = &Engine::SceneManager::GetActiveScene();
				auto selectedEntity = scenePtr->CreateEntityRef((entt::entity)currentEntityID);
				// Sets the selected entity to the one picked by the scene.
				if (selectedEntity.IsValid())
				{
					EditorSelection::SetSelectedEntity(selectedEntity);
					return;
				}
				EditorSelection::SetSelectedEntity(Engine::Entity::None);
			}
		}
	}

	void SceneView::UpdateTransformationWidget(const Engine::Timestep& ts)
	{
		m_transformationWidget.SetEnabled(EditorSelection::HasSelectedEntity());
		if (m_transformationWidget.IsEnabled())
		{
			auto entity = EditorSelection::GetSelectedEntity();
			if (entity.HasComponent<Engine::Transform3DComponent>())
			{
				Engine::Transform3DComponent& transform3D
					= entity.GetComponent<Engine::Transform3DComponent>();
				if (entity != m_prevSelectedEntity)
				{
					m_transformationWidget.SetTransform(transform3D);
					m_prevSelectedEntity = entity;
					return;
				}

				if (m_entityWidgetChanged)
				{
					transform3D.SetLocalPosition(
						m_transformationWidget.GetTransform().GetLocalPosition());
					transform3D.SetLocalEulerAngles(
						m_transformationWidget.GetTransform().GetLocalEulerAngles());
					transform3D.SetLocalScale(
						m_transformationWidget.GetTransform().GetLocalScale());
				}
				else
				{
					m_transformationWidget.SetTransform(transform3D);
				}
			}
			else if (entity.HasComponent<Engine::Transform2DComponent>())
			{
				Engine::Transform2DComponent& transform2D
					= entity.GetComponent<Engine::Transform2DComponent>();
				if (entity != m_prevSelectedEntity)
				{
					m_transformationWidget.SetTransform(transform2D);
					m_prevSelectedEntity = entity;
					return;
				}

				if (m_entityWidgetChanged)
				{
					const auto& transform = m_transformationWidget.GetTransform();
					transform2D.SetLocalPosition(
						transform.GetLocalPosition().x, transform.GetLocalPosition().y);
					transform2D.SetLocalRotation(
						transform.GetLocalEulerAngles(false).z, false);
					transform2D.SetLocalScale(transform.GetLocalScale().x, transform.GetLocalScale().y);
				}
				else
				{
					m_transformationWidget.SetTransform(transform2D);
				}
			}
		}
		m_prevSelectedEntity = EditorSelection::GetSelectedEntity();
	}

	bool SceneView::GetCameraDirection(MathLib::Vector3& outputDirection,
		const EditorCamera& editorCamera, const Engine::PlatformInput& platformInput) const
	{
		bool hasInput = false;
		if (ImGui::IsKeyDown(platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_W))
			&& !ImGui::IsKeyDown(platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_S)))
		{
			hasInput = true;
			outputDirection += editorCamera.GetForward();
		}
		if (ImGui::IsKeyDown(platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_S))
			&& !ImGui::IsKeyDown(platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_W)))
		{
			hasInput = true;
			outputDirection += -editorCamera.GetForward();
		}
		if (ImGui::IsKeyDown(platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_A))
			&& !ImGui::IsKeyDown(platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_D)))
		{
			hasInput = true;
			outputDirection += -editorCamera.GetRight();
		}
		if (ImGui::IsKeyDown(platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_D))
			&& !ImGui::IsKeyDown(platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_A)))
		{
			hasInput = true;
			outputDirection += editorCamera.GetRight();
		}
		if (ImGui::IsKeyDown(platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_Q))
			&& !ImGui::IsKeyDown(platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_E)))
		{
			hasInput = true;
			outputDirection += -MathLib::Vector3::UnitY;
		}
		if (ImGui::IsKeyDown(platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_E))
			&& !ImGui::IsKeyDown(platformInput.FromKeyCode<ImGuiKey>(Engine::KEY_CODE_Q)))
		{
			hasInput = true;
			outputDirection += MathLib::Vector3::UnitY;
		}
		return hasInput;
	}

	const MathLib::Vector2 SceneView::GetResizedWindow() const
	{
		MathLib::Vector2 windowSize = m_windowSize;
		windowSize.y -= m_windowBarSpacing;
		return windowSize;
	}

	void SceneView::RenderScene()
	{
		if (!m_open)
		{
			return;
		}
		EditorCamera& editorCamera = EditorSceneManager::GetEditorCamera();
		// Sets the editor camera properties based on the window size.
		{
			MathLib::Vector2 windowSize = GetResizedWindow();
			EditorCameraProperties& properties = editorCamera.GetCameraProperties();
			properties.width = windowSize.x;
			properties.height = windowSize.y;
			m_frameBuffer->Resize((uint32_t)properties.width, (uint32_t)properties.height);
		}

		Engine::CameraConstants cameraConstants;
		{
			cameraConstants.c_viewProjection
				= editorCamera.GetViewProjectionMatrix();
			cameraConstants.c_cameraPosition = editorCamera.GetTransformMatrix().GetTranslation();
		}

		m_frameBuffer->Bind();
		Engine::SceneManager::Render(cameraConstants);
		m_frameBuffer->UnBind();
	}


	void SceneView::Draw()
	{
		if (!m_open)
		{
			return;
		}
		static ImGuiWindowFlags sWindowFlags = ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoCollapse;
		ImGuiWindowFlags windowFlags = sWindowFlags;
		ImGui::Begin("Scene", &m_open, windowFlags);

		// Updates scene view attributes.
		{
			ImGuiIO& io = ImGui::GetIO();
            ImVec2 windowSize = ImGui::GetWindowSize();
			m_windowSize = *reinterpret_cast<MathLib::Vector2*>(&windowSize);
			m_focused = ImGui::IsWindowFocused();
			m_mouseScroll = MathLib::Vector2{ io.MouseWheelH, io.MouseWheel };
			m_windowBarSpacing = ImGui::GetTextLineHeightWithSpacing() * 2.0f;
		}

		Engine::Texture* frameTexture = m_frameBuffer->GetRenderTargetTexture(0);
		if (frameTexture != nullptr)
		{
			ImGui::Image((void*)frameTexture->GetTextureID(),
				ImVec2((float)frameTexture->GetWidth(), (float)frameTexture->GetHeight()));
		}

		// Adjusts the window position so that the transformation widget fits in the middle
		// of the corresponding transform.
		{
            ImVec2 windowPos = ImGui::GetWindowPos();
			m_windowPosition = *reinterpret_cast<MathLib::Vector2*>(&windowPos);
			m_windowPosition.x += (m_windowSize.x - ImGui::GetContentRegionAvail().x) * 0.5f;
			m_windowPosition.y += ImGui::GetTextLineHeight() * 2.0f;
		}
		DrawTransformationWidgetInput();

		ImGui::End();
	}

	void SceneView::DrawTransformationWidgetInput()
	{
		if (m_focused)
		{
			Engine::PlatformInput& platformInput = Engine::Input::GetPlatformInput();
			if (ImGui::IsKeyDown(
				platformInput.FromKeyCode<ImGuiKey>(Engine::InputKeyCode::KEY_CODE_W)))
			{
				m_transformationWidget.m_widgetOperation
					= ImGuizmo::OPERATION::TRANSLATE;
			}
			else if (ImGui::IsKeyDown(
				platformInput.FromKeyCode<ImGuiKey>(Engine::InputKeyCode::KEY_CODE_E)))
			{
				m_transformationWidget.m_widgetOperation
					= ImGuizmo::OPERATION::ROTATE;
			}
			else if (ImGui::IsKeyDown(
				platformInput.FromKeyCode<ImGuiKey>(Engine::InputKeyCode::KEY_CODE_R)))
			{
				m_transformationWidget.m_widgetOperation
					= ImGuizmo::OPERATION::SCALE;
			}
		}
		m_entityWidgetChanged
			= m_transformationWidget.OnImGuiRender(
				m_windowPosition,
				GetResizedWindow());
	}
}
