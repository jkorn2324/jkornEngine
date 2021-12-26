#include "SandboxProjectPCH.h"
#include "Game.h"

#include "Source\Rendering\Shader.h"
#include "Source\Rendering\VertexBuffer.h"
#include "Source\Rendering\IndexBuffer.h"
#include "Source\Rendering\GraphicsRenderer.h"
#include "Source\Rendering\BufferLayout.h"
#include "Source\Rendering\ConstantBuffer.h"
#include "Source\Rendering\Texture.h"

#include "Source\Assets\AssetManager.h";
#include "Source\Assets\AssetCache.h";

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
		m_cameraEntity(nullptr)
	{
		m_graphicsRenderer = new Engine::GraphicsRenderer();
	}

	Game::~Game()
	{
		delete m_cameraEntity;
		delete m_scene;
		delete m_vertexBuffer;
		delete m_indexBuffer;

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
		m_cameraEntity->AddComponent<Engine::Transform3DComponent>();

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
		Engine::Shader* shader = shaders.Get(L"Shaders/TriangleShader.hlsl");

		m_graphicsRenderer->SetShader(shader);
		m_graphicsRenderer->SetActiveIndexBuffer(m_indexBuffer);
		m_graphicsRenderer->SetActiveVertexBuffer(m_vertexBuffer);

		m_graphicsRenderer->DrawActiveElements();

		m_graphicsRenderer->EndFrame();
	}

	void Game::InitializeRenderBuffers()
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

		Engine::AssetCache<Engine::Texture>& textureAssetCache =
			Engine::AssetManager::GetTextures();
		Engine::Texture* texture = textureAssetCache.Load(
			L"Assets/Textures/happy-face.png");
	}
}