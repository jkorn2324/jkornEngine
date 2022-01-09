#pragma once

namespace Engine
{

	class VertexBuffer
	{
	public:
		explicit VertexBuffer(const void* bufferData, std::uint32_t numVertices, std::size_t stride);
		virtual ~VertexBuffer() { }
		
		std::uint32_t GetNumVerts() const;
		std::uint32_t GetStride() const;

		virtual bool IsValid() const =0;

		virtual void SetData(const void* bufferData, std::uint32_t numVertices,
			std::uint32_t stride) =0;
		virtual void Bind() const =0;

	protected:
		std::uint32_t m_stride;
		std::uint32_t m_numVerts;

	public:
		static VertexBuffer* Create(const void* bufferData,
			std::uint32_t numVertices, std::size_t stride);
	};
}