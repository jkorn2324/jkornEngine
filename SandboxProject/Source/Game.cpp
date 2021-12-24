#include "SandboxProjectPCH.h"
#include "Game.h"
#include "Source\Rendering\Shader.h"
#include "Source\Rendering\VertexBuffer.h"
#include "Source\Rendering\IndexBuffer.h"
#include "Source\Rendering\GraphicsRenderer.h"
#include "Source\Rendering\BufferLayout.h"
#include "Source\Rendering\ConstantBuffer.h"

#include "Source\Entities\Entity.h"
#include "Source\Components\CameraComponent.h"

namespace DirectXTestProject
{

	Game::Game()
		: m_indexBuffer(nullptr), 
		m_vertexBuffer(nullptr),
		m_shader(nullptr),
		m_graphicsRenderer(nullptr),
		m_cameraEntity(nullptr)
	{
		m_graphicsRenderer = new Engine::GraphicsRenderer();
	}

	Game::~Game()
	{
		delete m_cameraEntity;
		delete m_shader;
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

		// Creates an entity with a camera component.
		m_cameraEntity = new Engine::Entity();
		Engine::CameraComponent* camera = new Engine::CameraComponent(m_cameraEntity);

		return true;
	}

	void Game::Update(float deltaTime)
	{
		m_cameraEntity->Update(deltaTime);
	}

	void Game::Render()
	{
		// Clear back buffer, draw viewport.
		m_graphicsRenderer->BeginFrame();

		m_graphicsRenderer->SetShader(m_shader);
		m_graphicsRenderer->SetActiveIndexBuffer(m_indexBuffer);
		m_graphicsRenderer->SetActiveVertexBuffer(m_vertexBuffer);

		// Sets the camera component.
		Engine::CameraComponent* cameraComponent = m_cameraEntity->GetComponent<
			Engine::CameraComponent>();
		m_graphicsRenderer->SetConstantBuffer(0, cameraComponent->m_constantBuffer,
			Engine::ConstantBufferFlags::VERTEX_SHADER | Engine::ConstantBufferFlags::PIXEL_SHADER);
	
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

		m_shader = new Engine::Shader();
		m_shader->Load(L"Shaders/TriangleShader.hlsl", bufferLayout);
	}
}