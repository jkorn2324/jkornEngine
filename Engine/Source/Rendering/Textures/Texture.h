#pragma once

#include "EngineMacros.h"

#include <memory>
#include <string>

namespace MathLib
{
	class Vector4;
}

namespace Engine
{
	class FixedArray;

	enum TextureReadWriteFlags
	{
		Flag_CPU_ReadTexture = 1 << 0,
		Flag_CPU_WriteTexture = 1 << 1,
		Flag_GPU_ReadTexture = 1 << 2,
		Flag_GPU_WriteTexture = 1 << 3
	};

	enum TextureFormat
	{
		TextureFormat_Unknown,
		// 1 Byte Per Color Channel, 1 32 Bit Integer
		TextureFormat_RGBA8,
		// 4 Floats, 1 Float Per Color Channel
		TextureFormat_RGBA32,
		// 4 Bytes Per Channel, 4 Floats
		TextureFormat_ARGB32,
		// Non Color Channel, 1 32 Bit Integer
		TextureFormat_Int32,
		// 1 32 Bit Float
		TextureFormat_Float32,
		// 1 8 Bit Int
		TextureFormat_Int8,
		// 1 16 Bit Int
		TextureFormat_Int16,
	};

	static size_t SizeOfFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat_ARGB32: return sizeof(float) * 4;
		case TextureFormat_RGBA32: return sizeof(float) * 4;
		case TextureFormat_Float32: return sizeof(float);
		case TextureFormat_Int32: return sizeof(uint32_t);
		case TextureFormat_Int16: return sizeof(uint16_t);
		case TextureFormat_Int8: return sizeof(uint8_t);
		case TextureFormat_RGBA8: return sizeof(float);
		}
		return 0;
	}

	static const int c_readWriteFlags = (int)(Flag_GPU_ReadTexture | Flag_GPU_WriteTexture);

	struct TextureSpecifications
	{
		TextureReadWriteFlags readWriteFlags;
		TextureFormat textureFormat;

		TextureSpecifications()
			: readWriteFlags((TextureReadWriteFlags)c_readWriteFlags), textureFormat(TextureFormat_RGBA32) { }
		TextureSpecifications(uint32_t flags, TextureFormat textureFormat)
			: readWriteFlags((TextureReadWriteFlags)flags), textureFormat(textureFormat) { }
	};

	template<typename T>
	class AssetSerializer;
	template<typename T>
	class AssetCache;

	class Texture
	{
	public:
		explicit Texture();
		explicit Texture(uint32_t width, uint32_t height, const TextureSpecifications& specifications);
		virtual ~Texture() { }

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

		virtual bool IsValid() const =0;
		virtual void Bind(std::uint32_t textureSlot) const =0;
		virtual const void* GetTextureID() const =0;

		TextureFormat GetTextureFormat() const { return m_specifications.textureFormat; }

		bool IsReadable() const { return m_specifications.readWriteFlags & Flag_CPU_ReadTexture; }
		bool IsWritable() const { return m_specifications.readWriteFlags & Flag_CPU_WriteTexture; }

		virtual bool GetPixel(uint32_t x, uint32_t y, MathLib::Vector4& pixel) const =0;
		virtual void SetPixel(uint32_t x, uint32_t y, const MathLib::Vector4& pixel) =0;

		virtual void CopyPixels(FixedArray& pixelArray) const = 0;

		friend bool CopyTexture(Texture& a, Texture& b);
		static bool CopyTexture(Texture& a, Texture& b);

	protected:
		bool LoadFromFile_Internal(const wchar_t* texturePath, const TextureSpecifications& specifications);
		virtual bool LoadFromFile_Internal(const wchar_t* texturePath) =0;
		virtual bool CopyTo(Texture& b) = 0;

	protected:
		TextureSpecifications m_specifications;
		uint32_t m_width, m_height;

	public:
		static bool Create(Texture** texture);
		static bool Create(Texture** texture, uint32_t width, uint32_t height, 
			const TextureSpecifications& specifications);
		static bool Create(std::shared_ptr<Texture>& texture);
		static bool Create(std::shared_ptr<Texture>& texture, uint32_t width, uint32_t height, 
			const TextureSpecifications& specifications);

		static bool LoadFromFile(Texture** texture,
			const wchar_t* texturePath);
		
		static bool LoadFromFile(Texture** texture,
			const wchar_t* texturePath,
			const TextureSpecifications& specifications);
		
		static bool LoadFromFile(std::shared_ptr<Texture>& texture,
			const wchar_t* texturePath);

		static bool LoadFromFile(std::shared_ptr<Texture>& texture,
			const wchar_t* texturePath,
			const TextureSpecifications& specifications);

		friend class GraphicsRenderer;
	};
}