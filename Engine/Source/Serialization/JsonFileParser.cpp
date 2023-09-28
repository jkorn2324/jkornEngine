#include "EnginePCH.h"
#include "JsonFileParser.h"

#include "document.h"

namespace Engine
{
	JsonFileParser::JsonFileParser(const char* fileName)
		: m_buffer(nullptr), m_document(), m_bufferSize(0)
	{
		FILE* filePath;
        rapidjson::Document mdoc;
		fopen_s(&filePath, fileName, "rb");
		Parse(filePath);
	}

	JsonFileParser::JsonFileParser(const wchar_t* fileName)
		: m_buffer(nullptr), m_document(), m_bufferSize(0)
	{
		FILE* filePath;
		_wfopen_s(&filePath, fileName, L"rb");
		Parse(filePath);
	}

	JsonFileParser::JsonFileParser(const std::wstring& fileName)
		: m_buffer(nullptr), m_document(), m_bufferSize(0)
	{
		FILE* filePath;
		_wfopen_s(&filePath, fileName.c_str(), L"rb");
		Parse(filePath);
	}

	JsonFileParser::JsonFileParser(const std::string& fileName)
		: m_buffer(nullptr), m_document(), m_bufferSize(0)
	{
		FILE* filePath;
		fopen_s(&filePath, fileName.c_str(), "rb");
		Parse(filePath);
	}

	JsonFileParser::~JsonFileParser()
	{
		delete m_buffer;
	}

	void JsonFileParser::Parse(FILE* file)
	{
		if (file == nullptr)
		{
			return;
		}
		fseek(file, 0, SEEK_END);
		m_bufferSize = (size_t)(ftell(file));
		fseek(file, 0, SEEK_SET);
		m_buffer = new char[m_bufferSize + 1];
		size_t readLength = fread(m_buffer, 1, m_bufferSize, file);
		m_buffer[readLength] = 0;
		fclose(file);
		m_document.Parse(m_buffer);
        JKORN_ENGINE_ASSERT(!m_document.HasParseError(), "Document has trouble parsing the file.");
	}
}
