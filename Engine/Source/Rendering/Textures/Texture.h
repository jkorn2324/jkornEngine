#pragma once

namespace Engine
{

	class Texture
	{
	public:
		explicit Texture();
		virtual ~Texture() { }

		std::uint32_t GetWidth() const;
		std::uint32_t GetHeight() const;
		virtual bool IsValid() const =0;

		virtual void Bind(std::uint32_t textureSlot) const =0;

	protected:
		virtual bool Load(const wchar_t* texturePath)=0;

	protected:
		std::uint32_t m_width, m_height;

	private:
		static Texture* Create();

	public:
		static Texture* StaticLoad(const std::wstring& texturePath);

		friend class GraphicsRenderer;
	};
}