#include "SandboxProjectPCH.h"
#include "Game.h"

#include "Source\Rendering\Shader.h"
#include "Source\Rendering\VertexBuffer.h"
#include "Source\Rendering\IndexBuffer.h"
#include "Source\Rendering\GraphicsRenderer.h"
#include "Source\Rendering\BufferLayout.h"
#include "Source\Rendering\ConstantBuffer.h"
#include "Source\Rendering\Texture.h"
#include "Source\Rendering\Camera.h"

#include "Source\Assets\AssetManager.h"
#include "Source\Assets\AssetCache.h"

#include "Source\Scene\Scene.h"
#include "Source\Scene\Entity.h"
#include "Source\Scene\Components.h"

namespace DirectXTestProject
{

	Game::Game()
		: m_indexBuffer(nullptr), 
		m_vertexBuffer(nullptr),
		m_graphicsRenderer(nullptr),
		m_scene(nullptr),
		m_cameraEntity(nullptr),
		m_spriteEntity(nullptr),
		m_spriteVertexBuffer(nullptr),
		m_spriteIndexBuffer(nullptr),
		m_cameraConstantBuffer(nullptr),
		m_spriteConstantBuffer(nullptr),
		m_cameraConstants(),
		m_entityConstantBuffer(nullptr),
		m_entityConstants()
	{
		m_graphicsRenderer = new Engine::GraphicsRenderer();
	}

	Game::~Game()
	{
		Engine::AssetManager::UncacheAssets();

		delete m_cameraEntity;
		delete m_spriteEntity;
		delete m_scene;

		delete m_vertexBuffer;
		delete m_indexBuffer;

		delete m_spriteVertexBuffer;
		delete m_spriteIndexBuffer;

		delete m_cameraConstantBuffer;
		delete m_entityConstantBuffer;
		delete m_spriteConstantBuffer;

		delete m_graphicsRenderer;
	}

	bool Game::Initialize(const HWND& window)
	{
		if (!m_graphicsRenderer->Initialize(window))
		{
			return false;
		}
		InitializeRenderBuffers();

		m_scene = new Engine::Scene();

		m_cameraEntity = new Engine::Entity(m_scene->CreateEntity());
		{
			Engine::Transform3DComponent& camComponentTransform
				= m_cameraEntity->AddComponent<Engine::Transform3DComponent>();
			camComponentTransform.SetPosition(MathLib::Vector3(0.0f, 0.0f, -20.0f));
			camComponentTransform.LookAt(MathLib::Vector3(0.0f, 0.0f, 0.0f));
			m_cameraEntity->AddComponent<Engine::SceneCameraComponent>();
		}

		m_spriteEntity = new Engine::Entity(m_scene->CreateEntity());
		{
			Engine::Transform3DComponent& component
				= m_spriteEntity->AddComponent<Engine::Transform3DComponent>();
			component.SetScale(0.1f, 0.1f, 0.1f);
			m_spriteEntity->AddComponent<Engine::SpriteComponent>(
				Engine::AssetManager::GetTextures().Get(L"Assets/Textures/happy-face.png"));
		}
		return true;
	}

	void Game::Update(float deltaTime)
	{
		if (m_scene != nullptr)
		{
			m_scene->Update(deltaTime);
		}
	}


	void Game::Render()
	{
		// Clear back buffer, draw viewport.
		m_graphicsRenderer->BeginFrame();

		// Gets the shader.
		Engine::AssetCache<Engine::Shader>& shaders = Engine::AssetManager::GetShaders();

		// Updates the camera constants
		{
			Engine::Camera* camera = m_scene->GetCamera();
			if (camera != nullptr)
			{
				auto mat = camera->GetViewMatrix();
				mat.Invert();

				m_cameraConstants.c_cameraPosition = mat.GetTranslation();
				m_cameraConstants.c_viewProjection =
					camera->GetViewProjectionMatrix();

				m_cameraConstantBuffer->Update(&m_cameraConstants,
					sizeof(m_cameraConstants));
				m_graphicsRenderer->SetConstantBuffer(0,
					m_cameraConstantBuffer,
					Engine::ConstantBufferFlags::VERTEX_SHADER | Engine::ConstantBufferFlags::PIXEL_SHADER);
			}
		}

		{
			Engine::Shader* shader =
				shaders.Get(L"Shaders/SpriteShader.hlsl");
			m_graphicsRenderer->SetShader(shader);

			Engine::SpriteComponent& sprite =
				m_spriteEntity->GetComponent<Engine::SpriteComponent>();
			m_graphicsRenderer->SetTexture(0, sprite.texture);
			Engine::Transform3DComponent& transform =
				m_spriteEntity->GetComponent<Engine::Transform3DComponent>();
			m_entityConstants.c_objectToWorld = transform.GetTransformMatrix();

			if (sprite.texture != nullptr)
			{
				m_entityConstants.c_objectToWorld = MathLib::Matrix4x4::CreateScale(
					(float)sprite.texture->GetWidth(), (float)sprite.texture->GetHeight(), 1.0f)
					* m_entityConstants.c_objectToWorld;
			}

			m_entityConstantBuffer->Update(&m_entityConstants,
				sizeof(m_entityConstants));
			m_graphicsRenderer->SetConstantBuffer(1,
				m_entityConstantBuffer,
				Engine::ConstantBufferFlags::VERTEX_SHADER | Engine::ConstantBufferFlags::PIXEL_SHADER);

			m_spriteConstants.c_spriteColor = sprite.color;
			m_spriteConstantBuffer->Update(
				&m_spriteConstants, sizeof(m_spriteConstants));
			m_graphicsRenderer->SetConstantBuffer(2,
				m_spriteConstantBuffer,
				Engine::ConstantBufferFlags::PIXEL_SHADER);

			m_graphicsRenderer->SetActiveVertexBuffer(m_spriteVertexBuffer);
			m_graphicsRenderer->SetActiveIndexBuffer(m_spriteIndexBuffer);
			m_graphicsRenderer->DrawActiveElements();
		}

		m_graphicsRenderer->EndFrame();
	}

	void Game::InitializeRenderBuffers()
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

			m_vertexBuffer = new Engine::VertexBuffer(
				&vertices[0], sizeof(vertices) / sizeof(vertices[0]), sizeof(vertices[0]));
			m_indexBuffer = new Engine::IndexBuffer(
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

		{
			SpriteShaderVertex vertices[4] =
			{
				{ MathLib::Vector3(-0.5f, -0.5f, 0.0f), MathLib::Vector2(0.0f, 1.0f) },
				{ MathLib::Vector3(0.5f, -0.5f, 0.0f), MathLib::Vector2(1.0f, 1.0f) },
				{ MathLib::Vector3(0.5f, 0.5f, 0.0f), MathLib::Vector2(1.0f, 0.0f) },
				{ MathLib::Vector3(-0.5f, 0.5f, 0.0f), MathLib::Vector2(0.0f, 0.0f) }
			};

			// Vertices are constructed in clockwise-order.
			std::uint16_t indices[6] =
			{
				2, 1, 0,
				0, 3, 2
			};

			m_spriteVertexBuffer = new Engine::VertexBuffer(
				&vertices[0], sizeof(vertices) / sizeof(vertices[0]), sizeof(vertices[0]));
			m_spriteIndexBuffer = new Engine::IndexBuffer(
				&indices[0], sizeof(indices) / sizeof(indices[0]), sizeof(indices[0]));

			Engine::AssetCache<Engine::Texture>& textureAssetCache =
				Engine::AssetManager::GetTextures();
			textureAssetCache.Load(
				L"Assets/Textures/happy-face.png");

			Engine::BufferLayout bufferLayout({
				{ "POSITION", offsetof(SpriteShaderVertex, position), 
					sizeof(MathLib::Vector3), Engine::BufferLayoutType::FLOAT3 },
				{ "TEXCOORD", offsetof(SpriteShaderVertex, uv),
					sizeof(MathLib::Vector2), Engine::BufferLayoutType::FLOAT2 }
			});

			Engine::AssetCache<Engine::Shader>& shaderAssetCache =
				Engine::AssetManager::GetShaders();
			shaderAssetCache.Load<const Engine::BufferLayout&>(
				L"Shaders/SpriteShader.hlsl", bufferLayout);
		}

		m_cameraConstantBuffer = new Engine::ConstantBuffer(
			&m_cameraConstants, sizeof(m_cameraConstants));
		m_entityConstantBuffer = new Engine::ConstantBuffer(
			&m_entityConstants, sizeof(m_entityConstants));
		m_spriteConstantBuffer = new Engine::ConstantBuffer(
			&m_spriteConstants, sizeof(m_spriteConstants));
	}
}