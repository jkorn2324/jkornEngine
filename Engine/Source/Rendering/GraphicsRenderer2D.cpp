#include "EnginePCH.h"
#include "GraphicsRenderer2D.h"

#include "RenderingAPI.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "BufferLayout.h"

#include "AssetManager.h"
#include "AssetCache.h"
#include "Profiler.h"

#include "Texture.h"
#include "SubTexture.h"

namespace Engine
{
	using Mat4x4 = MathLib::Matrix4x4;
	using Vec3 = MathLib::Vector3;
	using Vec4 = MathLib::Vector4;
	using Vec2 = MathLib::Vector2;

	struct GraphicsObjectConstants
	{
		MathLib::Matrix4x4 c_objectToWorld;
		MathLib::Vector3 c_worldPosition;

	private:
		float pad;
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
	static GraphicsObjectConstants s_spriteObjectConstants;

	static AssetRef<Material> s_spriteMaterial;
	static AssetRef<Shader> s_spriteShader;

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

	static void DrawRectInternal(const MathLib::Matrix4x4& transform, const MathLib::Vector4& color, const AssetRef<Texture>& texture)
	{
		PROFILE_SCOPE(DrawRectInternal, Rendering);

		// Clears the texture at the initial texture slot.
		GraphicsRenderer::GetRenderingAPI().ClearTexture(0);

		// Bind Material.
		MaterialConstants& constants = s_spriteMaterial->GetMaterialConstants();
		constants.SetMaterialConstant("c_spriteColor", color);
		s_spriteMaterial->SetTexture(0, texture);
		s_spriteMaterial->Bind();

		s_spriteObjectConstants.c_objectToWorld = transform;
		s_spriteObjectConstants.c_worldPosition = transform.GetTranslation();

		// Bind Per Object Constants.
		s_spriteObjectConstantBuffer->SetData(&s_spriteObjectConstants, 
			sizeof(GraphicsObjectConstants));

		s_spriteObjectConstantBuffer->Bind(1, ConstantBufferFlags::PIXEL_SHADER
			| ConstantBufferFlags::VERTEX_SHADER);

		GraphicsRenderer::Draw(s_spriteVertexBuffer,
			s_spriteIndexBuffer);
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
		{
			Engine::BufferLayout bufferLayout = 
				{{ "POSITION", offsetof(GraphicsSpriteVertex, position),
						sizeof(MathLib::Vector3), Engine::BufferLayoutType::FLOAT3 },
					{ "TEXCOORD", offsetof(GraphicsSpriteVertex, uv),
						sizeof(MathLib::Vector2), Engine::BufferLayoutType::FLOAT2 }
				};

			Engine::TypedAssetManager<Engine::Shader>& shaderAssetCache =
				Engine::AssetManager::GetShaders();
			shaderAssetCache.Load<const Engine::BufferLayout&>(s_spriteShader,
				L"Shaders/SpriteShader.hlsl", bufferLayout);

			Engine::TypedAssetManager<Engine::Material>& materialCache =
				Engine::AssetManager::GetMaterials();
			// Defines the sprite material.
			materialCache.Cache(s_spriteMaterial, 
				L"SpriteMaterial", 
				MaterialConstantsLayout {
					{ "c_spriteColor", LayoutType_Vector4 }
				});
			s_spriteMaterial->SetShader(s_spriteShader);
		}

		s_spriteObjectConstantBuffer = ConstantBuffer::Create(
			&s_spriteObjectConstants, sizeof(GraphicsObjectConstants));
	}

	void GraphicsRenderer2D::Release()
	{
		delete s_spriteObjectConstantBuffer;
		delete s_spriteVertexBuffer;
		delete s_spriteIndexBuffer;
	}

	void GraphicsRenderer2D::DrawRect(const MathLib::Vector2& pos, const MathLib::Vector2& scale, const AssetRef<Texture>& texture)
	{
		Mat4x4 mat = Mat4x4::CreateScale(scale.x, scale.y, 1.0f)
			* Mat4x4::CreateTranslation(pos.x, pos.y, 0.0f);
		DrawRect(mat, Vec4::One, texture);
	}
	
	void GraphicsRenderer2D::DrawRect(const MathLib::Matrix4x4& transformMat, const MathLib::Vector4& color, const AssetRef<Texture>& texture)
	{
		// Used so that the vertex buffers are always set to a default rect.

		if (!s_spriteVertexBufferDefault)
		{
			s_spriteVertexBuffer->SetData(&vertices,
				sizeof(vertices) / sizeof(vertices[0]), sizeof(vertices[0]));
			s_spriteVertexBufferDefault = true;
		}
		
		Mat4x4 mat = transformMat;
		if (texture)
		{
			mat = Mat4x4::CreateScale((float)texture->GetWidth(), (float)texture->GetHeight(), 1.0f) * mat;
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