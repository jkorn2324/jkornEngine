#pragma once

#include <filesystem>

#if defined(PLATFORM_MACOSX)
#include "prettywriter.h"
#include "stringbuffer.h"
#endif

#if defined(PLATFORM_WINDOWS)
#include <rapidjson\prettywriter.h>
#include <rapidjson\stringbuffer.h>
#endif

namespace Engine
{
	class JsonFileWriter
	{
	public:
		JsonFileWriter(const std::filesystem::path& path);
		~JsonFileWriter();

	public:
		template<typename T>
		void Write(const std::string& name, const T& value);
		void Write(const std::string& name, const char*& value);
		void Write(const std::string& name, const char*& value, size_t size);

		void BeginArray(const std::string& name);
		void EndArray();

		void BeginObject();
		void BeginObject(const std::string& name);
		void EndObject();

		bool Flush();

	private:
		rapidjson::PrettyWriter<rapidjson::StringBuffer> m_prettyWriter;
		rapidjson::StringBuffer m_stringBuffer;
		std::filesystem::path m_path;
	};
}
