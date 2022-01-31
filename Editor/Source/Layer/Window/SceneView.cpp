#include "EditorPCH.h"

#include "SceneView.h"
#include "EditorUtils.h"
#include "PlatformInput.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	SceneView::SceneView()
		: m_frameBuffer(nullptr),
		m_open(true),
		m_focused(false),
		m_prevMousePos(),
		m_mouseScroll(),
		m_windowSize()
	{
		Engine::FrameBufferSpecification editorSpecifications(
			{
				Engine::FrameBufferAttachmentType::DEPTH_STENCIL,
				Engine::FrameBufferAttachmentType::RENDER_TARGET
			});

		Engine::Application& app = Engine::Application::Get();
		m_windowSize.x = editorSpecifications.width = app.GetWindow().GetWidth();
		m_windowSize.y = editorSpecifications.height = app.GetWindow().GetHeight();
		m_frameBuffer = Engine::FrameBuffer::Create(editorSpecifications);
	}

	SceneView::~SceneView()
	{
		delete m_frameBuffer;
	}

	void SceneView::OnEvent(Engine::Event& event)
	{
		// TODO: Implementation
	}

	void SceneView::OnUpdate(const Engine::Timestep& ts)
	{
		if (!m_focused)
		{
			return;
		}
		HandleCameraInput(ts);
	}

	void SceneView::HandleCameraInput(const Engine::Timestep& ts)
	{
		// Resets the camera.
		MathLib::Vector2 currentPos = *reinterpret_cast<MathLib::Vector2*>(
			&ImGui::GetMousePos());

		Engine::PlatformInput& platformInput = Engine::Input::GetPlatformInput();
		EditorCamera& editorCamera = EditorSceneManager::GetEditorCamera();
		if (ImGui::IsKeyPressed(
			platformInput.FromKeyCode(Engine::InputKeyCode::KEY_CODE_R)))
		{
			editorCamera.ResetCamera();
			m_prevMousePos = currentPos;
			return;
		}

		// Focuses the camera.
		if (ImGui::IsKeyPressed(
			platformInput.FromKeyCode(Engine::InputKeyCode::KEY_CODE_F)))
		{
			editorCamera.FocusCamera();
			m_prevMousePos = currentPos;
			return;
		}
		
		MathLib::Vector2 delta = currentPos - m_prevMousePos;

		// Moves the camera based on mouse dragging.
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
		{
			MathLib::Vector3 dir = editorCamera.GetRight() * -delta.x
				+ editorCamera.GetUp() * delta.y;
			editorCamera.MoveCamera(dir * MOUSE_CAMERA_MULTIPLIER, true);
		}

		bool handledZoom = false;
		if (ImGui::IsKeyDown(
			platformInput.FromKeyCode(Engine::KEY_CODE_LEFT_ALT)))
		{
			float altKeyHeld = Engine::Input::GetTimeKeyHeld(
				Engine::InputKeyCode::KEY_CODE_LEFT_ALT);
			// Only updates the rotation if the left mouse button is held.
			if (ImGui::IsMouseDown(
				platformInput.FromMouseButton(Engine::InputMouseButton::MOUSE_BUTTON_LEFT)))
			{
				float leftMouseButtonTime = Engine::Input::GetMouseButtonTimeHeld(
					Engine::InputMouseButton::MOUSE_BUTTON_LEFT);
				if (altKeyHeld > leftMouseButtonTime)
				{
					editorCamera.RotateCamera(delta);
				}
			}

			if (ImGui::IsMouseDown(
				platformInput.FromMouseButton(Engine::InputMouseButton::MOUSE_BUTTON_RIGHT)))
			{
				float rightMouseButtonTime = Engine::Input::GetMouseButtonTimeHeld(
					Engine::InputMouseButton::MOUSE_BUTTON_RIGHT);
				handledZoom = altKeyHeld > rightMouseButtonTime
					&& editorCamera.ZoomCamera(ts * delta.x * MOUSE_CAMERA_MULTIPLIER);
			}
		}
		else
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

		if (!handledZoom)
		{
			editorCamera.ZoomCamera(ts * m_mouseScroll.y);
		}
		m_prevMousePos = currentPos;
	}

	bool SceneView::GetCameraDirection(MathLib::Vector3& outputDirection,
		const EditorCamera& editorCamera, const Engine::PlatformInput& platformInput) const
	{
		bool hasInput = false;
		MathLib::Vector3 cameraInputDirection = MathLib::Vector3::Zero;
		if (ImGui::IsKeyDown(platformInput.FromKeyCode(Engine::KEY_CODE_W))
			&& !ImGui::IsKeyDown(platformInput.FromKeyCode(Engine::KEY_CODE_S)))
		{
			hasInput = true;
			cameraInputDirection += editorCamera.GetForward();
		}
		if (ImGui::IsKeyDown(platformInput.FromKeyCode(Engine::KEY_CODE_S))
			&& !ImGui::IsKeyDown(platformInput.FromKeyCode(Engine::KEY_CODE_W)))
		{
			hasInput = true;
			cameraInputDirection += -editorCamera.GetForward();
		}
		if (ImGui::IsKeyDown(platformInput.FromKeyCode(Engine::KEY_CODE_A))
			&& !ImGui::IsKeyDown(platformInput.FromKeyCode(Engine::KEY_CODE_D)))
		{
			hasInput = true;
			cameraInputDirection += -editorCamera.GetRight();
		}
		if (ImGui::IsKeyDown(platformInput.FromKeyCode(Engine::KEY_CODE_D))
			&& !ImGui::IsKeyDown(platformInput.FromKeyCode(Engine::KEY_CODE_A)))
		{
			hasInput = true;
			cameraInputDirection += editorCamera.GetRight();
		}
		if (ImGui::IsKeyDown(platformInput.FromKeyCode(Engine::KEY_CODE_Q))
			&& !ImGui::IsKeyDown(platformInput.FromKeyCode(Engine::KEY_CODE_E)))
		{
			hasInput = true;
			cameraInputDirection += -MathLib::Vector3::UnitY;
		}
		if (ImGui::IsKeyDown(platformInput.FromKeyCode(Engine::KEY_CODE_E))
			&& !ImGui::IsKeyDown(platformInput.FromKeyCode(Engine::KEY_CODE_Q)))
		{
			hasInput = true;
			cameraInputDirection += MathLib::Vector3::UnitY;
		}
		return hasInput;
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
			EditorCameraProperties& properties = editorCamera.GetCameraProperties();
			properties.width = m_windowSize.x;
			properties.height = m_windowSize.y;
			m_frameBuffer->Resize(properties.width, properties.height);
		}

		Engine::CameraConstants cameraConstants;
		{
			cameraConstants.c_viewProjection
				= editorCamera.GetViewProjectionMatrix();
			cameraConstants.c_cameraPosition = editorCamera.GetTransformMatrix().GetTranslation();
		}

		m_frameBuffer->Bind();
		Engine::SceneManager::Render(cameraConstants);
	}

	
	void SceneView::Draw()
	{
		if (!m_open)
		{
			return;
		}
		// TODO: Implementation

		static ImGuiWindowFlags sWindowFlags = ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoCollapse;
		ImGuiWindowFlags windowFlags = sWindowFlags;
		ImGui::Begin("Scene", &m_open, windowFlags);

		// Updates scene view attributes.
		{
			m_windowSize = *reinterpret_cast<MathLib::Vector2*>(&ImGui::GetWindowSize());
			m_focused = ImGui::IsWindowFocused();
			// TODO: Fix
			m_mouseScroll = MathLib::Vector2{ 0.0f, 0.0f };
		}

		Engine::Texture* frameTexture = m_frameBuffer->GetTexture(Engine::RENDER_TARGET);
		if (frameTexture != nullptr)
		{
			Engine::Window& mainWindow = Engine::Application::Get().GetWindow();
			ImVec2 appSize = { (float)mainWindow.GetWidth(), (float)mainWindow.GetHeight() };

			ImGui::Image((void*)frameTexture->GetTextureID(),
				ImVec2(frameTexture->GetWidth(), frameTexture->GetHeight()));
		}
		ImGui::End();
	}
}