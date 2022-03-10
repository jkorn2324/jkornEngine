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
		virtual ~Shader() { }

		virtual bool IsValid() const=0;
		virtual void Bind() const =0;

	protected:
		virtual bool Load(const wchar_t* fileName, const BufferLayout& bufferLayout)=0;

	private:
		static bool DeserializeFromFile(Shader& shader, struct AssetDeserializationFileData& deserializeData, const BufferLayout& bufferLayout);
		SERIALIZABLE_ASSET(Shader);
	};
}