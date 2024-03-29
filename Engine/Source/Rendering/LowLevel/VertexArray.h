#pragma once

#include <cstdint>
#include <vector>

#include <memory>

#include "BufferLayout.h"

namespace Engine
{

	class VertexBuffer;
	class IndexBuffer;

	class VertexArray
	{
	public:
		explicit VertexArray() :  m_bufferLayout() { }
		virtual ~VertexArray() { }

		virtual bool IsValid() const { return m_indexBuffer != nullptr && GetNumVertexBuffers() > 0; }
		
		virtual void SetVertexBuffers(const std::vector<std::shared_ptr<VertexBuffer>>& buffers) =0;
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) =0;
		virtual bool GetVertexBuffer(std::shared_ptr<VertexBuffer>& buf, uint32_t index) const =0;

		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
		{
			// TODO: Index buffer isn't getting reset.
			m_indexBuffer.reset();
			m_indexBuffer = indexBuffer;
		}

		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_indexBuffer; }

		virtual size_t GetNumVertexBuffers() const = 0;
		virtual void Bind() const = 0;

		virtual void ClearVertexBuffers()=0;
		
		const BufferLayout& GetLayout() const { return m_bufferLayout; }

	protected:
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		BufferLayout m_bufferLayout;

	public:
		static bool Create(VertexArray** outVertexArray);
		static bool Create(std::shared_ptr<VertexArray>& outVertexArray);
		static bool Create(std::unique_ptr<VertexArray>& outVertexArray);

		friend class GraphicsRenderer;
	};
}