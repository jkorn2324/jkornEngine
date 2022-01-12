#pragma once

namespace Engine
{

	class VertexBuffer
	{
	public:
		explicit VertexBuffer(const void* bufferData, uint32_t numVertices, uint32_t stride);
		virtual ~VertexBuffer() { }
		
		uint32_t GetNumVerts() const;
		uint32_t GetStride() const;

		virtual bool IsValid() const =0;

		virtual void SetData(const void* bufferData, uint32_t numVertices,
			uint32_t stride) =0;
		virtual void Bind() const =0;

	protected:
		uint32_t m_stride;
		uint32_t m_numVerts;

	public:
		static VertexBuffer* Create(const void* bufferData,
			uint32_t numVertices, uint32_t stride);
	};
}