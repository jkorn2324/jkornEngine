#pragma once

#include <string>
#include <vector>
#include <initializer_list>
#include <unordered_map>

#include "Memory.h"
#include "Buffer.h"

namespace Engine
{
	enum MaterialConstantLayoutType
	{
		LayoutType_Unknown,

		LayoutType_Int16,
		LayoutType_Int32,
		LayoutType_Int64,
		
		LayoutType_Uint16,
		LayoutType_Uint32,
		LayoutType_Uint64,

		LayoutType_Float,
		LayoutType_Bool,

		LayoutType_Vector2,
		LayoutType_Vector3,
		LayoutType_Vector4,
		LayoutType_Quaternion
	};

	static size_t GetStrideFromMaterialConstantLayoutType(const MaterialConstantLayoutType& layoutType)
	{
		switch (layoutType)
		{
			case LayoutType_Int16: return sizeof(int16_t);
			case LayoutType_Int32: return sizeof(int32_t);
			case LayoutType_Int64: return sizeof(int64_t);
			case LayoutType_Uint16: return sizeof(uint16_t);
			case LayoutType_Uint32: return sizeof(uint32_t);
			case LayoutType_Uint64: return sizeof(uint64_t);
			case LayoutType_Float: return sizeof(float);
			case LayoutType_Bool: return sizeof(bool);
			case LayoutType_Vector2: return sizeof(float) * 2;
			case LayoutType_Vector3: return sizeof(float) * 3;
			case LayoutType_Vector4: return sizeof(float) * 4;
			case LayoutType_Quaternion: return sizeof(float) * 4;
		}
		return 0;
	}

	// How Material Constants will work is the following:
	// Create a material constants layout builder, fill it with attributes
	// Create a material constants layout from the builder
	// Fill a Material Layout Buffer utilizing the material constants layout.

	struct MaterialConstantLayoutAttribute
	{
		std::string name;
		size_t layoutStride;
		MaterialConstantLayoutType layoutType;
		bool addPadding;

		MaterialConstantLayoutAttribute(const std::string& name, const MaterialConstantLayoutType& layoutType, bool pad = false)
			: name(name), layoutStride(0), layoutType(layoutType), addPadding(pad)
		{
			layoutStride = GetStrideFromMaterialConstantLayoutType(layoutType);
		}
		MaterialConstantLayoutAttribute(const MaterialConstantLayoutType& layoutType)
			: name(""), addPadding(true), layoutStride(0), layoutType(layoutType) 
		{
			layoutStride = GetStrideFromMaterialConstantLayoutType(layoutType);
		}
	};

	struct MaterialConstantsLayout
	{
		std::vector<MaterialConstantLayoutAttribute> layoutAttributes;

		MaterialConstantsLayout() : layoutAttributes() { }
		MaterialConstantsLayout(const std::initializer_list<MaterialConstantLayoutAttribute>& layout)
			: layoutAttributes(layout) { }

		size_t GetLayoutSize() const
		{
			size_t layoutSize = 0;
			const size_t len = layoutAttributes.size();
			for (size_t index = 0; index < len; ++index)
			{
				const MaterialConstantLayoutAttribute& attrib = layoutAttributes[index];
				layoutSize += attrib.layoutStride;
			}
			return layoutSize;
		}
	};


	/**
	 * The built material constants. (with layout). 
	 */
	class MaterialConstants
	{
	public:
		/**
		 * The material constant value.
		 */
		struct MaterialConstantValue
		{
			uint32_t offset;
			size_t stride;
		};

	private:
		/**
		 * Structure for the material constant buffer.
		 */
		struct MaterialConstantBufferData
		{
			uint32_t offset;
			size_t stride;
			MaterialConstantLayoutType layoutType;
			bool isPadding;
		};

	public:
		MaterialConstants(const MaterialConstantsLayout& materialConstantsLayout);

		/**
		 * Changes the layout for the material constants. 
		 */
		void SetLayout(const MaterialConstantsLayout& materialConstantsLayout);

		/**
		 * Get Constant Value Properties (layout + stride). 
		 */
		bool GetConstantValue(const std::string& name, MaterialConstantValue& constantValue) const
		{
			const auto& found = m_materialConstants.find(name);
			if (found != m_materialConstants.end())
			{
				MaterialConstantBufferData bufData = found->second;
				constantValue.offset = bufData.offset;
				constantValue.stride = bufData.stride;
				return true;
			}
			return false;
		}

		/**
		 * Gets the layout size for the material constants. 
		 */
		size_t GetLayoutBufferSize() const { return m_layoutSize; }

	private:
		std::unordered_map<std::string, MaterialConstantBufferData> m_materialConstants;
		size_t m_layoutSize;
	};

	/**
	 * Represents a class that can view a constant inside of a buffer.
	 */
	template<typename TBuffer>
	class MaterialConstantsView
	{
		static_assert(IsValidBuffer<TBuffer>::Value, "The buffer must be a valid buffer.");

	public:
		MaterialConstantsView(const TBuffer& buf, MaterialConstants& constants)
			: m_buffer(buf), m_constants(constants) { }

		template<typename T>
		T* GetValue(const std::string& name) const
		{
			MaterialConstants::MaterialConstantValue val;
			if (m_constants.GetConstantValue(name, val))
			{
				return m_buffer.Get<T>(val.offset);
			}
			return nullptr;
		}

	private:
		const TBuffer& m_buffer;
		MaterialConstants& m_constants;
	};

	/**
	 * Represents a class that can modify a buffer based on the material constants. 
	 */
	template<typename TBuffer>
	class MaterialConstantsModifier
	{
		static_assert(IsValidBuffer<TBuffer>::Value, "The buffer must be a valid buffer.");

	public:
		MaterialConstantsModifier(TBuffer& buf, MaterialConstants& constants)
			: m_buffer(buf), m_constants(constants)
		{

		}

		template<typename T>
		void SetValue(const std::string& name, const T& value)
		{
			MaterialConstants::MaterialConstantValue val;
			if (!m_constants.GetConstantValue(name, val))
			{
				return;
			}
			size_t size = sizeof(T);
			if (size != val.stride)
			{
				return;
			}
			m_buffer.Set<T>(value, val.offset);
		}

	private:
		TBuffer& m_buffer;
		MaterialConstants& m_constants;
	};
}

