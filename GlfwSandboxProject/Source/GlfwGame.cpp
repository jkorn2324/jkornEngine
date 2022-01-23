#include "GlfwGame.h"

#include "imgui.h"

namespace GlfwSandbox
{
	static const wchar_t* CARDS_LARGE_TILEMAP = L"Assets/Textures/PlayingCards/Tilesheet/cardsLarge_tilemap.png";
	
	static float s_rotation = 0.0f;

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
	}
	
	GlfwGame::~GlfwGame()
	{
		Engine::SceneSerializer serializer(&Engine::SceneManager::GetActiveScene());
		serializer.Serialize(L"GlfwGameScene.json");

		delete m_frameBuffer;
		delete m_subTexture;
		delete m_entityConstantBuffer;
		delete m_vertexBuffer;
		delete m_indexBuffer;
	}

	void GlfwGame::InitializeRenderBuffers()
	{
		Engine::AssetCache<Engine::Texture>& textureAssetCache =
			Engine::AssetManager::GetTextures();
		{
			textureAssetCache.Load(L"Assets/Textures/happy-face.png");
			Engine::Texture* texture = textureAssetCache.Load(CARDS_LARGE_TILEMAP);
			m_subTexture = Engine::SubTexture::CreateFromTexCoords(texture,
				MathLib::Vector2(11.0f, 2.0f), MathLib::Vector2(51.0f, 61.0f));
		}
		m_entityConstantBuffer = Engine::ConstantBuffer::Create(
			&m_entityConstants, sizeof(m_entityConstants));

		Engine::FrameBufferSpecification frameBufferSpecification({
			{ Engine::FrameBufferAttachmentType::DEPTH_STENCIL }
		});
		frameBufferSpecification.width = Engine::GraphicsRenderer::GetRenderingAPI().GetWidth();
		frameBufferSpecification.height = Engine::GraphicsRenderer::GetRenderingAPI().GetHeight();
		m_frameBuffer = Engine::FrameBuffer::Create(frameBufferSpecification);
	}

	void GlfwGame::InitializeSceneComponents()
	{
		Engine::SceneManager::LoadScene(L"GlfwGameScene.json");
		Engine::Scene& scene = Engine::SceneManager::GetActiveScene();

		Engine::Entity cameraEntity = scene.Find("CameraEntity");
		if (cameraEntity.IsValid())
		{
			return;
		}
		cameraEntity = scene.CreateEntity("CameraEntity");
		{
			Engine::Transform3DComponent& camComponentTransform
				= cameraEntity.AddComponent<Engine::Transform3DComponent>();
			camComponentTransform.SetLocalPosition(MathLib::Vector3(-1.0f, 0.0f, 0.0f));
			camComponentTransform.LookAt(MathLib::Vector3(0.0f, 0.0f, 0.0f));
			
			Engine::SceneCameraComponent& camComponent
				= cameraEntity.AddComponent<Engine::SceneCameraComponent>();
			Engine::CameraProperties& properties
				= camComponent.camera.GetProperties();
			Engine::Application& app = Engine::Application::Get();
			properties.orthoWidth = (float)app.GetWindow().GetWidth();
			properties.orthoHeight = (float)app.GetWindow().GetHeight();
		}

		Engine::Entity entity = scene.CreateEntity("HappyFace");
		{
			Engine::Transform3DComponent& component
				= entity.AddComponent<Engine::Transform3DComponent>();
			component.SetLocalScale(200.0f, 200.0f, 1.0f);
			entity.AddComponent<Engine::SpriteComponent>();
			/* entity.AddComponent<Engine::SpriteComponent>(
				Engine::AssetManager::GetTextures().Get(L"Assets/Textures/happy-face.png")); */
		}
	}

	void GlfwGame::OnUpdate(const Engine::Timestep& ts)
	{
		Engine::SceneManager::OnRuntimeUpdate(ts);

		Engine::Scene& scene = Engine::SceneManager::GetActiveScene();
		Engine::Entity entity = scene.Find("HappyFace");

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
				MathLib::Vector3 scale = transformComponent.GetLocalScale() + MathLib::Vector3::One *
					Engine::Input::GetMouseScrollOffset().y * 0.02f;
				transformComponent.SetLocalScale(scale);
			}

			if (Engine::Input::IsMouseButtonHeld(Engine::MOUSE_BUTTON_MIDDLE))
			{
				s_rotation += 10.0f;
				transformComponent.SetLocalRotation(MathLib::Quaternion::FromEuler(0.0f, 0.0f, s_rotation));
			}

			MathLib::Vector2 screenPos = Engine::Input::GetMouseScreenPos();
			position = scene.GetCamera()->ScreenToWorld(screenPos);
			transformComponent.SetLocalPosition(position);
		}
		Render();
	}

	// Editor Stuff for ImGui.

	void GlfwGame::OnImGuiRender()
	{
		Engine::Scene& scene = Engine::SceneManager::GetActiveScene();
		Engine::Entity spriteEntity = scene.Find("HappyFace");
		if (spriteEntity.IsValid())
		{
			Engine::SpriteComponent& component = 
				spriteEntity.GetComponent<Engine::SpriteComponent>();

			ImGui::Begin("Variables Editor");
			ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&component.color));
			ImGui::End();
		}
	}

	void GlfwGame::OnEvent(Engine::Event& event)
	{
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Invoke<Engine::WindowResizedEvent>(BIND_EVENT_FUNCTION(GlfwGame::OnWindowResize));
	}

	bool GlfwGame::OnWindowResize(Engine::WindowResizedEvent& event)
	{
		Engine::Scene& scene = Engine::SceneManager::GetActiveScene();
		Engine::Entity cameraEntity = scene.Find("CameraEntity");
		if (cameraEntity.IsValid())
		{
			Engine::SceneCameraComponent& component
				= cameraEntity.GetComponent<Engine::SceneCameraComponent>();
			Engine::CameraProperties& properties
				= component.camera.GetProperties();
			properties.orthoWidth = (float)event.width;
			properties.orthoHeight = (float)event.height;
		}
		return true;
	}

	// TODO: May need to render on a separate thread
	// user presses and resizes the screen.

	void GlfwGame::Render()
	{
		m_frameBuffer->Bind();
		Engine::SceneManager::GetActiveScene().Render();

		{
			Engine::GraphicsRenderer2D::DrawRect(MathLib::Vector2(20.0f, 0.0f),
				MathLib::Vector2::One, m_subTexture);
		}
	}
}