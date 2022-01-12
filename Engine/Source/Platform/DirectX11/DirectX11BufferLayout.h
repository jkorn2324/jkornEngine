#pragma once

#include "BufferLayout.h"

struct D3D11_INPUT_ELEMENT_DESC;

namespace Engine
{

	class DirectX11BufferLayout : public BufferLayout
	{
	public:
		explicit DirectX11BufferLayout(const std::initializer_list<BufferLayoutParam>& params);
		~DirectX11BufferLayout();
		
		uint32_t GetNumElements() const override;
		const D3D11_INPUT_ELEMENT_DESC* GetD3D11InputElementDesc() const;

	private:
		D3D11_INPUT_ELEMENT_DESC* m_inputElementDesc;
		uint32_t m_numElements;
	};
}