#pragma once

#include "BufferLayout.h"

struct D3D11_INPUT_ELEMENT_DESC;

namespace Engine
{

	class DirectX11BufferLayoutParser
	{
	public:
		explicit DirectX11BufferLayoutParser(const BufferLayout& bufferlayout);
		~DirectX11BufferLayoutParser();
		
		uint32_t GetNumElements() const { return m_numElements; }
		const D3D11_INPUT_ELEMENT_DESC* GetD3D11InputElementDesc() const;

	private:
		D3D11_INPUT_ELEMENT_DESC* m_inputElementDesc;
		uint32_t m_numElements;
	};
}