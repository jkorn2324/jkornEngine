#pragma once

#include "BufferLayout.h"

namespace Engine
{
	class Shader
	{
	public:
		explicit Shader() = default;
		virtual ~Shader() { }

		virtual bool IsValid() const=0;
		virtual void Bind() const =0;

	protected:
		virtual bool Load(const wchar_t* fileName, const BufferLayout& bufferLayout)=0;

	public:
		static Shader* StaticLoad(const std::wstring& fileName, const BufferLayout& bufferLayout);
		
	private:
		static Shader* Create();
	};
}