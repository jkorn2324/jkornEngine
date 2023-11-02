#pragma once

#include <memory>
#include <vector>
#include <initializer_list>

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


		friend bool operator==(const BufferLayoutParam& a, const BufferLayoutParam& b)
		{
			return a.name == b.name
				&& a.semanticType == b.semanticType
				&& a.layoutType == b.layoutType
				&& a.numValues == b.numValues
				&& a.semanticIndex == b.semanticIndex;
		}
		
		friend bool operator!=(const BufferLayoutParam& a, const BufferLayoutParam& b)
		{
			return a.name != b.name
				|| a.semanticType != b.semanticType
				|| a.layoutType != b.layoutType
				|| a.numValues != b.numValues
				|| a.semanticIndex != b.semanticIndex;
		}

		static const BufferLayoutParam Position0;
		static const BufferLayoutParam Normal0;
		static const BufferLayoutParam Binormal0;
		static const BufferLayoutParam Tangent0;
		static const BufferLayoutParam Color4_0;
		static const BufferLayoutParam Uv0;
		static const BufferLayoutParam Uv1;
		static const BufferLayoutParam Uv2;
		static const BufferLayoutParam Uv3;
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

		friend bool operator==(const BufferLayoutParameterSet& a, const BufferLayoutParameterSet& b)
		{
			if (a.parameters.size() != b.parameters.size()) return false;

			for (size_t i = 0; i < a.parameters.size(); ++i)
			{
				if (a.parameters[i] != b.parameters[i]) return false;
			}
			return true;
		}

		friend bool operator!=(const BufferLayoutParameterSet& a, const BufferLayoutParameterSet& b)
		{
			if (a.parameters.size() != b.parameters.size()) return true;

			for (size_t i = 0; i < a.parameters.size(); ++i)
			{
				if (a.parameters[i] != b.parameters[i]) return true;
			}
			return false;
		}

		static const BufferLayoutParameterSet Position;
		static const BufferLayoutParameterSet Normal;
		static const BufferLayoutParameterSet Binormal;
		static const BufferLayoutParameterSet Tangent;
		static const BufferLayoutParameterSet Uv0;
		static const BufferLayoutParameterSet Uv1;
		static const BufferLayoutParameterSet Uv2;
		static const BufferLayoutParameterSet Uv3;
		static const BufferLayoutParameterSet Color;
	};


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

		friend bool operator==(const BufferLayout& a, const BufferLayout& b)
		{
			if (a.parameters.size() != b.parameters.size()) return false;

			for (size_t i = 0; i < a.parameters.size(); ++i)
			{
				if (a.parameters[i] != b.parameters[i]) return false;
			}
			return true;
		}

		friend bool operator!=(const BufferLayout& a, const BufferLayout& b)
		{
			if (a.parameters.size() != b.parameters.size()) return true;

			for (size_t i = 0; i < a.parameters.size(); ++i)
			{
				if (a.parameters[i] != b.parameters[i]) return true;
			}
			return false;
		}
	};
}