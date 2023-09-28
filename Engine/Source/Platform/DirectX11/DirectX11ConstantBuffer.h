#pragma once

#include "ConstantBuffer.h"

struct ID3D11Buffer;

namespace Engine
{

	class DirectX11ConstantBuffer : public ConstantBuffer
	{
	public:
		explicit DirectX11ConstantBuffer(const void* buffer, std::size_t stride);
		~DirectX11ConstantBuffer();

		void SetData(void* buffer, std::size_t stride) override;
		void Bind(const std::uint32_t& slot, int flags) const override;

	private:
		ID3D11Buffer* m_constantBuffer;

		friend class DirectX11RenderingAPI;
	};
}