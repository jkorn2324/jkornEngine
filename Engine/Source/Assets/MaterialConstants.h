#pragma once

#include <string>
#include <vector>
#include <initializer_list>
#include <unordered_map>

namespace Engine
{

	struct MaterialConstantLayoutAttribute
	{
		std::string name;
		size_t layoutStride;
		bool pad;

		MaterialConstantLayoutAttribute(const std::string& name, size_t layoutStride, bool pad = false)
			: name(name), layoutStride(layoutStride), pad(pad) { }
	};

	struct MaterialConstantsLayout
	{
		std::vector<MaterialConstantLayoutAttribute> layoutAttributes;

		MaterialConstantsLayout(const std::initializer_list<MaterialConstantLayoutAttribute>& layout)
			: layoutAttributes(layout) { }
	};

	struct MaterialConstantBufferData
	{
		uint32_t offset;
		size_t stride;
	};

	class MaterialConstants
	{
	public:
		MaterialConstants(const MaterialConstantsLayout& materialConstantsLayout);
		~MaterialConstants();

		template<typename T>
		void SetMaterialConstant(const std::string& name, T value)
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
		char* m_materialBuffer;
		std::unordered_map<std::string, MaterialConstantBufferData> m_materialConstants;
		size_t m_totalBufferSize;
	};
}

