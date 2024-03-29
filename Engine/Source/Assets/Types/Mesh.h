#pragma once

#include "EngineMacros.h"

#include "Vector.h"
#include "GUID.h"
#include "VertexBuffer.h"

#include "Allocator.h"
#include "Buffer.h"

#include <string>

namespace Engine
{
	class IndexBuffer;
	class VertexArray;

	template<typename T>
	class MeshBuffer
	{
	public:
		MeshBuffer()
			: m_vertexBuffer(nullptr), m_vertices(nullptr)
		{
		}

		MeshBuffer(uint32_t numVertices)
			: m_vertexBuffer(nullptr), m_vertices(nullptr)
		{
			m_vertices = new char[numVertices * sizeof(T)];
			Memory::Memset(m_vertices, 0, sizeof(T) * numVertices);
			VertexBuffer::Create(m_vertexBuffer, m_vertices, numVertices,
				sizeof(T));
		}

		MeshBuffer(const MeshBuffer<T>& buffer)
		{
			MergeBuffers(buffer);
		}

		MeshBuffer& operator=(const MeshBuffer<T>& buffer)
		{
			MergeBuffers(buffer);
			return *this;
		}

		~MeshBuffer()
		{
			m_vertexBuffer.reset();
		}

		void SetVertices(const T* data, uint32_t numVertices, bool resetVertexCount = true)
		{
			if (resetVertexCount)
			{
				if (m_vertexBuffer != nullptr
					&& m_vertexBuffer->GetNumVerts() != numVertices
					&& m_vertices != nullptr)
				{
					delete[] m_vertices;
					m_vertices = new char[sizeof(T) * numVertices];
				}
			}

			if (m_vertexBuffer != nullptr)
			{
				m_vertexBuffer->SetData(data, numVertices, sizeof(T));
			}
			else
			{
				VertexBuffer::Create(m_vertexBuffer,
					data, numVertices, sizeof(T));
			}

			if (m_vertices != nullptr)
			{
				Memory::Memcpy(m_vertices, data, sizeof(T) * (size_t)numVertices);
			}
		}

		void SetVertexCount(uint32_t vertexCount)
		{
			if (m_vertexBuffer != nullptr
				&& m_vertexBuffer->GetNumVerts() == vertexCount) return;

			Release();

			size_t size = vertexCount * sizeof(T);
			m_vertices = new char[size];
			Memory::Memset(m_vertices, 0, size);
			VertexBuffer::Create(m_vertexBuffer,
				m_vertices, vertexCount, sizeof(T));
		}

		void SetBufferLayout(const BufferLayoutParameterSet& set)
		{
			if (m_vertexBuffer)
			{
				m_vertexBuffer->SetBufferLayoutParameters(set);
			}
		}

		const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { return m_vertexBuffer; }

		const T* GetRawBuffer() const { return reinterpret_cast<T*>(m_vertices); }

		void Refresh(size_t vertexCount)
		{
			Release();
		}

		const T& operator[](uint32_t index) const
		{
			return reinterpret_cast<T>(m_vertices[index * sizeof(T)]);
		}

		T& operator[](uint32_t index)
		{
			return reinterpret_cast<T>(m_vertices[index * sizeof(T)]);
		}

		void Release()
		{
			if (m_vertices != nullptr)
			{
				delete[] m_vertices;
				m_vertices = nullptr;
			}
		}

	private:
		void MergeBuffers(const MeshBuffer<T>& buffer)
		{
			Release();

			const auto& otherVBO = buffer.GetVertexBuffer();
			if (otherVBO != nullptr)
			{
				if (m_vertexBuffer != nullptr)
				{
					m_vertexBuffer->SetData(buffer.GetRawBuffer(),
						otherVBO->GetNumVerts(), otherVBO->GetStride());
				}
				else
				{
					VertexBuffer::Create(m_vertexBuffer,
						buffer.GetRawBuffer(), otherVBO->GetNumVerts(), otherVBO->GetStride());
				}
			}
			if (m_vertexBuffer != nullptr)
			{
				size_t size = m_vertexBuffer->GetNumVerts() * m_vertexBuffer->GetStride();
				m_vertices = new char[size];
				Memory::Memcpy(m_vertices, buffer.m_vertices, size);
			}
		}
	
	private:
		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		char* m_vertices;
	};

	class Mesh
	{
	public:
		static const BufferLayout c_defaultLayout;
		static const uint32_t c_maxUVsCount = 8;

	public:
		explicit Mesh();
		~Mesh();

		void RecalculateNormals();
	
		void SetIndices(const std::vector<uint32_t>& indices);
		void SetIndices(const uint32_t* indices, uint32_t indexCount);
		uint32_t* GetIndices() const;
		uint32_t GetIndexCount() const;

		const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_vertexArray; }

		void SetSkinned(bool skinned) { m_skinned = skinned; }
		bool IsSkinned() const { return m_skinned; }

		void SetMeshLayout(const BufferLayout& bufferLayout);

		void SetVertexCount(uint32_t vertexCount);
		uint32_t GetVertexCount() const { return m_vertexCount; }
		
		void SetPositions(const MathLib::Vector3* vertices, size_t verticesSize);
		void SetPositions(const std::vector<MathLib::Vector3>& vertices);
		const MathLib::Vector3* GetPositions() const { return m_positions.GetRawBuffer(); }
		
		void SetNormals(const MathLib::Vector3* normals, size_t normalsSize);
		void SetNormals(const std::vector<MathLib::Vector3>& normals);
		const MathLib::Vector3* GetNormals() const { return m_normals.GetRawBuffer(); }

		void SetBinormals(const MathLib::Vector3* binormals, size_t normalsSize);
		void SetBinormals(const std::vector<MathLib::Vector3>& binormals);
		const MathLib::Vector3* GetBinormals() const { return m_binormals.GetRawBuffer(); }

		void SetTangents(const MathLib::Vector3* tangents, size_t tangentsSize);
		void SetTangents(const std::vector<MathLib::Vector3>& tangents);
		const MathLib::Vector3* GetTangents() const { return m_tangents.GetRawBuffer(); }

		void SetColors(const std::vector<MathLib::Vector4>& colors);
		void SetColors(const MathLib::Vector4* vertexColors, size_t size);
		const MathLib::Vector4* GetVertexColors() const { return m_vertexColors.GetRawBuffer(); }

		void SetUVs(uint32_t index, const std::vector<MathLib::Vector2>& uvs);
		void SetUVs(uint32_t index, const MathLib::Vector2* uvs, size_t size);
		const MathLib::Vector2* GetUVs(uint32_t index) const { return m_uvs[index].GetRawBuffer(); }

	private:
		void RefreshVertexArray();

	private:	
		// Graphics Arrays
		std::shared_ptr<VertexArray> m_vertexArray;
		
		// Indices
		uint32_t* m_indices;
		
		// Vertices Data
		MeshBuffer<MathLib::Vector3> m_positions;
		MeshBuffer<MathLib::Vector3> m_normals;
		MeshBuffer<MathLib::Vector3> m_binormals;
		MeshBuffer<MathLib::Vector3> m_tangents;
		MeshBuffer<MathLib::Vector4> m_vertexColors;
		MeshBuffer<MathLib::Vector2> m_uvs[c_maxUVsCount];

		BufferLayout m_bufferLayout;
		uint32_t m_vertexCount;
		uint32_t m_indexCount;

		bool m_skinned;
	};
}
