#include "EnginePCH.h"
#include "MaterialConstants.h"

namespace Engine
{
	MaterialConstants::MaterialConstants(const MaterialConstantsLayout& materialConstantsLayout)
		: m_totalBufferSize(0),
		m_materialConstants(),
		m_materialBuffer(nullptr)
	{
		for (size_t i = 0; i < materialConstantsLayout.layoutAttributes.size(); i++)
		{
			MaterialConstantLayoutAttribute attribute = materialConstantsLayout.layoutAttributes[i];
			uint32_t offset = (uint32_t)m_totalBufferSize;
			m_totalBufferSize += attribute.layoutStride;

			if (!attribute.pad)
			{
				m_materialConstants.emplace(attribute.name,
					MaterialConstantBufferData{ offset, attribute.layoutStride });
			}
		}
		m_materialBuffer = new char[m_totalBufferSize];
	}

	MaterialConstants::MaterialConstants(const MaterialConstants& constants)
		: m_totalBufferSize(constants.GetBufferSize()),
		m_materialConstants(constants.m_materialConstants),
		m_materialBuffer(nullptr)
	{
		m_materialBuffer = new char[m_totalBufferSize];
		if (constants.m_materialBuffer != nullptr)
		{
			std::memcpy(m_materialBuffer, constants.GetRawBuffer(), m_totalBufferSize);
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
			std::memcpy(m_materialBuffer, constants.GetRawBuffer(), m_totalBufferSize);
		}
		return *this;
	}

	MaterialConstants::~MaterialConstants()
	{
		delete[] m_materialBuffer;
	}

}