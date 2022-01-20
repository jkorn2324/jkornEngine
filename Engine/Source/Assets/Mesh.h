#pragma once

#include <string>

#include "Source\Vector.h"

namespace Engine
{

	class VertexBuffer;
	class IndexBuffer;

	class Mesh
	{
	public:
		explicit Mesh();
		~Mesh();

		void SetIndices(const uint32_t* indices, uint32_t indexCount);
		void SetVertices(const void* vertices, size_t stride, uint32_t indexCount);

		template<typename T>
		T& operator[](int index) { return (T)m_vertices[index]; }

		template<typename T>
		T* GetVertices() const { return reinterpret_cast<T*>(m_vertices); }
		std::uint32_t GetNumVertices() const;

		std::uint32_t* GetIndices() const;
		std::uint32_t GetNumIndices() const;

		VertexBuffer* GetVertexBuffer() const { return m_vertexBuffer; }
		IndexBuffer* GetIndexBuffer() const { return m_indexBuffer; }

		void SetSkinned(bool skinned) { m_skinned = skinned; }
		bool IsSkinned() const { return m_skinned; }

	private:	
		char* m_vertices;
		std::uint32_t* m_indices;

		VertexBuffer* m_vertexBuffer;
		IndexBuffer* m_indexBuffer;

		std::uint32_t m_vertexCount;
		std::uint32_t m_indexCount;

		bool m_skinned;
	};
}