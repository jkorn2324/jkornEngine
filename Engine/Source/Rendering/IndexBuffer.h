#pragma once

#include <d3d11.h>

namespace Engine
{

	class IndexBuffer
	{
	public:
		explicit IndexBuffer(const void* buffer, std::uint32_t numIndices, std::size_t indexStride);
		~IndexBuffer();

		bool IsValid() const;
		std::uint32_t GetNumIndices() const;

	private:
		void Bind() const;

	private:
		ID3D11Buffer* m_indexBuffer;
		std::uint32_t m_numIndices;
		std::size_t m_indexStride;

		// DirectX Specific Variables.
		DXGI_FORMAT m_format;

		friend class GraphicsRenderer;
	};
}