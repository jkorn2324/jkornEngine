#include "EnginePCH.h"
#include "Mesh.h"

#include "StringUtils.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "JsonFileWriter.h"
#include "AssetSerializer.h"


#include <memory>
#include <locale>
#include <codecvt>

namespace Engine
{
	Mesh::Mesh()
		: m_vertices(),
		m_normals(),
		m_bitangents(),
		m_vertexColors(),
		m_indices(nullptr),
		m_vertexCount(0),
		m_indexCount(0),
		m_skinned(false),
		m_vertexArray(nullptr)
	{
		VertexArray::Create(m_vertexArray);
	}

	Mesh::~Mesh()
	{
		m_vertices.Release();
		m_normals.Release();
		m_bitangents.Release();
		m_vertexColors.Release();
		m_vertexArray->ClearVertexBuffers();

		delete[] m_indices;
	}

	void Mesh::SetIndices(const uint32_t* indices, uint32_t indexCount)
	{
		m_indices = new std::uint32_t[indexCount];
		std::memcpy(m_indices, indices, 
			sizeof(std::uint32_t) * indexCount);
		
		// Creates the index buffer.
		std::shared_ptr<IndexBuffer> indexBuffer;
		IndexBuffer::Create(indexBuffer, indices, indexCount, sizeof(uint32_t));
		
		if (m_vertexArray != nullptr)
		{
			m_vertexArray->SetIndexBuffer(indexBuffer);
		}
	}

	void Mesh::SetVertexCount(uint32_t vertexCount)
	{
		if (m_vertexCount != vertexCount)
		{
			m_vertexArray->ClearVertexBuffers();
			m_uv0.SetVertexCount(vertexCount);
			m_vertices.SetVertexCount(vertexCount);
			m_normals.SetVertexCount(vertexCount);
			m_bitangents.SetVertexCount(vertexCount);
			m_vertexColors.SetVertexCount(vertexCount);
		}
		m_vertexCount = vertexCount;

	}

	void Mesh::SetVertices(const std::initializer_list<MathLib::Vector3>& vertices)
	{
		SetVertices(reinterpret_cast<const MathLib::Vector3*>(&vertices), vertices.size());
	}

	void Mesh::SetVertices(const MathLib::Vector3* vertices, size_t verticesCount)
	{
		m_vertices.SetVertices(vertices, verticesCount);
		// TODO: Better way of adding buffers
		m_vertexArray->AddVertexBuffer(m_vertices.GetVertexBuffer());
	}

	void Mesh::SetBitangents(const std::initializer_list<MathLib::Vector3>& bitangents)
	{
		SetBitangents(reinterpret_cast<const MathLib::Vector3*>(&bitangents), bitangents.size());
	}

	void Mesh::SetBitangents(const MathLib::Vector3* bitangents, size_t tangentsSize)
	{
		m_bitangents.SetVertices(bitangents, (uint32_t)tangentsSize);
		// TODO: Better way of adding buffers.
		m_vertexArray->AddVertexBuffer(m_bitangents.GetVertexBuffer());
	}

	void Mesh::SetColors(const std::initializer_list<MathLib::Vector4>& colors)
	{
		SetColors(reinterpret_cast<const MathLib::Vector4*>(&colors), colors.size());
	}

	void Mesh::SetColors(const MathLib::Vector4* vertexColors, size_t size)
	{
		m_vertexColors.SetVertices(vertexColors, (uint32_t)size);
		// TODO: Better way of adding buffers.
		m_vertexArray->AddVertexBuffer(m_vertexColors.GetVertexBuffer());
	}

	void Mesh::SetUV0(const std::initializer_list<MathLib::Vector2>& uvs)
	{
		SetUV0(reinterpret_cast<const MathLib::Vector2*>(&uvs), uvs.size());
	}
	
	void Mesh::SetUV0(const MathLib::Vector2* uvs, size_t size)
	{
		m_uv0.SetVertices(uvs, (uint32_t)size);
		// TODO: Better way of adding buffers.
		m_vertexArray->AddVertexBuffer(m_uv0.GetVertexBuffer());
	}

	void Mesh::SetNormals(const std::initializer_list<MathLib::Vector3>& normals)
	{
		SetNormals(reinterpret_cast<const MathLib::Vector3*>(&normals), normals.size());
	}

	void Mesh::SetNormals(const MathLib::Vector3* normals, size_t normalsSize)
	{
		m_normals.SetVertices(normals, normalsSize);
		// TODO: Better way of adding buffers.
		m_vertexArray->AddVertexBuffer(m_normals.GetVertexBuffer());
	}

	uint32_t Mesh::GetNumVertices() const
	{
		return m_vertexCount;
	}

	void Mesh::RecalculateNormals()
	{
		// TODO: Recalculate the normals.
	}

	uint32_t* Mesh::GetIndices() const
	{
		return m_indices;
	}

	uint32_t Mesh::GetIndexCount() const
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
