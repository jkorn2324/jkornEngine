#include "EnginePCH.h"
#include "GraphicsRenderer2D.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "BufferLayout.h"

#include "AssetManager.h"
#include "AssetCache.h"

#include "Texture.h"
#include "SubTexture.h"

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
	bool s_spriteVertexBufferDefault = true;

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
		0, 1, 2,
		2, 3, 0
	};

	static void DrawRectInternal(const MathLib::Matrix4x4& transform, const MathLib::Vector4& color, class Texture* texture)
	{
		GraphicsRenderer* graphicsRenderer = GraphicsRenderer::Get();

		// Bind Material.
		s_spriteMaterial->materialConstants.c_spriteColor = color;
		s_spriteMaterial->SetTexture(0, texture);
		s_spriteMaterial->Bind();

		// Bind Per Object Constants.
		s_spriteObjectConstantBuffer->SetData(&transform, sizeof(Mat4x4));
		graphicsRenderer->SetConstantBuffer(1, s_spriteObjectConstantBuffer,
			ConstantBufferFlags::PIXEL_SHADER | ConstantBufferFlags::VERTEX_SHADER);

		// Bind Vertex & Index Buffers & Draw.
		graphicsRenderer->SetActiveVertexBuffer(s_spriteVertexBuffer);
		graphicsRenderer->SetActiveIndexBuffer(s_spriteIndexBuffer);
		graphicsRenderer->DrawActiveElements();
	}


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
		// Used so that the vertex buffers are always set to a default rect.

		if (!s_spriteVertexBufferDefault)
		{
			s_spriteVertexBuffer->SetData(&vertices,
				sizeof(vertices) / sizeof(vertices[0]), sizeof(vertices[0]));
			s_spriteVertexBufferDefault = true;
		}
		
		Mat4x4 mat = transformMat;
		if (texture != nullptr)
		{
			mat = Mat4x4::CreateScale(texture->GetWidth(), texture->GetHeight(), 1.0f) * mat;
		}
		DrawRectInternal(mat, color, texture);
	}

	void GraphicsRenderer2D::DrawRect(const MathLib::Vector2& pos, const MathLib::Vector2 &scale, SubTexture* texture)
	{
		Mat4x4 mat = Mat4x4::CreateScale(scale.x, scale.y, 1.0f)
			* Mat4x4::CreateTranslation(pos.x, pos.y, 0.0f);
		DrawRect(mat, Vec4::One, texture);
	}
	
	void GraphicsRenderer2D::DrawRect(const MathLib::Matrix4x4& transformMat, const MathLib::Vector4& color, SubTexture* texture)
	{
		// Only changes UVs if it doesn't have defaults.
		if (texture->HasDefaultUVS())
		{
			DrawRect(transformMat, color, texture->GetTexture());
			return;
		}

		// Changes the UVs so that is based on the sub texture.
		GraphicsSpriteVertex verts[4];
		std::memcpy(&verts[0], &vertices[0], sizeof(vertices));
		verts[0].uv = texture->GetUVS()[0];
		verts[1].uv = texture->GetUVS()[1];
		verts[2].uv = texture->GetUVS()[2];
		verts[3].uv = texture->GetUVS()[3];

		s_spriteVertexBuffer->SetData(&verts, 4, sizeof(GraphicsSpriteVertex));
		s_spriteVertexBufferDefault = false;
		
		DrawRectInternal(Mat4x4::CreateScale(
			texture->GetSize().x, texture->GetSize().y, 1.0f) * transformMat, 
			color, texture->GetTexture());
	}

}