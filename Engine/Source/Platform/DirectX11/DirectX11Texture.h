#pragma once

#include "Texture.h"
#include <d3d11.h>

namespace Engine
{

	class DirectX11Texture : public Texture
	{
	public:
		explicit DirectX11Texture();
		~DirectX11Texture();

		bool IsValid() const override;

	protected:
		bool Load(const wchar_t* texturePath);
		void Bind(const std::uint32_t slot) const override;
		void Free();

	private:
		ID3D11Resource* m_texture;
		ID3D11ShaderResourceView* m_shaderResourceView;
	};
}