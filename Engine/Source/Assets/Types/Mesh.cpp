#include "EnginePCH.h"
#include "Mesh.h"

#include "StringUtils.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "JsonFileWriter.h"

#include "Memory.h"

#include <memory>
#include <locale>
#include <codecvt>

namespace Engine
{


	namespace
	{
		const BufferLayout c_defaultMeshLayoutInternal =
		{
			BufferLayoutParameterSet::Position,
			BufferLayoutParameterSet::Normal,
			BufferLayoutParameterSet::Uv0
		};
	}

	const BufferLayout Mesh::c_defaultLayout = c_defaultMeshLayoutInternal;

	Mesh::Mesh()
		: m_positions(),
		m_normals(),
		m_binormals(),
		m_tangents(),
		m_vertexColors(),
		m_bufferLayout(c_defaultMeshLayoutInternal),
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
		m_positions.Release();
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

		delete[] m_indices;

		m_indices = nullptr;
	}

	void Mesh::SetIndices(const std::vector<uint32_t>& indices)
	{
		SetIndices(&indices[0], (uint32_t)indices.size());
	}

	void Mesh::SetIndices(const uint32_t* indices, uint32_t indexCount)
	{
		m_indices = new std::uint32_t[indexCount];
		Memory::Memcpy(m_indices, indices,
			sizeof(std::uint32_t) * indexCount);

		// Creates the index buffer.
		std::shared_ptr<IndexBuffer> indexBuffer;
		IndexBuffer::Create(indexBuffer, indices, indexCount, sizeof(uint32_t));

		if (m_vertexArray != nullptr)
		{
			m_vertexArray->SetIndexBuffer(indexBuffer);
		}
	}

	void Mesh::SetMeshLayout(const BufferLayout& bufferLayout)
	{
		m_bufferLayout = bufferLayout;
		RefreshVertexArray();
	}

	void Mesh::SetVertexCount(uint32_t vertexCount)
	{
		if (m_vertexCount != vertexCount)
		{
			// Sets the vertex count.
			for (uint32_t i = 0; i < c_maxUVsCount; ++i)
			{
				m_uvs[i].SetVertexCount(vertexCount);
				m_uvs[i].SetBufferLayout({
					{ "UV", BufferLayoutSemanticType::Type_TexCoord, BufferLayoutType::Float32, 2, i }
					});
			}
			m_positions.SetVertexCount(vertexCount);
			m_positions.SetBufferLayout(BufferLayoutParameterSet::Position);

			m_normals.SetVertexCount(vertexCount);
			m_normals.SetBufferLayout(BufferLayoutParameterSet::Normal);

			m_binormals.SetVertexCount(vertexCount);
			m_binormals.SetBufferLayout(BufferLayoutParameterSet::Binormal);

			m_tangents.SetVertexCount(vertexCount);
			m_tangents.SetBufferLayout(BufferLayoutParameterSet::Tangent);

			m_vertexColors.SetVertexCount(vertexCount);
			m_vertexColors.SetBufferLayout(BufferLayoutParameterSet::Color);
		}
		m_vertexCount = vertexCount;
		RefreshVertexArray();
	}

	void Mesh::RefreshVertexArray()
	{
		m_vertexArray->ClearVertexBuffers();

		if (m_vertexCount > 0)
		{
			for (const auto& parameters : m_bufferLayout.parameters)
			{
				if (parameters == BufferLayoutParameterSet::Position)
				{
					m_vertexArray->AddVertexBuffer(m_positions.GetVertexBuffer());
				}
				else if (parameters == BufferLayoutParameterSet::Tangent)
				{
					m_vertexArray->AddVertexBuffer(m_tangents.GetVertexBuffer());
				}
				else if (parameters == BufferLayoutParameterSet::Normal)
				{
					m_vertexArray->AddVertexBuffer(m_normals.GetVertexBuffer());
				}
				else if (parameters == BufferLayoutParameterSet::Binormal)
				{
					m_vertexArray->AddVertexBuffer(m_binormals.GetVertexBuffer());
				}
				else if (parameters == BufferLayoutParameterSet::Color)
				{
					m_vertexArray->AddVertexBuffer(m_vertexColors.GetVertexBuffer());
				}
				else if (parameters == BufferLayoutParameterSet::Uv0)
				{
					m_vertexArray->AddVertexBuffer(m_uvs[0].GetVertexBuffer());
				}
				else if (parameters == BufferLayoutParameterSet::Uv1)
				{
					m_vertexArray->AddVertexBuffer(m_uvs[1].GetVertexBuffer());
				}
				else if (parameters == BufferLayoutParameterSet::Uv2)
				{
					m_vertexArray->AddVertexBuffer(m_uvs[2].GetVertexBuffer());
				}
				else if (parameters == BufferLayoutParameterSet::Uv3)
				{
					m_vertexArray->AddVertexBuffer(m_uvs[3].GetVertexBuffer());
				}
			}
		}
	}

	void Mesh::SetPositions(const std::vector<MathLib::Vector3>& vertices)
	{
		SetPositions(&vertices[0], vertices.size());
	}

	void Mesh::SetPositions(const MathLib::Vector3* vertices, size_t verticesCount)
	{
		m_positions.SetVertices(vertices, (uint32_t)verticesCount);

		// Sets the buffer layout parameters.
		const auto& buffer = m_vertexColors.GetVertexBuffer();
		buffer->SetBufferLayoutParameters(BufferLayoutParameterSet::Position);
		RefreshVertexArray();
	}

	void Mesh::SetBinormals(const std::vector<MathLib::Vector3>& bitangents)
	{
		SetBinormals(&bitangents[0], bitangents.size());
	}

	void Mesh::SetBinormals(const MathLib::Vector3* bitangents, size_t tangentsSize)
	{
		m_binormals.SetVertices(bitangents, (uint32_t)tangentsSize);

		// Sets the buffer layout parameters.
		const auto& buffer = m_vertexColors.GetVertexBuffer();
		buffer->SetBufferLayoutParameters(BufferLayoutParameterSet::Binormal);
		RefreshVertexArray();
	}

	void Mesh::SetTangents(const MathLib::Vector3* tangents, size_t tangentsSize)
	{
		m_tangents.SetVertices(tangents, (uint32_t)tangentsSize);

		// Sets the buffer layout parameters.
		const auto& buffer = m_vertexColors.GetVertexBuffer();
		buffer->SetBufferLayoutParameters(BufferLayoutParameterSet::Tangent);
		RefreshVertexArray();
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

		// Sets the buffer layout parameters.
		const auto& buffer = m_vertexColors.GetVertexBuffer();
		buffer->SetBufferLayoutParameters(BufferLayoutParameterSet::Color);
		RefreshVertexArray();
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

		// Sets the buffer layout parameters.
		const auto& buffer = uvIn.GetVertexBuffer();
		buffer->SetBufferLayoutParameters({
			 { "UV", BufferLayoutSemanticType::Type_TexCoord, BufferLayoutType::Float32, 2, index }
			});
		RefreshVertexArray();
	}

	void Mesh::SetNormals(const std::vector<MathLib::Vector3>& normals)
	{
		SetNormals(&normals[0], normals.size());
	}

	void Mesh::SetNormals(const MathLib::Vector3* normals, size_t normalsSize)
	{
		m_normals.SetVertices(normals, (uint32_t)normalsSize);

		// Sets the buffer layout parameters.
		const auto& buffer = m_normals.GetVertexBuffer();
		buffer->SetBufferLayoutParameters({
			BufferLayoutParam::Normal0
		});
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
}
