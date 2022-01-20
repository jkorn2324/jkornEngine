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

	MaterialConstants::~MaterialConstants()
	{
		delete[] m_materialBuffer;
	}
}