#pragma once

#include "IndexBuffer.h"

#include <d3d11.h>

namespace Engine
{
	class DirectX11IndexBuffer : public IndexBuffer
	{
	public:
		explicit DirectX11IndexBuffer(const void* buffer,
			uint32_t numIndices, uint32_t stride);
		~DirectX11IndexBuffer();

		bool IsValid() const override;

		void SetData(const void* buffer,
			uint32_t numIndices, uint32_t stride) override;

		void Bind() const override;

	private:
		ID3D11Buffer* m_indexBuffer;
		DXGI_FORMAT m_format;

		friend class DirectX11RenderingAPI;
	};
}


