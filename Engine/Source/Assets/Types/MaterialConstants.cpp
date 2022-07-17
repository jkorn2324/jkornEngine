#include "EnginePCH.h"
#include "MaterialConstants.h"

#include "Memory.h"

namespace Engine
{
	MaterialConstants::MaterialConstants(const MaterialConstantsLayout& materialConstantsLayout)
		: m_totalBufferSize(0),
		m_materialConstants(),
		m_materialBuffer(nullptr)
	{
		SetLayout(materialConstantsLayout);
	}

	MaterialConstants::MaterialConstants(const MaterialConstantsLayout& materialConstantsLayout, size_t totalBufferSize)
		: m_totalBufferSize(0),
		m_materialConstants(),
		m_materialBuffer(nullptr)
	{
		SetLayout(materialConstantsLayout, totalBufferSize);
	}

	MaterialConstants::MaterialConstants(const MaterialConstants& constants)
		: m_totalBufferSize(constants.GetBufferSize()),
		m_materialConstants(constants.m_materialConstants),
		m_materialBuffer(nullptr)
	{
		m_materialBuffer = new char[m_totalBufferSize];
		if (constants.m_materialBuffer != nullptr)
		{
			Memory::Memcpy(m_materialBuffer, constants.GetRawBuffer(), m_totalBufferSize);
		}
	}

	MaterialConstants& MaterialConstants::operator=(const MaterialConstants& constants)
	{
		if (m_materialBuffer != nullptr)
		{
			delete[] m_materialBuffer;
		}
		m_totalBufferSize = constants.GetBufferSize();
		m_materialConstants = constants.m_materialConstants;
		m_materialBuffer = new char[m_totalBufferSize];
		if (constants.m_materialBuffer != nullptr)
		{
			Memory::Memcpy(m_materialBuffer, constants.GetRawBuffer(), m_totalBufferSize);
		}
		return *this;
	}

	void MaterialConstants::SetLayout(const MaterialConstantsLayout& layout)
	{
		ReleaseBuffer();

		m_totalBufferSize = 0;
		m_materialConstants.clear();

		for (size_t i = 0; i < layout.layoutAttributes.size(); i++)
		{
			MaterialConstantLayoutAttribute attribute = layout.layoutAttributes[i];
			uint32_t offset = (uint32_t)m_totalBufferSize;
			m_totalBufferSize += attribute.layoutStride;

			if (!attribute.pad)
			{
				m_materialConstants.emplace(attribute.name,
					MaterialConstantBufferData{ offset, attribute.layoutStride, attribute.layoutType, attribute.pad });
			}
		}
		m_materialBuffer = new char[m_totalBufferSize];
	}

	void MaterialConstants::SetLayout(const MaterialConstantsLayout& layout, size_t totalBufferSize)
	{
		ReleaseBuffer();

		m_totalBufferSize = totalBufferSize;
		m_materialConstants.clear();

		for(size_t i = 0; i < layout.layoutAttributes.size(); i++)
		{
			MaterialConstantLayoutAttribute attribute = layout.layoutAttributes[i];
			uint32_t offset = (uint32_t)m_totalBufferSize;
			if (!attribute.pad)
			{
				m_materialConstants.emplace(attribute.name,
					MaterialConstantBufferData{ offset, attribute.layoutStride, attribute.layoutType, attribute.pad });
			}
		}
		m_materialBuffer = new char[m_totalBufferSize];
	}

	MaterialConstants::~MaterialConstants()
	{
		ReleaseBuffer();
	}

	void MaterialConstants::ReleaseBuffer()
	{
		if (m_materialBuffer != nullptr)
		{
			delete[] m_materialBuffer;
			m_materialBuffer = nullptr;
		}
	}

	void MaterialConstants::SetRawBuffer(char* materialBuffer)
	{
		if (m_materialBuffer == nullptr)
		{
			m_materialBuffer = new char[m_totalBufferSize];
		}
		Memory::Memcpy(m_materialBuffer, materialBuffer, m_totalBufferSize);
	}
}