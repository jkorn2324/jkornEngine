#pragma once

#include "BufferLayout.h"
#include "EngineMacros.h"
#include "GUID.h"

namespace Engine
{
	template<typename T>
	class AssetSerializer;
	template<typename T>
	class AssetCache;

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
		virtual bool Load(const wchar_t* fileName, const BufferLayout& bufferLayout) = 0;

	protected:
		BufferLayout m_bufferLayout;

	private:
		static bool DeserializeFromFile(Shader& shader, struct AssetDeserializationFileData& deserializeData, const BufferLayout& bufferLayout);

		SERIALIZABLE_ASSET(Shader);
	};
}