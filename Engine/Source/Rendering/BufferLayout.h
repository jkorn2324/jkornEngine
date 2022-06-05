#pragma once


#include <initializer_list>
#include <memory>

namespace Engine
{

	enum class BufferLayoutSemanticType
	{
		Type_Position,
		Type_Normal,
		Type_Tangent,
		Type_Binormal,
		Type_TexCoord,
		Type_UV = Type_TexCoord,
		Type_Color,
		Type_Custom
	};

	enum class BufferLayoutType
	{
		Int8,
		Bool = Int8,

		Int16,
		Int32,

		Uint8,
		Uint16,
		Uint32,

		Float16,
		Float32,

		SNorm8,
		SNorm16,

		UNorm8,
		UNorm16
	};

	static size_t GetStrideFromBufferLayoutType(const BufferLayoutType& layoutType)
	{
		switch (layoutType)
		{
		case BufferLayoutType::Int8: return sizeof(int8_t);
		case BufferLayoutType::Int16: return sizeof(int16_t);
		case BufferLayoutType::Int32: return sizeof(int32_t);
		case BufferLayoutType::Uint8: return sizeof(uint8_t);
		case BufferLayoutType::Uint16: return sizeof(uint16_t);
		case BufferLayoutType::Uint32: return sizeof(uint32_t);
		case BufferLayoutType::Float16: return sizeof(uint16_t);
		case BufferLayoutType::Float32: return sizeof(float_t);
		case BufferLayoutType::SNorm8: return sizeof(int8_t);
		case BufferLayoutType::SNorm16: return sizeof(int16_t);
		case BufferLayoutType::UNorm8: return sizeof(uint8_t);
		case BufferLayoutType::UNorm16: return sizeof(uint16_t);
		}
		return 0;
	}

	const std::uint32_t MAX_SIZE_OF_NAME = 64;

	struct BufferLayoutParam
	{
		std::string name;
		BufferLayoutSemanticType semanticType;
		BufferLayoutType layoutType;
		uint32_t numValues = 1;
		uint32_t semanticIndex = 0;

		size_t GetStride() const { return GetStrideFromBufferLayoutType(layoutType) * (size_t)numValues; }
	};

	struct BufferLayoutParameterSet
	{
		std::vector<BufferLayoutParam> parameters;

		BufferLayoutParameterSet() : parameters() { }

		BufferLayoutParameterSet(const BufferLayoutParam& param)
			: parameters({ param })
		{
		}

		BufferLayoutParameterSet(const std::initializer_list<BufferLayoutParam>& params)
			: parameters(params)
		{

		}
	};

	// TODO: Refactor the buffer layout.
	struct BufferLayout
	{
		std::vector<BufferLayoutParameterSet> parameters;

		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferLayoutParameterSet>& initializerList)
			: parameters(initializerList) { }
		BufferLayout(const BufferLayout& layout)
			: parameters(layout.parameters) { }

		BufferLayout& operator=(const BufferLayout& layout)
		{
			parameters = layout.parameters;
			return *this;
		}

		uint32_t GetTotalParamSize() const
		{
			uint32_t i = 0;
			for (const BufferLayoutParameterSet& set : parameters)
			{
				i += (uint32_t)set.parameters.size();
			}
			return i;
		}

		uint32_t GetNumElements() const { return (uint32_t)parameters.size(); }
	};
}