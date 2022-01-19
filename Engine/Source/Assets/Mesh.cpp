#include "EnginePCH.h"
#include "Mesh.h"
#include "StringUtils.h"

#include <memory>
#include <fbxsdk.h>

#include <locale>
#include <codecvt>

namespace Engine
{

#pragma region fbx

	static FbxManager* s_fbxManager = nullptr;

	static void CreateFBXSettings()
	{
		if (s_fbxManager != nullptr)
		{
			return;
		}
		s_fbxManager = FbxManager::Create();
		FbxIOSettings* settings = FbxIOSettings::Create(
			s_fbxManager, IOSROOT);
		s_fbxManager->SetIOSettings(settings);
	}

	static FbxScene* LoadScene(const std::wstring& scenePath, 
		const std::string& name)
	{
		CreateFBXSettings();

		std::string converted;
		WStringToString(scenePath, converted);
		FbxImporter* importer = FbxImporter::Create(s_fbxManager, "");
		if (!importer->Initialize(converted.c_str(), -1,
			s_fbxManager->GetIOSettings()))
		{
			const char* errorString = importer->GetStatus().GetErrorString();
			DebugAssert(false, "Mesh Failed to load.");
			return nullptr;
		}
		FbxScene* outScene = FbxScene::Create(s_fbxManager, name.c_str());
		importer->Import(outScene);
		importer->Destroy();
		return outScene;
;	}

#pragma endregion

	uint32_t s_numMeshes = 0;

	Mesh::Mesh()
		: m_vertices(nullptr),
		m_indices(nullptr),
		m_vertexCount(0),
		m_indexCount(0) 
	{
		s_numMeshes++;
	}

	Mesh::~Mesh()
	{
		s_numMeshes--;

		if (s_numMeshes <= 0
			&& s_fbxManager != nullptr)
		{
			s_fbxManager->Destroy();
			s_fbxManager = nullptr;
		}

		delete[] m_vertices;
		delete[] m_indices;
	}

	void Mesh::SetIndices(const std::uint32_t* indices, std::uint32_t indexCount)
	{
		if (m_indices != nullptr)
		{
			delete[] m_indices;
		}
		m_indices = new std::uint32_t[indexCount];
		std::memcpy(m_indices, indices, 
			sizeof(std::uint32_t) * indexCount);
	}

	void Mesh::SetVertices(const MeshVertex* vertices, std::uint32_t vertexCount)
	{
		if (m_vertices != nullptr)
		{
			delete[] m_vertices;
		}
		m_vertices = new MeshVertex[vertexCount];
		std::memcpy(m_vertices, vertices,
			sizeof(MeshVertex) * vertexCount);
	}

	MeshVertex* Mesh::GetVertices() const
	{
		return m_vertices;
	}

	std::uint32_t Mesh::GetNumVertices() const
	{
		return m_vertexCount;
	}

	std::uint32_t* Mesh::GetIndices() const
	{
		return m_indices;
	}

	std::uint32_t Mesh::GetNumIndices() const
	{
		return m_indexCount;
	}

	bool Mesh::Load(const std::wstring& path)
	{
		std::wstring pathOut =
			path.substr(path.find_last_of(L'.'));
		if (pathOut == L".FBX"
			|| pathOut == L".fbx")
		{
			return LoadFBX(path);
		}
		return false;
	}

	bool Mesh::LoadFBX(const std::wstring& path)
	{
		// TODO: Implementation
#if 0
		FbxScene* fbxScene
			= LoadScene(path, "Mesh");
		if (fbxScene == nullptr)
		{
			return false;
		}

		fbxsdk::FbxNode* rootNode = fbxScene->GetRootNode();
		if (rootNode != nullptr)
		{
			int children = rootNode->GetChildCount();
			for (int i = 0; i < children; i++)
			{
				// TODO: Move fbx importing to another.
			}
		}

		fbxScene->Destroy();
#endif
		return false;
	}

	Mesh* Mesh::StaticLoad(const std::wstring& path)
	{
		Mesh* mesh = new Mesh();
		if (!mesh->Load(path.c_str()))
		{
			delete mesh;
			return nullptr;
		}
		return mesh;
	}
}
