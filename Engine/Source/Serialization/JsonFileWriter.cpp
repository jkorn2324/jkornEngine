#include "EnginePCH.h"
#include "JsonFileWriter.h"

#include "JsonUtils.h"
#include "Vector.h"
#include "GUID.h"

namespace Engine
{
	JsonFileWriter::JsonFileWriter(const std::filesystem::path& path)
		: m_path(path), m_stringBuffer(), m_prettyWriter(m_stringBuffer)
	{
		m_prettyWriter.StartObject();
	}

	JsonFileWriter::~JsonFileWriter()
	{
	}
	
	bool JsonFileWriter::Flush()
	{
		m_prettyWriter.EndObject();

#ifdef PLATFORM_WINDOWS
		FILE* file;
		errno_t errorCode = _wfopen_s(&file, m_path.c_str(), L"w");
		if (file == nullptr)
		{
			return false;
		}
		fprintf_s(file, "%s", m_stringBuffer.GetString());
		fclose(file);
		return true;
#endif
		return false;
	}

	void JsonFileWriter::BeginArray(const std::string& name) 
	{
		WriteString(m_prettyWriter, name);
		m_prettyWriter.StartArray();
	}

	void JsonFileWriter::EndArray()
	{
		m_prettyWriter.EndArray();
	}

	void JsonFileWriter::BeginObject()
	{
		m_prettyWriter.StartObject();
	}

	void JsonFileWriter::BeginObject(const std::string& name)
	{
		WriteString(m_prettyWriter, name);
		BeginObject();
	}

	void JsonFileWriter::EndObject()
	{
		m_prettyWriter.EndObject();
	}

	void JsonFileWriter::Write(const std::string& name, const char*& value)
	{
		Write(name, value, strlen(value));
	}

	void JsonFileWriter::Write(const std::string& name, const char*& value, size_t size)
	{
		WriteString(m_prettyWriter, name);
		m_prettyWriter.String(value, (rapidjson::SizeType)size);
	}

	template<typename T>
	inline void JsonFileWriter::Write(const std::string& name, const T& value)
	{
		static_assert(false);
	}

	// Writes an unsigned int 16.
	template<>
	void JsonFileWriter::Write(const std::string& name, const uint16_t& value)
	{
		WriteString(m_prettyWriter, name);
		m_prettyWriter.Uint((uint32_t)value);
	}

	// Writes a int 16.
	template<>
	void JsonFileWriter::Write(const std::string& name, const int16_t& value)
	{
		WriteString(m_prettyWriter, name);
		m_prettyWriter.Int((int32_t)value);
	}

	// Writes an unsigned int 32.
	template<>
	void JsonFileWriter::Write(const std::string& name, const uint32_t& value)
	{
		WriteString(m_prettyWriter, name);
		m_prettyWriter.Uint(value);
	}
	
	// Writes a int 32.
	template<>
	void JsonFileWriter::Write(const std::string& name, const int32_t& value)
	{
		WriteString(m_prettyWriter, name);
		m_prettyWriter.Int(value);
	}

	// Writes a int 64.
	template<>
	void JsonFileWriter::Write(const std::string& name, const int64_t& value)
	{
		WriteString(m_prettyWriter, name);
		m_prettyWriter.Int64(value);
	}

	// Writes an unsigned int 64.
	template<>
	void JsonFileWriter::Write(const std::string& name, const uint64_t& value)
	{
		WriteString(m_prettyWriter, name);
		m_prettyWriter.Uint64(value);
	}

	// Writes a float.
	template<>
	void JsonFileWriter::Write(const std::string& name, const float& value)
	{
		WriteString(m_prettyWriter, name);
		m_prettyWriter.Double((double)value);
	}

	// Writes a double.
	template<>
	void JsonFileWriter::Write(const std::string& name, const double& value)
	{
		WriteString(m_prettyWriter, name);
		m_prettyWriter.Double(value);
	}

	// Writes a bool.
	template<>
	void JsonFileWriter::Write(const std::string& name, const bool& value)
	{
		WriteString(m_prettyWriter, name);
		m_prettyWriter.Bool(value);
	}

	// Writes a string.
	template<>
	void JsonFileWriter::Write(const std::string& name, const std::string& value)
	{
		WriteString(m_prettyWriter, name);
		WriteString(m_prettyWriter, value);
	}

	// Writes a Vector2.
	template<>
	void JsonFileWriter::Write(const std::string& name, const MathLib::Vector2& value)
	{
		WriteString(m_prettyWriter, name);
		BeginObject();
		Write("x", value.x);
		Write("y", value.y);
		EndObject();
	}

	// Writes a Vector3.
	template<>
	void JsonFileWriter::Write(const std::string& name, const MathLib::Vector3& value)
	{
		WriteString(m_prettyWriter, name);
		BeginObject();
		Write("x", value.x);
		Write("y", value.y);
		Write("z", value.z);
		EndObject();
	}

	// Writes a Vector4.
	template<>
	void JsonFileWriter::Write(const std::string& name, const MathLib::Vector4& value)
	{
		WriteString(m_prettyWriter, name);
		BeginObject();
		Write("x", value.x);
		Write("y", value.y);
		Write("z", value.z);
		Write("w", value.w);
		EndObject();
	}

	// Writes a Quaternion.
	template<>
	void JsonFileWriter::Write(const std::string& name, const MathLib::Quaternion& value)
	{
		WriteString(m_prettyWriter, name);
		BeginObject();
		Write("x", value.x);
		Write("y", value.y);
		Write("z", value.z);
		Write("w", value.w);
		EndObject();
	}

	template<>
	void JsonFileWriter::Write(const std::string& name, const GUID& guid)
	{
		Write(name, (uint64_t)guid);
	}
}
