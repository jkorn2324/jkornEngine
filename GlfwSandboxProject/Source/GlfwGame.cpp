#include "GlfwGame.h"
#include "GraphicsRenderer.h"

#include "AssetManager.h"
#include "AssetCache.h"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "BufferLayout.h"

namespace GlfwSandbox
{
	GlfwGame::GlfwGame()
		: Layer("Game Layer"),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_spriteIndexBuffer(nullptr),
		m_spriteVertexBuffer(nullptr),
		m_cameraEntity(nullptr),
		m_spriteEntity(nullptr),
		m_scene(nullptr)
	{
		InitializeRenderBuffers();
		InitializeSceneComponents();
	}
	
	GlfwGame::~GlfwGame()
	{
		Engine::AssetManager::UncacheAssets();

		delete m_cameraConstantBuffer;
		delete m_spriteConstantBuffer;
		delete m_entityConstantBuffer;

		delete m_vertexBuffer;
		delete m_indexBuffer;
		delete m_spriteIndexBuffer;
		delete m_spriteVertexBuffer;
		delete m_cameraEntity;
		delete m_spriteEntity;
		delete m_scene;
	}

	void GlfwGame::OnUpdate(const Engine::Timestep& ts)
	{
		if (m_scene != nullptr)
		{
			m_scene->Update(ts);
		}
		Render();
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

	void GlfwGame::InitializeSceneComponents()
	{
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
	}

	// TODO: May need to render on a separate thread cuz screen gets weird if the 
	// user presses and resizes the screen.

	void GlfwGame::Render()
	{
		Engine::GraphicsRenderer* graphicsRenderer = Engine::GraphicsRenderer::Get();
		
		// Clears Back Buffer
		graphicsRenderer->BeginFrame();

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
				graphicsRenderer->SetConstantBuffer(0,
					m_cameraConstantBuffer,
					Engine::ConstantBufferFlags::VERTEX_SHADER | Engine::ConstantBufferFlags::PIXEL_SHADER);
			}
		}

		{
			Engine::Shader* shader =
				shaders.Get(L"Shaders/SpriteShader.hlsl");
			graphicsRenderer->SetShader(shader);

			Engine::SpriteComponent& sprite =
				m_spriteEntity->GetComponent<Engine::SpriteComponent>();
			graphicsRenderer->SetTexture(0, sprite.texture);
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
			graphicsRenderer->SetConstantBuffer(1,
				m_entityConstantBuffer,
				Engine::ConstantBufferFlags::VERTEX_SHADER | Engine::ConstantBufferFlags::PIXEL_SHADER);

			m_spriteConstants.c_spriteColor = sprite.color;
			m_spriteConstantBuffer->Update(
				&m_spriteConstants, sizeof(m_spriteConstants));
			graphicsRenderer->SetConstantBuffer(2,
				m_spriteConstantBuffer,
				Engine::ConstantBufferFlags::PIXEL_SHADER);

			graphicsRenderer->SetActiveVertexBuffer(m_spriteVertexBuffer);
			graphicsRenderer->SetActiveIndexBuffer(m_spriteIndexBuffer);
			graphicsRenderer->DrawActiveElements();
		}
		graphicsRenderer->EndFrame();
	}
}