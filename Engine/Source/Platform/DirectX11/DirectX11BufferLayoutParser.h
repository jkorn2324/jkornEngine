#pragma once

#include "BufferLayout.h"

#include <d3d11.h>

namespace Engine
{

	class DirectX11BufferLayoutParser
	{
	public:
		static const uint32_t MaxSlots = D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT;

		explicit DirectX11BufferLayoutParser(const BufferLayout& bufferlayout);
		~DirectX11BufferLayoutParser();
		
		uint32_t GetNumElements() const { return m_numElements; }
		const D3D11_INPUT_ELEMENT_DESC* GetD3D11InputElementDesc() const;

	private:
		D3D11_INPUT_ELEMENT_DESC m_inputElementDesc[MaxSlots];
		uint32_t m_numElements;
	};
}