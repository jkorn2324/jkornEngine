#pragma once

#include <string>

#include "Source\Vector.h"
#include "GUID.h"
#include "EngineMacros.h"

#include "VertexBuffer.h"

#include <string>

namespace Engine
{
	class IndexBuffer;
	class VertexArray;

	template<typename T>
	class AssetSerializer;
	template<typename T>
	class AssetCache;

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
			memset(m_vertices, 0, sizeof(T) * numVertices);
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
				std::memcpy(m_vertices, data, sizeof(T) * (size_t)numVertices);
			}
		}

		void SetVertexCount(uint32_t vertexCount)
		{
			if (m_vertexBuffer != nullptr
				&& m_vertexBuffer->GetNumVerts() == vertexCount) return;

			Release();

			size_t size = vertexCount * sizeof(T);
			m_vertices = new char[size];
			memset(m_vertices, 0, size);
			VertexBuffer::Create(m_vertexBuffer,
				m_vertices, vertexCount, sizeof(T));
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
				std::memcpy(m_vertices, buffer.m_vertices, size);
			}
		}
	
	private:
		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		char* m_vertices;
	};

	class Mesh
	{
	public:
		explicit Mesh();
		~Mesh();

		uint32_t GetNumVertices() const;

		void RecalculateNormals();
		
		void SetIndices(const uint32_t* indices, uint32_t indexCount);
		uint32_t* GetIndices() const;
		uint32_t GetIndexCount() const;

		const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_vertexArray; }

		void SetSkinned(bool skinned) { m_skinned = skinned; }
		bool IsSkinned() const { return m_skinned; }

		void SetVertexCount(uint32_t vertexCount);
		uint32_t GetVertexCount() const { return m_vertexCount; }
		
		void SetVertices(const MathLib::Vector3* vertices, size_t verticesSize);
		void SetVertices(const std::initializer_list<MathLib::Vector3>& vertices);
		const MathLib::Vector3* GetVertices() const { return m_vertices.GetRawBuffer(); }
		
		void SetNormals(const MathLib::Vector3* normals, size_t normalsSize);
		void SetNormals(const std::initializer_list<MathLib::Vector3>& vertices);
		const MathLib::Vector3* GetNormals() const { return m_normals.GetRawBuffer(); }

		void SetBitangents(const MathLib::Vector3* bitangents, size_t tangentsSize);
		void SetBitangents(const std::initializer_list<MathLib::Vector3>& bitangents);
		const MathLib::Vector3* GetBitangents() const { return m_bitangents.GetRawBuffer(); }

		void SetColors(const std::initializer_list<MathLib::Vector4>& colors);
		void SetColors(const MathLib::Vector4* vertexColors, size_t size);
		const MathLib::Vector4* GetVertexColors() const { return m_vertexColors.GetRawBuffer(); }

		void SetUVs(uint32_t index, const std::initializer_list<MathLib::Vector2>& uvs);
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
		MeshBuffer<MathLib::Vector3> m_vertices;
		MeshBuffer<MathLib::Vector3> m_normals;
		MeshBuffer<MathLib::Vector3> m_bitangents;
		MeshBuffer<MathLib::Vector4> m_vertexColors;

		MeshBuffer<MathLib::Vector2>* m_uvs;
		// TODO: Link Buffer Layout to Shader

		uint32_t m_vertexCount;
		uint32_t m_indexCount;

		bool m_skinned;

		SERIALIZABLE_ASSET(Mesh);
	};
}