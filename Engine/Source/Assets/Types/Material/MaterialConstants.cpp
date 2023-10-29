#include "EnginePCH.h"
#include "MaterialConstants.h"

#include "Memory.h"

namespace Engine
{
	MaterialConstants::MaterialConstants(const MaterialConstantsLayout& materialConstantsLayout)
		: m_materialConstants(),
		m_layoutSize(0)
	{
		SetLayout(materialConstantsLayout);
	}

	void MaterialConstants::SetLayout(const MaterialConstantsLayout& layout)
	{
		m_materialConstants.clear();

		size_t totalLayoutSize = 0;
		for (size_t i = 0; i < layout.layoutAttributes.size(); i++)
		{
			MaterialConstantLayoutAttribute attribute = layout.layoutAttributes[i];
			uint32_t offset = (uint32_t)totalLayoutSize;
			totalLayoutSize += attribute.layoutStride;

			if (!attribute.addPadding)
			{
				m_materialConstants.emplace(attribute.name,
					MaterialConstantBufferData{ offset, attribute.layoutStride, attribute.layoutType, attribute.addPadding });
			}
		}
		m_layoutSize = totalLayoutSize;
	}
}