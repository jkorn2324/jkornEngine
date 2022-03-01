#pragma once


#include <initializer_list>
#include <memory>

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

	struct BufferLayout
	{
		std::vector<BufferLayoutParam> parameters;

		BufferLayout(const std::initializer_list<BufferLayoutParam>& initializerList)
			: parameters(initializerList) { }

		uint32_t GetNumElements() const { return (uint32_t)parameters.size(); }
	};
}