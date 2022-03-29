#include "EnginePCH.h"
#include "Mesh.h"

#include "StringUtils.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

#include "JsonFileWriter.h"
#include "AssetSerializer.h"

#include <memory>
#include <locale>
#include <codecvt>

namespace Engine
{
	Mesh::Mesh()
		: m_vertices(nullptr),
		m_indices(nullptr),
		m_vertexCount(0),
		m_indexCount(0),
		m_skinned(false),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr)
	{
	}

	Mesh::~Mesh()
	{
		delete m_vertexBuffer;
		delete m_indexBuffer;
		delete[] m_vertices;
		delete[] m_indices;
	}

	void Mesh::SetIndices(const uint32_t* indices, uint32_t indexCount)
	{
		if (m_indices != nullptr)
		{
			delete[] m_indices;
		}

		if (m_indexBuffer != nullptr)
		{
			delete m_indexBuffer;
		}

		m_indices = new std::uint32_t[indexCount];
		std::memcpy(m_indices, indices, 
			sizeof(std::uint32_t) * indexCount);
		m_indexBuffer = IndexBuffer::Create(indices, indexCount, sizeof(uint32_t));
	}

	void Mesh::SetVertices(const void* vertices, size_t stride, uint32_t vertexCount)
	{
		if (m_vertices != nullptr)
		{
			delete[] m_vertices;
		}

		if (m_vertexBuffer != nullptr)
		{
			delete m_vertexBuffer;
		}
		m_vertices = new char[vertexCount * stride];
		std::memcpy(m_vertices, vertices, 
			stride * vertexCount);
		m_vertexBuffer = VertexBuffer::Create(vertices,
			vertexCount, (uint32_t)stride);
	}

	uint32_t Mesh::GetNumVertices() const
	{
		return m_vertexCount;
	}

	uint32_t* Mesh::GetIndices() const
	{
		return m_indices;
	}

	uint32_t Mesh::GetNumIndices() const
	{
		return m_indexCount;
	}

	bool Mesh::DeserializeFromFile(Mesh& mesh, AssetDeserializationFileData& value)
	{
		// TODO: Read from a normal file.
		return true;
	}

	bool Mesh::SerializeToFile(Mesh& mesh, AssetSerializationFileData& metaData)
	{
		// TODO: Write to a file.
		return true;
	}

	bool Mesh::SerializeToMetaFile(Mesh& mesh, AssetSerializationMetaFileData& metaData)
	{
		// Writes to the meta file.
		JsonFileWriter metaFileWriter(metaData.metaFilePath);
		metaFileWriter.Write("GUID", metaData.guid);
		metaFileWriter.Flush();
		return true;
	}

	bool Mesh::DeserializeMetaFile(Mesh& mesh, AssetDeserializationMetaFileData& metaData)
	{
		// TODO: Implementation
		return true;
	}

	Mesh* Mesh::Create()
	{
		return new Mesh();
	}
}
