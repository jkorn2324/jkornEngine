#include "EnginePCH.h"
#include "GraphicsRenderer3D.h"

#include "Material.h"
#include "ConstantBuffer.h"
#include "BufferLayout.h"

#include "Mesh.h"
#include "GraphicsRenderer.h"
#include "RenderingAPI.h"
#include "AssetManager.h"

#include "LightingComponents.h"

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

#pragma region lighting_structs

	struct GraphicsPointLightData
	{
	public:
		MathLib::Vector3 position;
	private:
		float fpad1;

	public:
		MathLib::Vector3 lightColor;
		float innerRadius = 1.0f;

		float outerRadius = 1.0f;
		float intensity = 1.0f;
		bool isEnabled = false;

		GraphicsPointLightData()
			: intensity(1.0f) { }

	private:
		bool bpad1, bpad2, bpad3;
		float fpad2;
	};

	struct GraphicsDirectionalLightData
	{
	public:
		MathLib::Vector3 direction;
	private:
		float fpad1;

	public:
		MathLib::Vector3 lightColor;
		float intensity = 1.0f;

		bool isEnabled = false;

		GraphicsDirectionalLightData()
			: intensity(1.0f) { }

	private:
		bool pad1, pad2, pad3;
		float fpad3, fpad4, fpad5;
	};

	struct LightingConstantBufferData
	{
		MathLib::Vector3 c_ambientLight = MathLib::Vector3::One;
	private:
		float lightingPad;

	public:
		GraphicsDirectionalLightData c_directionalLight;
		GraphicsPointLightData c_pointLightData[MAX_POINT_LIGHTS];
	};

	struct LightingData
	{
		uint32_t numPointLights = 0;
		bool lightsBound = false;
		LightingConstantBufferData constantBufferData;
	};

#pragma endregion

	static bool s_initialized = false;

	static GraphicsObjectConstants s_objectConstants;
	static ConstantBuffer* s_objectConstantBuffer = nullptr;

	static Mesh* s_cubeMesh = nullptr;
	static Material* s_defaultMaterial = nullptr;

	static LightingData s_lightingData;
	static ConstantBuffer* s_lightingConstantBuffer = nullptr;

	static GraphicsCubeMeshVertex cubeMeshVertices[] =
	{
		{ MathLib::Vector3(-0.5F, 0.5f, -0.5f),
			MathLib::Vector3(0.0f, 0.0f, -1.0f), MathLib::Vector2(0.0f, 0.0f) },
		{ MathLib::Vector3(0.5f, 0.5f, -0.5f),
			MathLib::Vector3(0.0f, 0.0f, -1.0f), MathLib::Vector2(1.0f, 0.0f) },
		{ MathLib::Vector3(0.5f, -0.5f, -0.5f),
			MathLib::Vector3(0.0f, 0.0f, -1.0f), MathLib::Vector2(1.0f, 1.0f) },
		{ MathLib::Vector3(-0.5f, -0.5f, -0.5f),
			MathLib::Vector3(0.0f, 0.0f, -1.0f), MathLib::Vector2(0.0f, 1.0f) },

		{ MathLib::Vector3(-0.5F, -0.5f, 0.5f),
			MathLib::Vector3(0.0f, 0.0f, 1.0f), MathLib::Vector2(0.0f, 0.0f) },
		{ MathLib::Vector3(0.5f, -0.5f, 0.5f),
			MathLib::Vector3(0.0f, 0.0f, 1.0f), MathLib::Vector2(1.0f, 0.0f) },
		{ MathLib::Vector3(0.5f, 0.5f, 0.5f),
			MathLib::Vector3(0.0f, 0.0f, 1.0f), MathLib::Vector2(1.0f, 1.0f) },
		{ MathLib::Vector3(-0.5f, 0.5f, 0.5f),
			MathLib::Vector3(0.0f, 0.0f, 1.0f), MathLib::Vector2(0.0f, 1.0f) },

		{ MathLib::Vector3(-0.5F, 0.5f, 0.5f),
			MathLib::Vector3(0.0f, 1.0f, 0.0f), MathLib::Vector2(0.0f, 0.0f) },
		{ MathLib::Vector3(0.5f, 0.5f, 0.5f),
			MathLib::Vector3(0.0f, 1.0f, 0.0f), MathLib::Vector2(1.0f, 0.0f) },
		{ MathLib::Vector3(0.5f, 0.5f, -0.5f),
			MathLib::Vector3(0.0f, 1.0f, 0.0f), MathLib::Vector2(1.0f, 1.0f) },
		{ MathLib::Vector3(-0.5f, 0.5f, -0.5f),
			MathLib::Vector3(0.0f, 1.0f, 0.0f), MathLib::Vector2(0.0f, 1.0f) },

		{ MathLib::Vector3(-0.5f, -0.5f, -0.5f),
			MathLib::Vector3(0.0f, -1.0f, 0.0f), MathLib::Vector2(0.0f, 0.0f) },
		{ MathLib::Vector3(0.5f, -0.5f, -0.5f),
			MathLib::Vector3(0.0f, -1.0f, 0.0f), MathLib::Vector2(1.0f, 0.0f) },
		{ MathLib::Vector3(0.5f, -0.5f, 0.5f),
			MathLib::Vector3(0.0f, -1.0f, 0.0f), MathLib::Vector2(1.0f, 1.0f) },
		{ MathLib::Vector3(-0.5f, -0.5f, 0.5f),
			MathLib::Vector3(0.0f, -1.0f, 0.0f), MathLib::Vector2(0.0f, 1.0f) },

		{ MathLib::Vector3(-0.5f, -0.5f, 0.5f),
			MathLib::Vector3(-1.0f, 0.0f, 0.0f), MathLib::Vector2(0.0f, 0.0f) },
		{ MathLib::Vector3(-0.5f, 0.5f, 0.5f),
			MathLib::Vector3(-1.0f, 0.0f, 0.0f), MathLib::Vector2(1.0f, 0.0f) },
		{ MathLib::Vector3(-0.5f, 0.5f, -0.5f),
			MathLib::Vector3(-1.0f, 0.0f, 0.0f), MathLib::Vector2(1.0f, 1.0f) },
		{ MathLib::Vector3(-0.5f, -0.5f, -0.5f),
			MathLib::Vector3(-1.0f, 0.0f, 0.0f), MathLib::Vector2(0.0f, 1.0f) },

		{ MathLib::Vector3(0.5f, -0.5f, -0.5f),
			MathLib::Vector3(1.0f, 0.0f, 0.0f), MathLib::Vector2(0.0f, 0.0f) },
		{ MathLib::Vector3(0.5f, 0.5f, -0.5f),
			MathLib::Vector3(1.0f, 0.0f, 0.0f), MathLib::Vector2(1.0f, 0.0f) },
		{ MathLib::Vector3(0.5f, 0.5f, 0.5f),
			MathLib::Vector3(1.0f, 0.0f, 0.0f), MathLib::Vector2(1.0f, 1.0f) },
		{ MathLib::Vector3(0.5f, -0.5f, 0.5f),
			MathLib::Vector3(1.0f, 0.0f, 0.0f), MathLib::Vector2(0.0f, 1.0f) }
	};

	static uint32_t cubeMeshIndices[] =
	{
		2, 1, 0,
		3, 2, 0,

		6, 5, 4,
		7, 6, 4,

		10, 9, 8,
		11, 10, 8,

		14, 13, 12,
		15, 14, 12,

		18, 17, 16,
		19, 18, 16,

		22, 21, 20,
		23, 22, 20
	};

	void GraphicsRenderer3D::Init()
	{
		if (s_initialized)
		{
			return;
		}
		
		s_initialized = true;
		// Initialize the lighting components.
		{
			s_lightingConstantBuffer = ConstantBuffer::Create(&s_lightingData.constantBufferData, 
				sizeof(s_lightingData.constantBufferData));
		}

		// Initializes the default material.
		{
			s_defaultMaterial = AssetManager::GetMaterials().Cache(L"Unlit-ColorUV",
				MaterialConstantsLayout {
					{"c_materialColor", sizeof(MathLib::Vector4) }
				});

			Engine::BufferLayout bufferLayout = {{
				{ "POSITION", offsetof(GraphicsCubeMeshVertex, pos),
					sizeof(MathLib::Vector3), Engine::BufferLayoutType::FLOAT3 },
				{ "NORMAL", offsetof(GraphicsCubeMeshVertex, normal),
					sizeof(MathLib::Vector3), Engine::BufferLayoutType::FLOAT3 },
				{ "TEXCOORD", offsetof(GraphicsCubeMeshVertex, uv),
					sizeof(MathLib::Vector2), Engine::BufferLayoutType::FLOAT2 }
			}};

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
		delete s_lightingConstantBuffer;
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

	void GraphicsRenderer3D::SetAmbientLight(const MathLib::Vector3& lightColor)
	{
		s_lightingData.constantBufferData.c_ambientLight = lightColor;
	}

	bool GraphicsRenderer3D::AddPointLight(const MathLib::Vector3& position, const PointLightComponent& pointLight)
	{
		if (s_lightingData.lightsBound)
		{
			s_lightingData.lightsBound = false;
			s_lightingData.numPointLights = 0;
		}

		if (s_lightingData.numPointLights >= MAX_POINT_LIGHTS)
		{
			return false;
		}
		
		if (!pointLight.enabled)
		{
			return true;
		}

		GraphicsPointLightData* pointLightData 
			= &s_lightingData.constantBufferData.c_pointLightData[s_lightingData.numPointLights];
		pointLightData->innerRadius = pointLight.innerRadius;
		pointLightData->outerRadius = pointLight.outerRadius;
		pointLightData->intensity = pointLight.lightIntensity;
		pointLightData->lightColor = pointLight.lightColor;
		pointLightData->position = position;
		pointLightData->isEnabled = pointLight.enabled;
		s_lightingData.numPointLights++;
		return true;
	}
	
	void GraphicsRenderer3D::SetDirectionalLight(const MathLib::Vector3& direction, 
		const DirectionalLightComponent& directionalLight)
	{
		GraphicsDirectionalLightData& directionalLightData
			= s_lightingData.constantBufferData.c_directionalLight;
		directionalLightData.direction = MathLib::Normalize(direction);
		directionalLightData.lightColor = directionalLight.lightColor;
		directionalLightData.intensity = directionalLight.lightIntensity;
		directionalLightData.isEnabled = directionalLight.enabled;
	}
	
	void GraphicsRenderer3D::BindLights()
	{
		s_lightingData.lightsBound = true;

		s_lightingConstantBuffer->SetData(&s_lightingData.constantBufferData,
			sizeof(s_lightingData.constantBufferData));
		s_lightingConstantBuffer->Bind(4,
			ConstantBufferFlags::VERTEX_SHADER | ConstantBufferFlags::PIXEL_SHADER);
	}
}