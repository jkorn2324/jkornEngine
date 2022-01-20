#include "EnginePCH.h"
#include "GraphicsRenderer3D.h"

#include "Material.h"
#include "ConstantBuffer.h"
#include "BufferLayout.h"

#include "Mesh.h"
#include "GraphicsRenderer.h"
#include "RenderingAPI.h"
#include "AssetManager.h"

namespace Engine
{

	struct GraphicsCubeMeshVertex
	{
		MathLib::Vector3 pos;
		MathLib::Vector3 normal;
		MathLib::Vector2 uv;
	};

	struct GraphicsObjectConstants
	{
		MathLib::Matrix4x4 c_objectToWorld = MathLib::Matrix4x4::Identity;
		MathLib::Vector3 c_objectPosition = MathLib::Vector3::Zero;

	private:
		float pad;
	};

	static bool s_initialized = false;

	static GraphicsObjectConstants s_objectConstants;
	static ConstantBuffer* s_objectConstantBuffer = nullptr;

	// TODO: Delete for material management.
	static Material* s_unlitMaterial = nullptr;

	static Mesh* s_cubeMesh = nullptr;
	static GraphicsCubeMeshVertex cubeMeshVertices[] =
	{
		{ MathLib::Vector3(-1.0f, -1.0f, -1.0f),
			MathLib::Vector3(-0.5f, -0.5f, -0.5f), MathLib::Vector2(0.0f, 0.0f) },
		{ MathLib::Vector3(-1.0f, -1.0f, 1.0f), 
			MathLib::Vector3(-0.5, -0.5, 0.5f), MathLib::Vector2(0.0f, 1.0f) },
		{ MathLib::Vector3(-1.0f, 1.0f, -1.0f), 
			MathLib::Vector3(-0.5f, 0.5f, 0.5f), MathLib::Vector2(1.0f, 0.0f) },
		{ MathLib::Vector3(-1.0f, 1.0f, 1.0f), 
			MathLib::Vector3(-0.5f, 0.5f, 0.5f), MathLib::Vector2(1.0f, 1.0f) },

		{ MathLib::Vector3(1.0f, -1.0f, -1.0f), 
			MathLib::Vector3(0.5f, -0.5f, -0.5f), MathLib::Vector2(0.0f, 1.0f) },
		{ MathLib::Vector3(1.0f, -1.0f, 1.0f),
			MathLib::Vector3(0.5f, 0.5f, -0.5f), MathLib::Vector2(0.0f, 0.0f) },
		{ MathLib::Vector3(1.0f, 1.0f, -1.0f), 
			MathLib::Vector3(0.5f, -0.5f, 0.5f), MathLib::Vector2(1.0f, 1.0f) },
		{ MathLib::Vector3(1.0f, 1.0f, 1.0f), 
			MathLib::Vector3(0.5f, 0.5f, 0.5f), MathLib::Vector2(1.0f, 0.0f) }
	};

	static uint32_t cubeMeshIndices[] =
	{
		1, 3, 0, 2,
		4, 6, 5, 7,
		6, 2, 7, 3,
		5, 1, 4, 0
	};

	void GraphicsRenderer3D::Init()
	{
		if (s_initialized)
		{
			return;
		}
		
		s_initialized = true;
		s_unlitMaterial = new Material({
			{ "c_materialColor", sizeof(MathLib::Vector4) }
		});

		// Initialize the constant buffer and buffer layout.
		{
			std::unique_ptr<Engine::BufferLayout> bufferLayout = BufferLayout::Create({
				{ "POSITION", offsetof(GraphicsCubeMeshVertex, pos),
					sizeof(MathLib::Vector3), Engine::BufferLayoutType::FLOAT3 },
				{ "NORMAL", offsetof(GraphicsCubeMeshVertex, uv),
					sizeof(MathLib::Vector2), Engine::BufferLayoutType::FLOAT3 },
				{ "TEXCOORD", offsetof(GraphicsCubeMeshVertex, uv),
					sizeof(MathLib::Vector2), Engine::BufferLayoutType::FLOAT2 }
			});
			s_unlitMaterial->SetShader(
				AssetManager::GetShaders().Load(
					L"Shaders/Unlit-VertUvPosShader.hlsl", bufferLayout));
			s_objectConstantBuffer = ConstantBuffer::Create(
				&s_objectConstants, sizeof(GraphicsObjectConstants));
		}

		// Generates the cube mesh, but should be stored in asset manager.
		{
			s_cubeMesh = new Mesh();
			s_cubeMesh->SetVertices(cubeMeshVertices,
				sizeof(GraphicsCubeMeshVertex), 
				sizeof(cubeMeshVertices) / sizeof(cubeMeshVertices[0]));
			s_cubeMesh->SetIndices(cubeMeshIndices,
				sizeof(cubeMeshIndices) / sizeof(cubeMeshIndices[0]));
		}
	}

	void GraphicsRenderer3D::Release()
	{
		delete s_cubeMesh;
		delete s_unlitMaterial;
		delete s_objectConstantBuffer;
	}

	void GraphicsRenderer3D::DrawMesh(const MathLib::Vector3& position, Mesh& mesh)
	{
		DrawMesh(MathLib::Matrix4x4::CreateTranslation(position), mesh);
	}

	void GraphicsRenderer3D::DrawMesh(const MathLib::Matrix4x4& transformMatrix, Mesh& mesh)
	{
		DrawMesh(transformMatrix, mesh, *s_unlitMaterial);
	}

	void GraphicsRenderer3D::DrawMesh(const MathLib::Matrix4x4& transformMatrix, Mesh& mesh, const Material& material)
	{
		// Bind the material.
		material.Bind();

		s_objectConstants.c_objectPosition = transformMatrix.GetTranslation();
		s_objectConstants.c_objectToWorld = transformMatrix;

		// Bind Object Constants.
		s_objectConstantBuffer->SetData(&s_objectConstants, sizeof(GraphicsObjectConstants));
		s_objectConstantBuffer->Bind(1, ConstantBufferFlags::PIXEL_SHADER
			| ConstantBufferFlags::VERTEX_SHADER);

		// Draws the mesh.
		GraphicsRenderer::GetRenderingAPI().Draw(
			mesh.GetVertexBuffer(), mesh.GetIndexBuffer());
	}

	void GraphicsRenderer3D::DrawCube(const MathLib::Matrix4x4& transformMatrix)
	{
		DrawCube(transformMatrix, *s_unlitMaterial);
	}
	
	void GraphicsRenderer3D::DrawCube(const MathLib::Matrix4x4& transformMatrix, const Material& material)
	{
		DrawMesh(transformMatrix, *s_cubeMesh, material);
	}
}