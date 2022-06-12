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
		m_binormals(),
		m_tangents(),
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
		m_binormals.Release();
		m_tangents.Release();
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

	void Mesh::SetIndices(const std::vector<uint32_t>& indices)
	{
		SetIndices(&indices[0], indices.size());
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
			m_binormals.SetVertexCount(vertexCount);
			m_vertexColors.SetVertexCount(vertexCount);
		}
		m_vertexCount = vertexCount;

	}

	void Mesh::SetVertices(const std::vector<MathLib::Vector3>& vertices)
	{
		SetVertices(&vertices[0], vertices.size());
	}

	void Mesh::SetVertices(const MathLib::Vector3* vertices, size_t verticesCount)
	{
		m_vertices.SetVertices(vertices, verticesCount);
		// TODO: Better way of adding buffers
		m_vertexArray->AddVertexBuffer(m_vertices.GetVertexBuffer());
	}

	void Mesh::SetBinormals(const std::vector<MathLib::Vector3>& bitangents)
	{
		SetBinormals(&bitangents[0], bitangents.size());
	}

	void Mesh::SetBinormals(const MathLib::Vector3* bitangents, size_t tangentsSize)
	{
		m_binormals.SetVertices(bitangents, (uint32_t)tangentsSize);
		// TODO: Better way of adding buffers.
		m_vertexArray->AddVertexBuffer(m_binormals.GetVertexBuffer());
	}

	void Mesh::SetTangents(const MathLib::Vector3* tangents, size_t tangentsSize)
	{
		m_tangents.SetVertices(tangents, (uint32_t)tangentsSize);
		// TODO: Better way of adding buffers.
		m_vertexArray->AddVertexBuffer(m_tangents.GetVertexBuffer());
	}

	void Mesh::SetTangents(const std::vector<MathLib::Vector3>& tangents)
	{
		SetTangents(&tangents[0], tangents.size());
	}

	void Mesh::SetColors(const std::vector<MathLib::Vector4>& colors)
	{
		SetColors(&colors[0], colors.size());
	}

	void Mesh::SetColors(const MathLib::Vector4* vertexColors, size_t size)
	{
		m_vertexColors.SetVertices(vertexColors, (uint32_t)size);
		// TODO: Better way of adding buffers.
		m_vertexArray->AddVertexBuffer(m_vertexColors.GetVertexBuffer());
	}

	
	void Mesh::SetUVs(uint32_t index, const std::vector<MathLib::Vector2>& uvs)
	{
		SetUVs(index, &uvs[0], uvs.size());
	}

	void Mesh::SetUVs(uint32_t index, const MathLib::Vector2* uvs, size_t size)
	{
		if (index >= c_maxUVsCount) return;
		MeshBuffer<MathLib::Vector2>& uvIn = m_uvs[index];
		uvIn.SetVertices(uvs, (uint32_t)size);
		// TODO: Better way of adding buffers.
		m_vertexArray->AddVertexBuffer(uvIn.GetVertexBuffer());
	}

	void Mesh::SetNormals(const std::vector<MathLib::Vector3>& normals)
	{
		SetNormals(&normals[0], normals.size());
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
