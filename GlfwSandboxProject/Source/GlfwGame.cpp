#include "GlfwGame.h"

#include "GraphicsRenderer.h"
#include "GraphicsRenderer2D.h"
#include "RenderingAPI.h"

#include "AssetManager.h"
#include "AssetCache.h"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"

#include "SubTexture.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "BufferLayout.h"
#include "Material.h"

#include "imgui.h"

namespace GlfwSandbox
{
	static const wchar_t* CARDS_LARGE_TILEMAP = L"Assets/Textures/PlayingCards/Tilesheet/cardsLarge_tilemap.png";

	GlfwGame::GlfwGame()
		: Layer("Game Layer"),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_spriteEntity(nullptr),
		m_scene(nullptr),
		m_entityConstantBuffer(nullptr),
		m_subTexture(nullptr)
	{
		InitializeRenderBuffers();
		InitializeSceneComponents();
	}
	
	GlfwGame::~GlfwGame()
	{
		Engine::AssetManager::UncacheAssets();

		delete m_subTexture;
		delete m_entityConstantBuffer;
		delete m_vertexBuffer;
		delete m_indexBuffer;
		delete m_spriteEntity;
		delete m_scene;
	}

	void GlfwGame::OnUpdate(const Engine::Timestep& ts)
	{
		if (m_scene != nullptr)
		{
			m_scene->Update(ts);
		}
		
		Engine::Transform3DComponent& transformComponent
			= m_spriteEntity->GetComponent<Engine::Transform3DComponent>();
		transformComponent.SetPosition(transformComponent.GetPosition() +
			MathLib::Vector3::UnitX * 4.0f * ts.GetSeconds());

		Render();
	}

	// Editor Stuff for ImGui.

	void GlfwGame::OnImGuiRender()
	{
		Engine::SpriteComponent& component = m_spriteEntity->GetComponent<Engine::SpriteComponent>();

		ImGui::Begin("Sprite Color Editor");
		ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&component.color));
		ImGui::End();
	}

	void GlfwGame::InitializeRenderBuffers()
	{
		{
			VertexPositionColor vertices[3] =
			{
				{
					MathLib::Vector3(0.0f, 0.5f, 0.0f),
					MathLib::Vector4(0.0f, 0.0f, 1.0f, 1.0f)
				},
				{
					MathLib::Vector3(0.0f, -0.5f, 0.0f),
					MathLib::Vector4(0.0f, 1.0f, 0.0f, 1.0f)
				},
				{
					MathLib::Vector3(-0.25f, 0.0f, 0.0f),
					MathLib::Vector4(1.0f, 0.0f, 0.0f, 1.0f)
				}
			};

			std::uint16_t indexBuffer[3]
			{
				0, 1, 2
			};

			m_vertexBuffer = Engine::VertexBuffer::Create(
				&vertices[0], sizeof(vertices) / sizeof(vertices[0]), sizeof(vertices[0]));
			m_indexBuffer = Engine::IndexBuffer::Create(
				&indexBuffer[0], sizeof(indexBuffer) / sizeof(indexBuffer[0]), sizeof(indexBuffer[0]));

			Engine::BufferLayout bufferLayout(
				{
					{ "POSITION", offsetof(VertexPositionColor, position),
						sizeof(MathLib::Vector3), Engine::BufferLayoutType::FLOAT3 },
					{ "COLOR", offsetof(VertexPositionColor, color),
						sizeof(MathLib::Vector4), Engine::BufferLayoutType::FLOAT4 }
				});

			Engine::AssetCache<Engine::Shader>& shaderAssetCache =
				Engine::AssetManager::GetShaders();
			Engine::Shader* shader = shaderAssetCache.Load<const Engine::BufferLayout&>(
				L"Shaders/TriangleShader.hlsl", bufferLayout);
		}

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
	}

	void GlfwGame::InitializeSceneComponents()
	{
		m_scene = new Engine::Scene();

		Engine::Entity entity = m_scene->CreateEntity();
		{
			Engine::Transform3DComponent& camComponentTransform
				= entity.AddComponent<Engine::Transform3DComponent>();
			camComponentTransform.SetPosition(MathLib::Vector3(-1.0f, 0.0f, 0.0f));
			camComponentTransform.LookAt(MathLib::Vector3(0.0f, 0.0f, 0.0f));
			entity.AddComponent<Engine::SceneCameraComponent>();
		}

		m_spriteEntity = new Engine::Entity(m_scene->CreateEntity());
		{
			Engine::Transform3DComponent& component
				= m_spriteEntity->AddComponent<Engine::Transform3DComponent>();
			component.SetScale(0.1f, 0.1f, 0.1f);
			m_spriteEntity->AddComponent<Engine::SpriteComponent>(
				Engine::AssetManager::GetTextures().Get(L"Assets/Textures/happy-face.png"));
		}
	}

	// TODO: May need to render on a separate thread
	// user presses and resizes the screen.

	void GlfwGame::Render()
	{
		Engine::GraphicsRenderer* graphicsRenderer = Engine::GraphicsRenderer::Get();
		graphicsRenderer->BeginFrame();

		m_scene->Render();

		// Draws the sprite based on a rect.
		{
			Engine::Transform3DComponent& transform =
				m_spriteEntity->GetComponent<Engine::Transform3DComponent>();
			Engine::SpriteComponent& spriteComponent =
				m_spriteEntity->GetComponent<Engine::SpriteComponent>();
			Engine::GraphicsRenderer2D::DrawRect(transform.GetTransformMatrix(),
				spriteComponent.color, spriteComponent.texture);
		}

		{
			Engine::GraphicsRenderer2D::DrawRect(MathLib::Vector2(20.0f, 0.0f),
				MathLib::Vector2::One, m_subTexture);
		}
	}
}