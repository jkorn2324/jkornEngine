#pragma once

#include "Texture.h"
#include <d3d11.h>

namespace Engine
{

	class DirectX11Texture : public Texture
	{
	public:
		explicit DirectX11Texture();
		explicit DirectX11Texture(const TextureSpecifications& flags);
		explicit DirectX11Texture(ID3D11Resource* resource,
			ID3D11ShaderResourceView* shaderResourceView);
		~DirectX11Texture();

		bool IsValid() const override;
		void Bind(const std::uint32_t slot) const override;

		const void* GetTextureID() const override;

		bool GetPixel(uint32_t x, uint32_t y, MathLib::Vector4& color) const override;
		void SetPixel(uint32_t x, uint32_t y, const MathLib::Vector4& pixelColor) override;

		void CopyPixels(FixedArray& pixelArray) const override;
	
	protected:
		bool Load(const wchar_t* texturePath, const TextureSerializedData& specifications) override;
		void Free();
		bool CopyTo(Texture& texture) override;

	private:
		static class DirectX11RenderingAPI& GetRenderingAPI();
		void UpdatePixels() const;

	private:
		ID3D11Resource* m_texture;
		ID3D11ShaderResourceView* m_shaderResourceView;
		uint32_t* m_pixels;

		friend class DirectX11FrameBuffer;
	};
}