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

	static const uint32_t c_maxUVsCount = 8;

	Mesh::Mesh()
		: m_vertices(),
		m_normals(),
		m_bitangents(),
		m_vertexColors(),
		m_indices(nullptr),
		m_vertexCount(0),
		m_indexCount(0),
		m_skinned(false),
		m_vertexArray(nullptr),
		m_uvs(nullptr)
	{
		m_uvs = new MeshBuffer<MathLib::Vector2>[c_maxUVsCount];
		VertexArray::Create(m_vertexArray);
	}

	Mesh::~Mesh()
	{
		m_vertices.Release();
		m_normals.Release();
		m_bitangents.Release();
		m_vertexColors.Release();
		m_vertexArray->ClearVertexBuffers();

		// Releases the UVs.
		for (uint32_t i = 0; i < c_maxUVsCount; ++i)
		{
			MeshBuffer<MathLib::Vector2>& uvsCount = m_uvs[i];
			uvsCount.Release();
		}

		delete[] m_uvs;
		delete[] m_indices;

		m_uvs = nullptr;
		m_indices = nullptr;
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
			if (m_vertexArray->GetNumVertexBuffers() > 0)
			{
				m_vertexArray->ClearVertexBuffers();
			}
			// Sets the vertex count.
			for (uint32_t i = 0; i < c_maxUVsCount; ++i)
			{
				m_uvs[i].SetVertexCount(vertexCount);
			}
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

	void Mesh::SetUVs(uint32_t index, const std::initializer_list<MathLib::Vector2>& uvs)
	{
		SetUVs(index, reinterpret_cast<const MathLib::Vector2*>(&uvs), uvs.size());
	}
	
	void Mesh::SetUVs(uint32_t index, const MathLib::Vector2* uvs, size_t size)
	{
		if (index >= c_maxUVsCount) return;
		MeshBuffer<MathLib::Vector2>& uvIn = m_uvs[index];
		uvIn.SetVertices(uvs, (uint32_t)size);
		// TODO: Better way of adding buffers.
		m_vertexArray->AddVertexBuffer(uvIn.GetVertexBuffer());
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

	void Mesh::RefreshVertexArray()
	{

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
