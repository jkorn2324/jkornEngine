#include "EnginePCH.h"
#include "Texture.h"

#include <d3d11.h>

namespace Engine
{

	Texture::Texture()
		: m_texture(nullptr),
		m_shaderResourceView(nullptr),
		m_width(0),
		m_height(0)
	{
		// TODO:
	}

	Texture::~Texture()
	{
		if (m_shaderResourceView != nullptr)
		{
			m_shaderResourceView->Release();
		}
		if (m_texture != nullptr)
		{
			m_texture->Release();
		}
	}

	void Texture::Bind() const
	{
		// TODO: 
	}
}