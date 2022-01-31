#include "EditorPCH.h"
#include "GameView.h"

#include "EditorUtils.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{

	GameView::GameView()
		: m_frameBuffer(nullptr),
		m_open(true)
	{

		const auto& window = Engine::Application::Get().GetWindow();
		Engine::FrameBufferSpecification frameBufferSpecification(
			{
				Engine::DEPTH_STENCIL,
				Engine::RENDER_TARGET
			});
		frameBufferSpecification.width = window.GetWidth();
		frameBufferSpecification.height = window.GetHeight();
		m_frameBuffer = Engine::FrameBuffer::Create(frameBufferSpecification);
	}

	GameView::~GameView()
	{
		delete m_frameBuffer;
	}

	void GameView::RenderScene()
	{
		m_frameBuffer->Bind();
		Engine::SceneManager::Render();
	}
	
	void GameView::Draw()
	{
		if (!m_open)
		{
			return;
		}

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoScrollbar;
		ImGui::Begin("Game", &m_open, windowFlags);

		Engine::Texture* texture = m_frameBuffer->GetTexture(Engine::RENDER_TARGET);
		if (texture != nullptr)
		{
			const void* textureID = texture->GetTextureID();
			ImGui::Image((void*)textureID, ImVec2((float)texture->GetWidth(),
				(float)texture->GetHeight()));
		}

		ImGui::End();
	}
}