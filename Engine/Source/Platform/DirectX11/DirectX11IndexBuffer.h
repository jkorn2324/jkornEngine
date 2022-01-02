#pragma once

#include "IndexBuffer.h"

namespace Engine
{
	class DirectX11IndexBuffer : public IndexBuffer
	{
	public:
		explicit DirectX11IndexBuffer(const void* buffer,
			std::uint32_t numIndices, std::uint32_t stride);
		~DirectX11IndexBuffer();

		bool IsValid() const override;

	protected:
		void Bind() const override;

	private:
		ID3D11Buffer* m_indexBuffer;
		DXGI_FORMAT m_format;

		friend class DirectX11RenderingAPI;
	};
}


