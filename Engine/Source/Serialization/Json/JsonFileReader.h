#pragma once

#include <string>
#include <rapidjson/document.h>

#include "JsonObjects.h"

namespace Engine
{

	class JsonFileReader
	{
	public:
		JsonFileReader(const char* fileName);
		JsonFileReader(const wchar_t* fileName);
		JsonFileReader(const std::wstring& fileName);
		JsonFileReader(const std::string& fileName);
		~JsonFileReader();

		ReadJsonObject GetJsonObject() const;

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
