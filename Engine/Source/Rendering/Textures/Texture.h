#pragma once

#include <string>
#include "EngineMacros.h"

namespace Engine
{
	template<typename T>
	class AssetSerializer;
	template<typename T>
	class AssetCache;

	class Texture
	{
	public:
		explicit Texture();
		virtual ~Texture() { }

		std::uint32_t GetWidth() const;
		std::uint32_t GetHeight() const;
		virtual bool IsValid() const =0;

		virtual void Bind(std::uint32_t textureSlot) const =0;

		virtual const void* GetTextureID() const =0;

		static Texture* CreateTexture();
		static Texture* CreateTexture(const std::wstring& filePath);

	protected:
		virtual bool Load(const wchar_t* texturePath)=0;

	protected:
		std::uint32_t m_width, m_height;

	private:
		SERIALIZABLE_ASSET(Texture);
		
		friend class GraphicsRenderer;
	};
}