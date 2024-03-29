#pragma once

namespace Engine
{

	class IndexBuffer
	{
	public:
		explicit IndexBuffer(const void* buffer, uint32_t numIndices, uint32_t indexStride);
		virtual ~IndexBuffer() { }

		virtual bool IsValid() const=0;

		uint32_t GetNumIndices() const;
		uint32_t GetStride() const;

		virtual void SetData(const void* buffer,
			uint32_t numIndices, uint32_t stride) =0;

	protected:
		uint32_t m_numIndices;
		uint32_t m_indexStride;

	public:
		static bool Create(std::shared_ptr<IndexBuffer>& buf,
			const void* buffer, uint32_t indices, uint32_t stride);
		static bool Create(std::unique_ptr<IndexBuffer>& buf,
			const void* buffer, uint32_t indices, uint32_t stride);
		static bool Create(IndexBuffer** buf,
			const void* buffer, uint32_t indices, uint32_t stride);
	};
}
