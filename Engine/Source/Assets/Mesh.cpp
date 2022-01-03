#include "EnginePCH.h"
#include "Mesh.h"

#include <memory>

namespace Engine
{
	Mesh::Mesh()
		: m_vertices(nullptr),
		m_indices(nullptr),
		m_vertexCount(0),
		m_indexCount(0) { }

	Mesh::~Mesh()
	{
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
	
	bool Mesh::Load(const wchar_t* path)
	{
		// TODO: Implement the load function
		return false;
	}
}
