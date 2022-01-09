#pragma once

#include "Texture.h"
#include <d3d11.h>

namespace Engine
{

	class DirectX11Texture : public Texture
	{
	public:
		explicit DirectX11Texture();
		explicit DirectX11Texture(ID3D11Resource* resource,
			ID3D11ShaderResourceView* shaderResourceView);
		~DirectX11Texture();

		bool IsValid() const override;
		void Bind(const std::uint32_t slot) const override;

	protected:
		bool Load(const wchar_t* texturePath);
		void Free();

	private:
		ID3D11Resource* m_texture;
		ID3D11ShaderResourceView* m_shaderResourceView;
	};
}