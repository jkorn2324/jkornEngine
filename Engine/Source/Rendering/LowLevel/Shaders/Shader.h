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
		static bool Create(Shader** shader);
		static bool Create(std::shared_ptr<Shader>& shader);
		static bool Create(Shader** shader, const BufferLayout& bufferLayout);
		static bool Create(std::shared_ptr<Shader>& shader, const BufferLayout& bufferLayout);

		static bool LoadFromFile(Shader** shader, 
			const wchar_t* fileName, const BufferLayout& bufferLayout);
		static bool LoadFromFile(std::shared_ptr<Shader>& shader,
			const wchar_t* fileName, const BufferLayout& bufferLayout);
	};
}