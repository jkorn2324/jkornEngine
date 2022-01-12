#pragma once


#include <d3d11.h>
#include <initializer_list>

namespace Engine
{

	enum class BufferLayoutType
	{
		BOOL,

		INT,
		INT2,
		INT3,
		INT4,

		UINT,
		UINT2,
		UINT3,
		UINT4,
		
		FLOAT,
		FLOAT2,
		FLOAT3,
		FLOAT4
	};

	const std::uint32_t MAX_SIZE_OF_NAME = 64;

	struct BufferLayoutParam
	{
		char name[MAX_SIZE_OF_NAME + 1];
		uint32_t offset;
		size_t stride;
		BufferLayoutType layoutType;

		BufferLayoutParam(const std::string& name, uint32_t offset, size_t stride, BufferLayoutType layoutType)
			: offset(offset), stride(stride), layoutType(layoutType)
		{
			uint32_t nameSize = name.size() > MAX_SIZE_OF_NAME ? 
				MAX_SIZE_OF_NAME : (uint32_t)name.size();
			std::memcpy(this->name, name.c_str(), nameSize);
			this->name[nameSize] = 0;
		}
	};

	class BufferLayout
	{
	public:
		// Deletes the buffer layout equivalent constructor.
		BufferLayout(const BufferLayout& layout) = delete;

		explicit BufferLayout(const std::initializer_list<BufferLayoutParam>& initializerList);
		~BufferLayout();

		const D3D11_INPUT_ELEMENT_DESC* GetD3D11InputElementDesc() const;
		uint32_t GetNumElements() const;

	private:
		D3D11_INPUT_ELEMENT_DESC* m_inputElementDesc;
		uint32_t m_numElements;
	};
}