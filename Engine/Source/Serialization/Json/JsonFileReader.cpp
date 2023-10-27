#include "EnginePCH.h"
#include "JsonFileReader.h"
#include "PlatformFile.h"

#include <rapidjson/document.h>

namespace Engine
{

	

	JsonFileReader::JsonFileReader(const char* fileName)
		: m_buffer(nullptr), m_document(), m_bufferSize(0)
	{
		FILE* filePath;
        rapidjson::Document mdoc;
        Platform::File::FOpenFile(&filePath, fileName, "rb");
		Parse(filePath);
	}

	JsonFileReader::JsonFileReader(const wchar_t* fileName)
		: m_buffer(nullptr), m_document(), m_bufferSize(0)
	{
		FILE* filePath;
        Platform::File::FOpenFile(&filePath, fileName, L"rb");
		Parse(filePath);
	}

	JsonFileReader::JsonFileReader(const std::wstring& fileName)
		: m_buffer(nullptr), m_document(), m_bufferSize(0)
	{
		FILE* filePath;
        Platform::File::FOpenFile(&filePath, fileName.c_str(), L"rb");
		Parse(filePath);
	}

	JsonFileReader::JsonFileReader(const std::string& fileName)
		: m_buffer(nullptr), m_document(), m_bufferSize(0)
	{
		FILE* filePath;
        Platform::File::FOpenFile(&filePath, fileName.c_str(), "rb");
		Parse(filePath);
	}

	JsonFileReader::~JsonFileReader()
	{
		delete m_buffer;
	}

	ReadJsonObject JsonFileReader::GetJsonObject() const
	{
		return ReadJsonObject(m_document);
	}

	void JsonFileReader::Parse(FILE* file)
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
