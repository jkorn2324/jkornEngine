#include "EnginePCH.h"
#include "GraphicsRenderer2D.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "BufferLayout.h"

#include "AssetManager.h"
#include "AssetCache.h"

#include "Texture.h"

namespace Engine
{
	using Mat4x4 = MathLib::Matrix4x4;
	using Vec3 = MathLib::Vector3;
	using Vec4 = MathLib::Vector4;
	using Vec2 = MathLib::Vector2;

	struct GraphicsSpriteConstants
	{
		MathLib::Vector4 c_spriteColor;

		GraphicsSpriteConstants()
			: c_spriteColor(MathLib::Vector4::One) { }
	};

	struct GraphicsSpriteVertex
	{
		Vec3 position;
		Vec2 uv;
	};

	static VertexBuffer* s_spriteVertexBuffer = nullptr;
	static IndexBuffer* s_spriteIndexBuffer = nullptr;

	static ConstantBuffer* s_spriteObjectConstantBuffer = nullptr;
	static Material<GraphicsSpriteConstants>* s_spriteMaterial = nullptr;

	static GraphicsSpriteVertex vertices[4] =
	{
		{ MathLib::Vector3(-0.5f, -0.5f, 0.0f), MathLib::Vector2(0.0f, 1.0f) },
		{ MathLib::Vector3(0.5f, -0.5f, 0.0f), MathLib::Vector2(1.0f, 1.0f) },
		{ MathLib::Vector3(0.5f, 0.5f, 0.0f), MathLib::Vector2(1.0f, 0.0f) },
		{ MathLib::Vector3(-0.5f, 0.5f, 0.0f), MathLib::Vector2(0.0f, 0.0f) }
	};

	static const std::uint32_t indices[6] =
	{
		2, 1, 0,
		0, 3, 2
	};

	void GraphicsRenderer2D::Init()
	{

		if (s_spriteVertexBuffer == nullptr)
		{
			s_spriteVertexBuffer = VertexBuffer::Create(
				&vertices, sizeof(vertices) / sizeof(vertices[0]), sizeof(GraphicsSpriteVertex));
		}
		if (s_spriteIndexBuffer == nullptr)
		{
			s_spriteIndexBuffer = IndexBuffer::Create(
				&indices, sizeof(indices) / sizeof(indices[0]), sizeof(uint32_t));
		}

		// Loads the Sprite Shader.
		// TODO: Make it so that these shaders can be tied to the engine.
		{
			Engine::BufferLayout bufferLayout({
					{ "POSITION", offsetof(GraphicsSpriteVertex, position),
						sizeof(MathLib::Vector3), Engine::BufferLayoutType::FLOAT3 },
					{ "TEXCOORD", offsetof(GraphicsSpriteVertex, uv),
						sizeof(MathLib::Vector2), Engine::BufferLayoutType::FLOAT2 }
				});

			Engine::AssetCache<Engine::Shader>& shaderAssetCache =
				Engine::AssetManager::GetShaders();
			Engine::Shader* shader = shaderAssetCache.Load<const Engine::BufferLayout&>(
				L"Shaders/SpriteShader.hlsl", bufferLayout);
			s_spriteMaterial = new Material<GraphicsSpriteConstants>();
			s_spriteMaterial->SetShader(shader);
		}

		Mat4x4 identity = Mat4x4::Identity;
		s_spriteObjectConstantBuffer = ConstantBuffer::Create(
			&identity, sizeof(Mat4x4));
	}

	void GraphicsRenderer2D::Release()
	{
		delete s_spriteObjectConstantBuffer;
		delete s_spriteMaterial;
		delete s_spriteVertexBuffer;
		delete s_spriteIndexBuffer;
	}

	void GraphicsRenderer2D::DrawRect(const MathLib::Vector2& pos, const MathLib::Vector2& scale, Texture* texture)
	{
		Mat4x4 mat = Mat4x4::CreateScale(scale.x, scale.y, 1.0f)
			* Mat4x4::CreateTranslation(pos.x, pos.y, 0.0f);
		DrawRect(mat, Vec4::One, texture);
	}
	
	void GraphicsRenderer2D::DrawRect(const MathLib::Matrix4x4& transformMat, const MathLib::Vector4& color, Texture* texture)
	{
		GraphicsRenderer* graphicsRenderer = GraphicsRenderer::Get();
		
		Mat4x4 mat = transformMat;
		if (texture != nullptr)
		{
			mat = Mat4x4::CreateScale(texture->GetWidth(), texture->GetHeight(), 1.0f) * mat;
		}
		
		// Bind Material.
		s_spriteMaterial->materialConstants.c_spriteColor = color;
		s_spriteMaterial->SetTexture(0, texture);
		s_spriteMaterial->Bind();

		// Bind Per Object Constants.
		s_spriteObjectConstantBuffer->SetData(&mat, sizeof(Mat4x4));
		graphicsRenderer->SetConstantBuffer(1, s_spriteObjectConstantBuffer,
			ConstantBufferFlags::PIXEL_SHADER | ConstantBufferFlags::VERTEX_SHADER);

		// Bind Vertex & Index Buffers & Draw.
		graphicsRenderer->SetActiveVertexBuffer(s_spriteVertexBuffer);
		graphicsRenderer->SetActiveIndexBuffer(s_spriteIndexBuffer);
		graphicsRenderer->DrawActiveElements();
	}
}