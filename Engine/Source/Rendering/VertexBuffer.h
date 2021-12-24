#pragma once

#include <d3d11.h>

namespace Engine
{

	class VertexBuffer
	{

	public:
		explicit VertexBuffer(const void* bufferData, std::uint32_t numVertices, std::size_t stride);
		~VertexBuffer();
		
		bool IsValid() const;
		std::uint32_t GetNumVerts() const;

	private:
		void Bind() const;

	private:
		ID3D11Buffer* m_vertexBuffer;
		std::uint32_t m_stride;
		std::uint32_t m_numVerts;

		friend class GraphicsRenderer;
	};
}