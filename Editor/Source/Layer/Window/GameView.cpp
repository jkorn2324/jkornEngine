#include "EditorPCH.h"
#include "GameView.h"

#include "EditorSelection.h"
#include "ImGuiUtils.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{

	static void DrawAspectMenuItem(const char* aspectName, GameViewAspectRatioType correspondingType,
		GameViewAspectRatioType& selectedAspect)
	{
		if (correspondingType == selectedAspect)
		{
			std::string newString = aspectName;
			newString += " [x]";

			if (ImGui::MenuItem(newString.c_str()))
			{
				selectedAspect = correspondingType;
			}
		}
		else
		{
			if (ImGui::MenuItem(aspectName))
			{
				selectedAspect = correspondingType;
			}
		}
	}

	GameView::GameView()
		: m_frameBuffer(nullptr),
		m_open(true),
		m_windowSize(),
		m_aspectRatio(),
		m_windowMenuBarSpacing()
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

		m_aspectRatio.m_aspectWidth = m_windowSize.x = (float)frameBufferSpecification.width;
		m_aspectRatio.m_aspectHeight = m_windowSize.y = (float)frameBufferSpecification.height;
	}

	GameView::~GameView()
	{
		delete m_frameBuffer;
	}

	void GameView::OnUpdate(const Engine::Timestep& ts)
	{
		Engine::Input::SetEnabled(m_focused && m_open);
	}

	void GameView::RenderScene()
	{
		// Sets the camera properties.
		{
			UpdateGameAspectRatio();

			Engine::SceneCamera* sceneCamera = reinterpret_cast<Engine::SceneCamera*>(
				Engine::SceneManager::GetActiveScene().GetCamera());
			if (sceneCamera != nullptr)
			{
				Engine::CameraProperties& camera = sceneCamera->GetProperties();
				camera.orthoWidth = m_aspectRatio.m_aspectWidth;
				camera.orthoHeight = m_aspectRatio.m_aspectHeight;
				camera.perspAspectRatio = m_aspectRatio.m_aspectHeight / m_aspectRatio.m_aspectWidth;
				m_sceneCameraType = sceneCamera->GetSceneCameraType();
			}
			m_frameBuffer->Resize(
				(uint32_t)m_aspectRatio.m_aspectWidth, (uint32_t)m_aspectRatio.m_aspectHeight);
		}
		
		m_frameBuffer->Bind();
		Engine::SceneManager::Render();
		m_frameBuffer->UnBind();
	}

	void GameView::UpdateGameAspectRatio()
	{
		switch (m_aspectRatioType)
		{
			case ASPECT_FREE:
			{
				ConstrainToAspectRatio(m_windowSize.x, 
					m_windowSize.y - m_windowMenuBarSpacing);
				break;
			}
			case ASPECT_16_9:
			{
				ConstrainToAspectRatio(16.0f, 9.0f);
				break;
			}
			case ASPECT_16_10:
			{
				ConstrainToAspectRatio(16.0f, 10.0f);
				break;
			}
			case ASPECT_21_9:
			{
				ConstrainToAspectRatio(21.0f, 9.0f);
				break;
			}
			case ASPECT_4_3:
			{
				ConstrainToAspectRatio(4.0f, 3.0f);
				break;
			}
			case ASPECT_3_2:
			{
				ConstrainToAspectRatio(3.0f, 2.0f);
				break;
			}
		}
	}

	void GameView::ConstrainToAspectRatio(float x, float y)
	{
		float windowSizeY = MathLib::Abs(m_windowSize.y - m_windowMenuBarSpacing);
		float candidateAspectRatio = MathLib::Abs(y) / x;
		float windowAspectRatio = windowSizeY / m_windowSize.x;
		float windowY = windowAspectRatio * x;

		// If the y is greater than y input, than it means that the aspect ratio should
		// be constrained to the x.
		if (windowY > y)
		{
			m_aspectRatio.m_aspectWidth = m_windowSize.x;
			m_aspectRatio.m_aspectHeight = m_aspectRatio.m_aspectWidth * candidateAspectRatio;
		}
		else
		{
			m_aspectRatio.m_aspectHeight = windowSizeY;
			m_aspectRatio.m_aspectWidth = windowSizeY * (x / y);
		}
	}
	
	void GameView::Draw()
	{
		if (!m_open)
		{
			return;
		}

		static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar;
		ImGui::Begin("Game", &m_open, windowFlags);

		// Updates the window properties.
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			m_windowSize = *reinterpret_cast<MathLib::Vector2*>(
				&ImGui::GetWindowSize());
			bool isNotHidden = window->DockNode != nullptr && !window->DockNode->IsHiddenTabBar();
			float textLineHeightSpacing = ImGui::GetTextLineHeightWithSpacing();
			m_windowMenuBarSpacing = isNotHidden
				* textLineHeightSpacing + textLineHeightSpacing * 2.0f;
			m_focused = ImGui::IsWindowFocused();
		}

		DrawMenuBar();

		Engine::Texture* texture = m_frameBuffer->GetTexture(Engine::RENDER_TARGET);
		if (texture != nullptr)
		{
			const void* textureID = texture->GetTextureID();
			ImVec2 textureSize = { (float)m_aspectRatio.m_aspectWidth, (float)m_aspectRatio.m_aspectHeight };

			// Calculates the window size of the game view for the texture.
			MathLib::Vector2 windowSizeDiff = m_windowSize;
			windowSizeDiff.y -= m_windowMenuBarSpacing;
			windowSizeDiff.y -= m_aspectRatio.m_aspectHeight;
			windowSizeDiff.x -= m_aspectRatio.m_aspectWidth;

			float texWidth = (float)texture->GetWidth();
			float texHeight = (float)texture->GetHeight();

			// Adjusts the uvs based on the texture height & texture width.
			ImVec2 uv0 = { 0.0f, 0.0f };
			ImVec2 uv1 = { MathLib::Clamp01(m_aspectRatio.m_aspectWidth / texWidth),
					MathLib::Clamp01(m_aspectRatio.m_aspectHeight / texHeight) };

			if (m_aspectRatio.m_aspectWidth > texWidth
				|| m_aspectRatio.m_aspectHeight > texHeight)
			{
				if (m_aspectRatio.m_aspectWidth > m_aspectRatio.m_aspectHeight)
				{
					uv0.x = MathLib::Clamp01((m_aspectRatio.m_aspectWidth - texWidth * 2.0f) / texWidth);
					uv1.x = 1.0f - uv0.x;
				}
			}

			ImVec2 texturePos = { windowSizeDiff.x * 0.5f, windowSizeDiff.y * 0.5f };
			ImGuiUtils::DrawImage(textureID, texturePos, textureSize, uv0, uv1);
		}

		ImGui::End();
	}

	void GameView::DrawMenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Aspect Ratio"))
			{
				DrawAspectMenuItem("Free Aspect",
					ASPECT_FREE, m_aspectRatioType);
				DrawAspectMenuItem("16:9",
					ASPECT_16_9, m_aspectRatioType);
				DrawAspectMenuItem("16:10",
					ASPECT_16_10, m_aspectRatioType);
				DrawAspectMenuItem("21:9",
					ASPECT_21_9, m_aspectRatioType);
				DrawAspectMenuItem("4:3",
					ASPECT_4_3, m_aspectRatioType);
				DrawAspectMenuItem("3:2",
					ASPECT_3_2, m_aspectRatioType);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}
}