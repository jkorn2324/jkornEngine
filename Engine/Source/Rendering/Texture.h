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

		bool Load(const wchar_t* texturePath);

	private:
		void Free();
		void Bind(std::uint32_t slot) const;

	private:
		ID3D11Resource* m_texture;
		ID3D11ShaderResourceView* m_shaderResourceView;

		std::uint32_t m_width, m_height;
	};
}