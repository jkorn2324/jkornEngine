#pragma once

#include <string>
#include "EngineMacros.h"

namespace Engine
{
	enum TextureReadWriteFlags
	{
		Flag_CPU_ReadTexture = 1 << 0,
		Flag_CPU_WriteTexture = 1 << 1,
		Flag_GPU_ReadTexture = 1 << 2,
		Flag_GPU_WriteTexture = 1 << 3
	};

	struct TextureSpecifications
	{
	private:
		static const int c_readWriteFlags = (int)(Flag_GPU_ReadTexture | Flag_GPU_WriteTexture);

	public:
		TextureReadWriteFlags readWriteFlags;
		uint32_t width = 0;
		uint32_t height = 0;

		TextureSpecifications()
			: readWriteFlags((TextureReadWriteFlags)c_readWriteFlags) { }
	};

	template<typename T>
	class AssetSerializer;
	template<typename T>
	class AssetCache;

	class Texture
	{
	public:
		explicit Texture();
		explicit Texture(const TextureSpecifications& specifications);
		virtual ~Texture() { }

		std::uint32_t GetWidth() const;
		std::uint32_t GetHeight() const;
		virtual bool IsValid() const =0;

		virtual void Bind(std::uint32_t textureSlot) const =0;

		virtual const void* GetTextureID() const =0;

		bool IsReadable() const { return m_readWriteFlags & Flag_CPU_ReadTexture; }
		bool IsWritable() const { return m_readWriteFlags & Flag_CPU_WriteTexture; }

		static Texture* CreateTexture();
		static Texture* CreateTexture(const std::wstring& filePath);

	protected:
		virtual bool Load(const wchar_t* texturePath)=0;

	protected:
		std::uint32_t m_width, m_height;
		TextureReadWriteFlags m_readWriteFlags;

	private:
		static Texture* Create(const TextureSpecifications& specifications);
		SERIALIZABLE_ASSET(Texture);
		
		friend class GraphicsRenderer;
	};
}