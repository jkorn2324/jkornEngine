#pragma once

#include "BufferLayout.h"
#include "EngineMacros.h"
#include "GUID.h"

namespace Engine
{
	class Shader
	{
	public:
		explicit Shader() = default;
		explicit Shader(const BufferLayout& bufferLayout);
		virtual ~Shader() { }

		virtual bool IsValid() const = 0;
		virtual void Bind() const = 0;

		const BufferLayout& GetBufferLayout() const { return m_bufferLayout; }

	protected:
		virtual bool LoadFromFile_Internal(const wchar_t* fileName) =0;
		bool LoadFromFile_Internal(const wchar_t* fileName, const BufferLayout& bufferLayout);

	protected:
		BufferLayout m_bufferLayout;

	public:

		static Shader* Create()
		{
			BufferLayout layout;
			return Create(layout, true);
		}

		/**
		 * Creates a shader with a buffer layout. 
		 */
		static Shader* Create(const BufferLayout& bufferLayout)
		{
			return Create(bufferLayout, false);
		}

		/**
		 * Loads a shader with a buffer layout from a file. 
		 */
		static Shader* LoadFromFile(const wchar_t* fileName, const BufferLayout& layout);

	private:
		static Shader* Create(const BufferLayout& bufferLayout, bool empty);
	};
}
