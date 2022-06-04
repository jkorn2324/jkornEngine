#include "EnginePCH.h"
#include "FBXConverter.h"

#include "Scene.h"
#include "Mesh.h"

#include <fbxsdk.h>

namespace Engine
{

	// Format for Mesh is Just
	// Position, UV, Normal


	static bool ConvertToMesh(FbxMesh* mesh, Mesh& outMesh)
	{
		if (!mesh) return false;

		// Triangulates the Mesh
		{
			fbxsdk::FbxGeometryConverter converter(mesh->GetFbxManager());
			converter.Triangulate(mesh, true);
		}

		// TODO: Implementation
#if 0
		// Stores the results per each vertex.
		std::vector<Mathlib::Vector3> outVertices(mesh->GetControlPointsCount());
		std::vector<uint32_t> outIndices;

		// Reads from the Fbx Mesh.
		{
			fbxsdk::FbxStringList uvSetList;
			mesh->GetUVSetNames(uvSetList);
			// TODO: Get More than one element uv set list.
			const FbxGeometryElementUV* uv = mesh->GetElementUV(uvSetList.GetStringAt(0));

			uint32_t polygonCount = mesh->GetPolygonCount();
			for (uint32_t i = 0; i < polygonCount; ++i)
			{
				uint32_t vertexIndices = mesh->GetPolygonSize(i);
				for (uint32_t vertexIndex = 0; vertexIndex < vertexIndices; ++vertexIndex)
				{
					uint32_t index = mesh->GetPolygonVertex(i, vertexIndex);
					size_t currentIndex = outVertices.size();

					fbxsdk::FbxVector4 normal;
					mesh->GetPolygonVertexNormal(i, vertexIndex, normal);

					MeshVertex vertex;
					if (uv)
					{
						uint32_t uvIndex = uv->GetIndexArray().GetAt(index);
						std::memcpy(&vertex.uv, &uv->GetDirectArray().GetAt(uvIndex), sizeof(MathLib::Vector2));
					}
					std::memcpy(&vertex.pos,
						&mesh->GetControlPointAt(index), sizeof(MathLib::Vector3));
					std::memcpy(&vertex.normal, &normal, sizeof(MathLib::Vector3));
				}
			}
		}

		// TODO: Implementation
		outMesh.SetVertices(reinterpret_cast<MeshVertex*>(&outVertices), 
			sizeof(MeshVertex), (uint32_t)outVertices.size());
		outMesh.SetIndices(reinterpret_cast<uint32_t*>(&outIndices), 
			(uint32_t)outIndices.size());
#endif
	}

	FBXConverter::FBXConverter(const char* fileName)
		: m_fbxManager(nullptr), m_fbxScene(nullptr)
	{
		if (!Initialize()) return;
		LoadFBXFile(fileName);
	}

	FBXConverter::~FBXConverter()
	{
		if (m_fbxScene)
		{
			m_fbxScene->Destroy();
		}
		if (m_fbxManager)
		{
			m_fbxManager->Destroy();
		}
		ZeroMemory(this, sizeof(FBXConverter));
	}

	bool FBXConverter::Initialize()
	{
		DebugAssert(m_fbxManager == nullptr, "FbxManager is not nullptr.");
		DebugAssert(m_fbxScene == nullptr, "FbxScene is not nullptr");

		m_fbxManager = fbxsdk::FbxManager::Create();
		if (m_fbxManager == nullptr) return false;

		fbxsdk::FbxIOSettings* settings = fbxsdk::FbxIOSettings::Create(m_fbxManager, IOSROOT);
		DebugAssert(settings, "Fbx settings shouldn't exist.");
		m_fbxManager->SetIOSettings(settings);
		return true;
	}

	void FBXConverter::LoadFBXFile(const char* fileName)
	{
		m_fbxScene = FbxScene::Create(m_fbxManager, "Imported Scene");
		if (m_fbxScene == nullptr) return;

		FbxImporter* fbxImporter = FbxImporter::Create(m_fbxManager, "Imported FBX");
		DebugAssert(fbxImporter, "Fbx Importer should exist.");
		if (!fbxImporter->Initialize(fileName, -1, m_fbxManager->GetIOSettings())
			&& !fbxImporter->Import(m_fbxScene))
		{
			// TODO: Failed.
			return;
		}
		fbxImporter->Destroy();
		// Comverts the scale to meters, which is our default unit of measurement.
		m_scale = (float)m_fbxScene->GetGlobalSettings().GetSystemUnit().GetConversionFactorTo(FbxSystemUnit::m);
	}

	void FBXConverter::ImportToScene(Scene& scene)
	{
		if (!IsValid()) return;

		FbxNode* rootNode = m_fbxScene->GetRootNode();
		const uint32_t numNodes = rootNode->GetChildCount();
		for (uint32_t i = 0; i < numNodes; ++i)
		{
			FbxNode* childNode = rootNode->GetChild(i);
			if (!childNode) continue;

			FbxMesh* mesh = childNode->GetMesh();
			if (mesh)
			{
				// TODO: Import mesh to scene.
			}
		}
	}
}