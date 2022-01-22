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

	static Mesh* s_cubeMesh = nullptr;
	static Material* s_defaultMaterial = nullptr;

	static GraphicsCubeMeshVertex cubeMeshVertices[] =
	{
		// Bottom Left Front Vertex
		{ MathLib::Vector3(-1.0f, -1.0f, -1.0f),
			MathLib::Vector3(-0.5f, -0.5f, -0.5f), MathLib::Vector2(0.0f, 0.0f) },
		// Bottom Left Back Vertex
		{ MathLib::Vector3(-1.0f, -1.0f, 1.0f), 
			MathLib::Vector3(-0.5, -0.5, 0.5f), MathLib::Vector2(0.0f, 1.0f) },
		// Top Left Front Vertex
		{ MathLib::Vector3(-1.0f, 1.0f, -1.0f), 
			MathLib::Vector3(-0.5f, 0.5f, 0.5f), MathLib::Vector2(1.0f, 0.0f) },
		// Top Left Back Vertex
		{ MathLib::Vector3(-1.0f, 1.0f, 1.0f), 
			MathLib::Vector3(-0.5f, 0.5f, 0.5f), MathLib::Vector2(1.0f, 1.0f) },
		// Bottom Right Front Vertex
		{ MathLib::Vector3(1.0f, -1.0f, -1.0f), 
			MathLib::Vector3(0.5f, -0.5f, -0.5f), MathLib::Vector2(0.0f, 1.0f) },
		// Bottom Right Back Vertex
		{ MathLib::Vector3(1.0f, -1.0f, 1.0f),
			MathLib::Vector3(0.5f, 0.5f, -0.5f), MathLib::Vector2(0.0f, 0.0f) },
		// Top Right Front Vertex
		{ MathLib::Vector3(1.0f, 1.0f, -1.0f), 
			MathLib::Vector3(0.5f, -0.5f, 0.5f), MathLib::Vector2(1.0f, 1.0f) },
		// Top Right Back Vertex
		{ MathLib::Vector3(1.0f, 1.0f, 1.0f), 
			MathLib::Vector3(0.5f, 0.5f, 0.5f), MathLib::Vector2(1.0f, 0.0f) }
	};

	static uint32_t cubeMeshIndices[] =
	{
		// Left Side
		1, 0, 2,
		2, 3, 1,
		// Right Side
		4, 5, 7,
		7, 6, 4,
		// Back Side
		5, 1, 3,
		3, 7, 5,
		// Front Side
		0, 4, 6,
		6, 2, 0,
		// Top Side
		3, 2, 6,
		6, 7, 3,
		// Bottom Side
		5, 4, 0,
		0, 1, 5
	};

	void GraphicsRenderer3D::Init()
	{
		if (s_initialized)
		{
			return;
		}
		
		s_initialized = true;
		{
			s_defaultMaterial = AssetManager::GetMaterials().Cache(L"Unlit-ColorUV",
				MaterialConstantsLayout {
					{"c_materialColor", sizeof(MathLib::Vector4) }
				});

			std::unique_ptr<Engine::BufferLayout> bufferLayout = BufferLayout::Create({
				{ "POSITION", offsetof(GraphicsCubeMeshVertex, pos),
					sizeof(MathLib::Vector3), Engine::BufferLayoutType::FLOAT3 },
				{ "NORMAL", offsetof(GraphicsCubeMeshVertex, uv),
					sizeof(MathLib::Vector2), Engine::BufferLayoutType::FLOAT3 },
				{ "TEXCOORD", offsetof(GraphicsCubeMeshVertex, uv),
					sizeof(MathLib::Vector2), Engine::BufferLayoutType::FLOAT2 }
			});

			Engine::Shader* shader = 
				AssetManager::GetShaders().Load(
					L"Shaders/Unlit-VertUvPosShader.hlsl", bufferLayout);
			s_defaultMaterial->SetShader(shader);
		}

		// Initialize the constant buffer and buffer layout.
		{
			s_objectConstantBuffer = ConstantBuffer::Create(
				&s_objectConstants, sizeof(GraphicsObjectConstants));
		}

		// Generates the cube mesh, but should be stored in asset manager.
		{
			s_cubeMesh = AssetManager::GetMeshes().Cache(L"DefaultCube");
			s_cubeMesh->SetVertices(cubeMeshVertices,
				sizeof(GraphicsCubeMeshVertex), 
				sizeof(cubeMeshVertices) / sizeof(cubeMeshVertices[0]));
			s_cubeMesh->SetIndices(cubeMeshIndices,
				sizeof(cubeMeshIndices) / sizeof(cubeMeshIndices[0]));
		}
	}

	void GraphicsRenderer3D::Release()
	{
		delete s_objectConstantBuffer;
	}

	void GraphicsRenderer3D::DrawMesh(const MathLib::Vector3& position, Mesh& mesh)
	{
		DrawMesh(MathLib::Matrix4x4::CreateTranslation(position), mesh);
	}

	void GraphicsRenderer3D::DrawMesh(const MathLib::Matrix4x4& transformMatrix, Mesh& mesh)
	{
		DrawMesh(transformMatrix, mesh, *s_defaultMaterial);
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
		DrawCube(transformMatrix, *s_defaultMaterial);
	}
	
	void GraphicsRenderer3D::DrawCube(const MathLib::Matrix4x4& transformMatrix, const Material& material)
	{
		DrawMesh(transformMatrix, *s_cubeMesh, material);
	}
}