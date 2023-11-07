#include "GlfwGame.h"

#include "CameraController.h"
#include <imgui.h>

#include "SystemUtility.h"

namespace GlfwSandbox
{
	static const wchar_t* CARDS_LARGE_TILEMAP = L"Assets/Textures/PlayingCards/Tilesheet/cardsLarge_tilemap.png";

	static float s_rotation = 0.0f;

	namespace
	{
		void InitializeSystems()
		{
			Engine::SystemManager::AddSystem<CameraControllerSystem>();
		}

		void InitializeCameraEntity(Engine::Scene& scene)
		{
			{
				auto cameraEntity = scene.FindEntity<Engine::NameComponent>("CameraEntity",
					[](const char* Name, const Engine::NameComponent& component) -> bool {
						return component.name == Name;
					});
				if (cameraEntity.IsValid())
				{
					// Adds the Camera Controller.
					cameraEntity.AddComponent<CameraController>();
					return;
				}
			}

			auto cameraEntity = scene.CreateEntity("CameraEntity");
			{
				Engine::Transform3DComponent& camComponentTransform
					= cameraEntity.AddComponent<Engine::Transform3DComponent>();
				camComponentTransform.SetLocalPosition(MathLib::Vector3(-1.0f, 0.0f, 0.0f));
				camComponentTransform.LookAt(MathLib::Vector3(0.0f, 0.0f, 0.0f));

				Engine::SceneCameraComponent& camComponent
					= cameraEntity.AddComponent<Engine::SceneCameraComponent>();
				Engine::CameraProperties& properties
					= camComponent.camera.GetProperties();
				camComponent.camera.SetSceneCameraType(Engine::SceneCameraType::TYPE_ORTHOGRAPHIC);
				Engine::Application& app = Engine::Application::Get();
				properties.orthoWidth = (float)app.GetWindow().GetWidth();
				properties.orthoHeight = (float)app.GetWindow().GetHeight();

				cameraEntity.AddComponent<CameraController>();
			}
		}
	}

	// The func ptr for finding by name.
	constexpr auto FindByNameFuncPtr = [](const char* Name, const Engine::NameComponent& component) -> bool {
		return Name == component.name;
	};

	GlfwGame::GlfwGame()
		: Layer("Game Layer"),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_entityConstantBuffer(nullptr),
		m_subTexture(nullptr),
		m_frameBuffer(nullptr)
	{
		InitializeRenderBuffers();
		InitializeSceneComponents();
		InitializeSystems();
	}

	GlfwGame::~GlfwGame()
	{
		delete m_frameBuffer;
		delete m_subTexture;
		delete m_entityConstantBuffer;
		delete m_vertexBuffer;
		delete m_indexBuffer;
	}

	void GlfwGame::InitializeRenderBuffers()
	{
		{
			Engine::Texture* texture;
			if (Engine::Texture::LoadFromFile(&texture, CARDS_LARGE_TILEMAP))
			{
				m_subTexture = Engine::SubTexture::CreateFromTexCoords(texture,
					MathLib::Vector2(11.0f, 2.0f), MathLib::Vector2(51.0f, 61.0f));
			}
		}

		// Creates a constant buffer.
		Engine::ConstantBuffer::Create(&m_entityConstantBuffer, &m_entityConstants, sizeof(m_entityConstants));

		Engine::FrameBufferDepthStencilContext depthStencilContext = { Engine::DepthFormat_D24UNorm_S8UInt };
		Engine::FrameBufferSpecification frameBufferSpecification({
			Engine::FrameBufferAttachment{ depthStencilContext }
		});
		frameBufferSpecification.width = Engine::GraphicsRenderer::GetRenderingAPI().GetWidth();
		frameBufferSpecification.height = Engine::GraphicsRenderer::GetRenderingAPI().GetHeight();
		m_frameBuffer = Engine::FrameBuffer::Create(frameBufferSpecification);
	}

	void GlfwGame::InitializeSceneComponents()
	{
		Engine::SceneManager::LoadScene(L"GlfwGameScene.json");
		Engine::Scene& scene = Engine::SceneManager::GetActiveScene();

		InitializeCameraEntity(scene);

		auto entity = scene.CreateEntity("HappyFace");
		{
			Engine::Transform3DComponent& component
				= entity.AddComponent<Engine::Transform3DComponent>();
			component.SetLocalScale(1.0f, 1.0f, 1.0f);
			Engine::SpriteComponent& sprite
				= entity.AddComponent<Engine::SpriteComponent>();

			Engine::Texture::LoadFromFile(&sprite.texture, L"Assets/Textures/happy-face.png");
		}
	}

	void GlfwGame::OnUpdate(const Engine::Timestep& ts)
	{
		Engine::SceneManager::OnUpdate(ts);
		Engine::SceneManager::OnRuntimeUpdate(ts);

		// Invokes Update on the systems.
		Engine::SystemUtility::InvokeOnUpdate(ts, true);

		Engine::Scene& scene = Engine::SceneManager::GetActiveScene();

		auto entity = scene.FindEntity<Engine::NameComponent>("HappyFace", FindByNameFuncPtr);
		if (entity.IsValid())
		{
			Engine::Transform3DComponent& transformComponent
				= entity.GetComponent<Engine::Transform3DComponent>();
			MathLib::Vector3 position = transformComponent.GetWorldPosition();
			MathLib::Vector3 direction;
			if (Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_A))
			{
				direction = -MathLib::Vector3::UnitX;
			}
			else if (Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_D))
			{
				direction = MathLib::Vector3::UnitX;
			}

#if 0
			if (Engine::Input::IsKeyPressed(Engine::InputKeyCode::KEY_CODE_SPACE))
			{
				position += MathLib::Vector3::UnitY * 20.0f;
			}
#endif

			{
				Engine::Transform3DComponent& transformComponent
					= entity.GetComponent<Engine::Transform3DComponent>();
				/* MathLib::Vector3 scale = transformComponent.GetLocalScale() + MathLib::Vector3::One *
					Engine::Input::GetMouseScrollOffset().y * 0.02f;
				transformComponent.SetLocalScale(scale); */
			}

			if (Engine::Input::IsMouseButtonHeld(Engine::MOUSE_BUTTON_MIDDLE))
			{
				s_rotation += 10.0f;
				transformComponent.SetLocalEulerAngles(0.0f, 0.0f, s_rotation);
			}

			MathLib::Vector2 screenPos = Engine::Input::GetMouseScreenPos();
			auto camera = scene.GetCamera();
			if (camera != nullptr)
			{
				position = camera->ScreenToWorld(screenPos);
				transformComponent.SetLocalPosition(position);
			}
		}
		Render();
	}

	// Editor Stuff for ImGui.

	void GlfwGame::OnImGuiRender()
	{
		Engine::Scene& scene = Engine::SceneManager::GetActiveScene();
		auto spriteEntity = scene.FindEntity<Engine::NameComponent>("HappyFace", FindByNameFuncPtr);
		if (spriteEntity.IsValid())
		{
			Engine::SpriteComponent& component =
				spriteEntity.GetComponent<Engine::SpriteComponent>();

			ImGui::Begin("Variables Editor");
			ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&component.color));
			ImGui::End();
		}
	}

	void GlfwGame::OnEvent(Engine::IEvent& event)
	{
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Invoke<Engine::WindowEventType, Engine::WindowResizedEvent>(BIND_EVENT_FUNCTION(GlfwGame::OnWindowResize));
	}

	bool GlfwGame::OnWindowResize(Engine::WindowResizedEvent& event)
	{
		Engine::Scene& scene = Engine::SceneManager::GetActiveScene();
		auto cameraEntity = scene.FindEntity<Engine::NameComponent>("CameraEntity", FindByNameFuncPtr);
		if (cameraEntity.IsValid())
		{
			Engine::SceneCameraComponent& component
				= cameraEntity.GetComponent<Engine::SceneCameraComponent>();
			Engine::CameraProperties& properties
				= component.camera.GetProperties();
			properties.orthoWidth = (float)event.width;
			properties.orthoHeight = (float)event.height;
		}
		m_frameBuffer->Resize(event.width, event.height);
		return true;
	}

	// TODO: May need to render on a separate thread
	// user presses and resizes the screen.

	void GlfwGame::Render()
	{
		m_frameBuffer->Bind();
		Engine::SceneManager::Render();

		{
			MathLib::Vector2 scale = { 1.0f, 1.0f };
			Engine::GraphicsRenderer2D::DrawRect(MathLib::Vector2(0.0f, 0.0f),
				scale, m_subTexture);
		}
		// Don't want to unbind this because then it will result in render target view being cleared. 
        // m_frameBuffer->UnBind();
	}
}
