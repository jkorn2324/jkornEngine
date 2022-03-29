#pragma once

#include <string>
#include <vector>
#include <initializer_list>
#include <unordered_map>

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

	struct MaterialConstantLayoutAttribute
	{
		std::string name;
		size_t layoutStride;
		MaterialConstantLayoutType layoutType;
		bool pad;

		MaterialConstantLayoutAttribute(const std::string& name, const MaterialConstantLayoutType& layoutType, bool pad = false)
			: name(name), layoutStride(0), layoutType(layoutType), pad(pad)
		{
			layoutStride = GetStrideFromMaterialConstantLayoutType(layoutType);
		}
		MaterialConstantLayoutAttribute(const MaterialConstantLayoutType& layoutType)
			: name(""), pad(true), layoutStride(0), layoutType(layoutType) 
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
	};

	struct MaterialConstantBufferData
	{
		uint32_t offset;
		size_t stride;
		MaterialConstantLayoutType layoutType;
		bool pad;
	};

	class MaterialConstants
	{
	public:
		MaterialConstants(const MaterialConstantsLayout& materialConstantsLayout);
		MaterialConstants(const MaterialConstantsLayout& materialConstantsLayout, size_t totalBufferSize);
		~MaterialConstants();

		MaterialConstants(const MaterialConstants& constants);
		MaterialConstants& operator=(const MaterialConstants& constants);

		void SetLayout(const MaterialConstantsLayout& materialConstantsLayout);
		void SetLayout(const MaterialConstantsLayout& materialConstantsLayout, size_t totalBufferSize);

		template<typename T>
		void SetMaterialConstant(const std::string& name, const T& value)
		{
			const auto& found = m_materialConstants.find(name);
			if (found != m_materialConstants.end())
			{
				char* bufferPosition = m_materialBuffer + found->second.offset;
				size_t bufferSize = found->second.stride;
				if (sizeof(value) > bufferSize)
				{
					return;
				}
				std::memcpy(bufferPosition, &value, sizeof(value));
			}
		}

		template<typename T>
		T* GetMaterialConstant(const std::string& name)
		{
			const auto& found = m_materialConstants.find(name);
			if (found != m_materialConstants.end())
			{
				char* bufferPosition = m_materialBuffer + found->second.offset;
				size_t bufferSize = found->second.stride;
				if (sizeof(T) > bufferSize)
				{
					return nullptr;
				}
				return (T*)bufferPosition;
			}
			return nullptr;
		}

		const char* GetRawBuffer() const { return m_materialBuffer; }
		size_t GetBufferSize() const { return m_totalBufferSize; }

	private:
		void ReleaseBuffer();

		void SetRawBuffer(char* materialBuffer)
		{
			if (m_materialBuffer == nullptr)
			{
				m_materialBuffer = new char[m_totalBufferSize];
			}
			std::memcpy(m_materialBuffer, materialBuffer, m_totalBufferSize);
		}


	private:
		char* m_materialBuffer;
		std::unordered_map<std::string, MaterialConstantBufferData> m_materialConstants;
		size_t m_totalBufferSize;

		friend class Material;
	};
}

