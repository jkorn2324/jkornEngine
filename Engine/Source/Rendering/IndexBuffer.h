#pragma once

#include <d3d11.h>

namespace Engine
{

	class IndexBuffer
	{
	public:
		explicit IndexBuffer(const void* buffer, std::uint32_t numIndices, std::size_t indexStride);
		virtual ~IndexBuffer() { }

		virtual bool IsValid() const=0;

		std::uint32_t GetNumIndices() const;
		std::uint32_t GetStride() const;

	protected:
		virtual void Bind() const =0;

	private:
		std::uint32_t m_numIndices;
		std::size_t m_indexStride;

	public:
		static IndexBuffer* Create(const void* buffer,
			std::uint32_t numIndices, std::uint32_t stride);

		friend class GraphicsRenderer;
	};
}