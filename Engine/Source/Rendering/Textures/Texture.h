#pragma once

#include "EngineMacros.h"
#include "GraphicsFormat.h"
#include "Buffer.h"

#include <string>

namespace MathLib
{
	class Vector4;
}

namespace Engine
{
	enum TextureReadWriteFlags
	{
		Flag_CPU_ReadTexture = 1 << 0,
		Flag_CPU_WriteTexture = 1 << 1,
		Flag_GPU_ReadTexture = 1 << 2,
		Flag_GPU_WriteTexture = 1 << 3
	};

    enum TextureFlags
    {
        TextureFlags_None = 0,
        TextureFlags_Temporary = 1 << 0,
        TextureFlags_IsRenderTarget = 1 << 1
    };

	static const int c_readWriteFlags = (int)(Flag_GPU_ReadTexture | Flag_GPU_WriteTexture);

	struct TextureSpecifications
	{
		TextureReadWriteFlags readWriteFlags;
		GraphicsFormat textureFormat;
        TextureFlags textureFlags;

		TextureSpecifications()
			: readWriteFlags((TextureReadWriteFlags)c_readWriteFlags), textureFormat(GraphicsFormat_RGBA32_Float), textureFlags(TextureFlags_IsRenderTarget) { }
		TextureSpecifications(uint32_t flags, GraphicsFormat textureFormat)
			: readWriteFlags((TextureReadWriteFlags)flags), textureFormat(textureFormat), textureFlags(TextureFlags_IsRenderTarget) { }
        
        TextureSpecifications(uint32_t readWriteFlags, GraphicsFormat textureFormat, uint32_t textureFlags)
            : readWriteFlags((TextureReadWriteFlags)readWriteFlags), textureFormat(textureFormat), textureFlags((TextureFlags)textureFlags) { }
	};

	class Texture
	{
	public:
		explicit Texture();
		explicit Texture(uint32_t width, uint32_t height, const TextureSpecifications& specifications);
		virtual ~Texture() { }

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

		virtual bool IsValid() const =0;
		virtual void Bind(uint32_t textureSlot) const =0;
		virtual const void* GetTextureID() const =0;

		GraphicsFormat GetTextureFormat() const { return m_specifications.textureFormat; }

		bool IsReadable() const { return m_specifications.readWriteFlags & Flag_CPU_ReadTexture; }
		bool IsWritable() const { return m_specifications.readWriteFlags & Flag_CPU_WriteTexture; }
        
        bool IsRenderTarget() const { return m_specifications.textureFlags & TextureFlags_IsRenderTarget; }

		virtual bool GetPixel(uint32_t x, uint32_t y, MathLib::Vector4& pixel) const =0;
		virtual void SetPixel(uint32_t x, uint32_t y, const MathLib::Vector4& pixel) =0;

		template<typename TAllocator = DefaultAllocator>
		void CopyPixels(FixedRuntimeBuffer<TAllocator>& buffer)
		{
			size_t formatSize = Graphics::SizeOfFormat(GetTextureFormat());
			size_t bufferSize = formatSize * m_width * m_height;
			buffer = FixedRuntimeBuffer<TAllocator>(bufferSize);
			BufferModifier view = BufferModifier::Create(buffer);
			CopyPixels(view);
		}

		virtual void CopyPixels(BufferModifier& pixels) const = 0;

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
