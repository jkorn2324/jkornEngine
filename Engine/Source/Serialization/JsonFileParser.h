#pragma once

#include <rapidjson\document.h>
#include <string>

namespace Engine
{
	class JsonFileParser
	{
	public:
		JsonFileParser(const char* fileName);
		JsonFileParser(const wchar_t* fileName);
		JsonFileParser(const std::wstring& fileName);
		JsonFileParser(const std::string& fileName);
		~JsonFileParser();

		rapidjson::Document& GetDocument() { return m_document; }
		const rapidjson::Document& GetDocument() const { return m_document; }

		bool IsValid() const { return m_buffer != nullptr && !m_document.HasParseError(); }

	private:
		void Parse(FILE* file);

	private:
		char* m_buffer;
		rapidjson::Document m_document;
		size_t m_bufferSize;
	};
}