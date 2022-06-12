#include "EnginePCH.h"
#include "FBXConverter.h"

#include "Scene.h"
#include "Mesh.h"

#include <fbxsdk.h>

namespace Engine
{

	// Format for Mesh is Just
	// Position, UV, Normal


	static bool ConvertToMesh(fbxsdk::FbxMesh* mesh, Mesh& outMesh)
	{
		if (!mesh) return false;

		// Triangulates the Mesh
		{
			fbxsdk::FbxGeometryConverter converter(mesh->GetFbxManager());
			converter.Triangulate(mesh, true);
		}

		// Stores the results per each vertex.
		std::vector<MathLib::Vector3> outVertices(mesh->GetControlPointsCount());
		std::vector<MathLib::Vector3> outNormals(mesh->GetControlPointsCount());
		std::vector<MathLib::Vector3> outBinormals(mesh->GetControlPointsCount());
		std::vector<MathLib::Vector3> outTangents(mesh->GetControlPointsCount());


		uint32_t polygonCount = mesh->GetPolygonCount();

		std::vector<uint32_t> outIndices;
		// Reads from the Fbx Mesh.
		{
			{
				std::vector<MathLib::Vector2> uvsTempArr;
				fbxsdk::FbxStringList uvSetList;
				mesh->GetUVSetNames(uvSetList);
				for (uint32_t uv = 0; uv < uvSetList.GetCount(); ++uv)
				{
					uvsTempArr.clear();
					FbxGeometryElementUV* uvElement = mesh->GetElementUV(uvSetList.GetStringAt(uv));
					if (!uvElement) break;

					for (uint32_t i = 0; i < polygonCount; ++i)
					{
						uint32_t vertexIndices = mesh->GetPolygonSize(i);
						for (uint32_t vertexIndex = 0; vertexIndex < vertexIndices; ++vertexIndex)
						{
							uvsTempArr.push_back(*reinterpret_cast<MathLib::Vector2*>(
								&uvElement->GetDirectArray().GetAt(vertexIndex)));
						}
					}
					outMesh.SetUVs(uv, uvsTempArr);
				}
			}
			
			for (uint32_t i = 0; i < polygonCount; ++i)
			{
				uint32_t vertexIndices = mesh->GetPolygonSize(i);
				for (uint32_t vertexIndex = 0; vertexIndex < vertexIndices; ++vertexIndex)
				{
					uint32_t index = mesh->GetPolygonVertex(i, vertexIndex);

					outVertices.push_back(*reinterpret_cast<MathLib::Vector3*>(
						&mesh->GetControlPointAt(index)));
					outNormals.push_back(*reinterpret_cast<MathLib::Vector3*>(
						mesh->GetElementNormal(index)));
					outBinormals.push_back(*reinterpret_cast<MathLib::Vector3*>(
						mesh->GetElementBinormal(index)));
					outTangents.push_back(*reinterpret_cast<MathLib::Vector3*>(
						mesh->GetElementTangent(index)));
					outIndices.push_back(index);
				}
			}
		}
		outMesh.SetVertices(outVertices);
		outMesh.SetIndices(outIndices);
		outMesh.SetNormals(outNormals);
		outMesh.SetBinormals(outBinormals);
		outMesh.SetTangents(outTangents);
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