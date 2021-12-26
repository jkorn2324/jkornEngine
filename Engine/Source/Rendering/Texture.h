#pragma once

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

namespace Engine
{

	class Texture
	{

	public:
		explicit Texture();
		~Texture();

		static Texture* StaticLoad(const std::wstring& texturePath);

	private:
		void Free();
		void Bind(std::uint32_t textureSlot) const;
		bool Load(const wchar_t* texturePath);

	private:
		ID3D11Resource* m_texture;
		ID3D11ShaderResourceView* m_shaderResourceView;

		std::uint32_t m_width, m_height;
	};
}