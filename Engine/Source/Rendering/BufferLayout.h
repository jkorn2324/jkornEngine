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

	static size_t GetStrideFromBufferLayoutType(const BufferLayoutType& layoutType)
	{
		switch (layoutType)
		{
			case BufferLayoutType::BOOL: return sizeof(bool);
			case BufferLayoutType::INT: return sizeof(int32_t);
			case BufferLayoutType::INT2: return sizeof(int32_t) * 2;
			case BufferLayoutType::INT3: return sizeof(int32_t) * 3;
			case BufferLayoutType::INT4: return sizeof(int32_t) * 4;
			case BufferLayoutType::UINT: return sizeof(uint32_t);
			case BufferLayoutType::UINT2: return sizeof(uint32_t) * 2;
			case BufferLayoutType::UINT3: return sizeof(uint32_t) * 3;
			case BufferLayoutType::UINT4: return sizeof(uint32_t) * 4;
			case BufferLayoutType::FLOAT: return sizeof(float);
			case BufferLayoutType::FLOAT2: return sizeof(float) * 2;
			case BufferLayoutType::FLOAT3: return sizeof(float) * 3;
			case BufferLayoutType::FLOAT4: return sizeof(float) * 4;
		}
		return 0;
	}

	const std::uint32_t MAX_SIZE_OF_NAME = 64;

	struct BufferLayoutParam
	{
		char name[MAX_SIZE_OF_NAME + 1];
		uint32_t offset;
		size_t stride;
		BufferLayoutType layoutType;

		BufferLayoutParam(const std::string& name, uint32_t offset, BufferLayoutType layoutType)
			: offset(offset), layoutType(layoutType)
		{
			stride = GetStrideFromBufferLayoutType(layoutType);
			uint32_t nameSize = name.size() > MAX_SIZE_OF_NAME ?
				MAX_SIZE_OF_NAME : (uint32_t)name.size();
			std::memcpy(this->name, name.c_str(), nameSize);
			this->name[nameSize] = 0;
		}

		BufferLayoutParam(const std::string& name, uint32_t offset, size_t stride, BufferLayoutType layoutType)
			: offset(offset), stride(stride), layoutType(layoutType)
		{
			uint32_t nameSize = name.size() > MAX_SIZE_OF_NAME ? 
				MAX_SIZE_OF_NAME : (uint32_t)name.size();
			std::memcpy(this->name, name.c_str(), nameSize);
			this->name[nameSize] = 0;
		}

		BufferLayoutParam(const BufferLayoutParam& param) 
			: offset(param.offset), stride(param.stride), layoutType(param.layoutType)
		{
			uint32_t nameSize = sizeof(param.name) > 0 ? sizeof(param.name) : 1;
			std::memcpy(name, param.name, nameSize);
			name[nameSize - 1] = 0;
		}
	};

	struct BufferLayout
	{
		std::vector<BufferLayoutParam> parameters;

		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferLayoutParam>& initializerList)
			: parameters(initializerList) { }
		BufferLayout(const BufferLayout& layout)
			: parameters(layout.parameters) { }

		BufferLayout& operator=(const BufferLayout& layout)
		{
			parameters = layout.parameters;
			return *this;
		}

		uint32_t GetNumElements() const { return (uint32_t)parameters.size(); }
	};
}