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

	protected:
		virtual void Bind() const =0;

	private:
		std::uint32_t m_stride;
		std::uint32_t m_numVerts;

	public:
		static VertexBuffer* Create(const void* bufferData,
			std::uint32_t numVertices, std::size_t stride);

		friend class GraphicsRenderer;
	};
}