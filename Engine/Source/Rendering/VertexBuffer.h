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
		static bool Create(std::shared_ptr<VertexBuffer>& ptr,
			const void* bufferData, uint32_t numVertices, uint32_t stride);
		static bool Create(VertexBuffer** buf,
			const void* bufferData, uint32_t numVertices, uint32_t stride);
	};
}