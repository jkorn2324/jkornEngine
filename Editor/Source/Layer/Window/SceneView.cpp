#include "EditorPCH.h"

#include "SceneView.h"
#include "EditorUtils.h"

#include <imgui.h>

namespace Editor
{
	SceneView::SceneView()
		: m_frameBuffer(nullptr),
		m_open(true)
	{
		Engine::FrameBufferSpecification editorSpecifications(
			{
				Engine::FrameBufferAttachmentType::DEPTH_STENCIL,
				Engine::FrameBufferAttachmentType::RENDER_TARGET
			});

		Engine::Application& app = Engine::Application::Get();
		editorSpecifications.width = app.GetWindow().GetWidth();
		editorSpecifications.height = app.GetWindow().GetHeight();
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

	void SceneView::RenderScene()
	{
		if (!m_open)
		{
			return;
		}
		EditorCamera& editorCamera = EditorSceneManager::GetEditorCamera();
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


		Engine::Texture* frameTexture = m_frameBuffer->GetTexture(Engine::RENDER_TARGET);
		if (frameTexture != nullptr)
		{
			// TODO: Fit the texture inside of the window.
			ImGui::Image((void*)frameTexture->GetTextureID(),
				ImVec2(frameTexture->GetWidth(), frameTexture->GetHeight()));
		}
		ImGui::End();
	}
}