#pragma once

#include "VertexBuffer.h"

#include <d3d11.h>

namespace Engine
{

	class DirectX11VertexBuffer : public VertexBuffer
	{
	public:
		explicit DirectX11VertexBuffer(const void* buffer,
			std::uint32_t numVertices, std::uint32_t stride);
		~DirectX11VertexBuffer();

		bool IsValid() const override;

	protected:
		void Bind() const override;

	private:
		ID3D11Buffer* m_vertexBuffer;

		friend class DirectX11RenderingAPI;
	};
}