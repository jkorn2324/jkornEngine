#include "EnginePCH.h"
#include "JsonObjects.h"

namespace Engine
{

	// ---------------------------------------- Parsed Json Object ----------------------------------------------


	ReadJsonObject::ReadJsonObject(GenericObject& object)
		: m_object(object), m_valid(true)
	{
	}

	ReadJsonObject::ReadJsonObject(rapidjson::Document& document)
		: m_object(document.GetObject()), m_valid(true) 
	{ 
	}

	ReadJsonObject::ReadJsonObject(const ReadJsonObject& jsonObject)
		: m_object(jsonObject.m_object), m_valid(jsonObject.m_valid)
	{

	}

	ReadJsonObject::ReadJsonObject(const rapidjson::Document& document)
		: m_object(const_cast<rapidjson::Document&>(document).GetObject()), m_valid(true) 
	{ 
	}

	ReadJsonObject& ReadJsonObject::operator=(const ReadJsonObject& jsonObject)
	{
		m_object = jsonObject.m_object;
		m_valid = jsonObject.m_valid;
		return *this;
	}

	bool ReadJsonObject::GetValue(const std::string& inString, ReadJsonValue& inValue) const
	{
		if (!IsValid()) return false;
		if (m_object.HasMember(inString.c_str())) return false;
		inValue = ReadJsonValue(m_object[inString.c_str()]);
		return true;
	}

	// ------------------------------------------ Parsed Json Array ----------------------------------------------

	ReadJsonArray::ReadJsonArray(GenericArray& inArray)
		: m_array(inArray), m_valid(true) 
	{

	}

	ReadJsonArray::ReadJsonArray(const ReadJsonArray& inArray)
		: m_array(inArray.m_array), m_valid(inArray.m_valid)
	{ 
	
	}

	ReadJsonArray& ReadJsonArray::operator=(const ReadJsonArray& jsonArray)
	{
		m_array = jsonArray.m_array;
		m_valid = jsonArray.m_valid;
		return *this;
	}

	bool ReadJsonArray::GetElementAt(size_t index, ReadJsonValue& inValue) const
	{
		if (index >= GetSize())
		{
			return false;
		}
		auto value = m_array.Begin() + index;
		inValue = ReadJsonValue(*value);
		return true;
	}

	// ------------------------------------------- Parsed Json Value ----------------------------------------------

	rapidjson::Value ReadJsonValue::s_defaultValue;

	ReadJsonValue::ReadJsonValue()
		: m_value(s_defaultValue), m_valid(false)
	{

	}

	ReadJsonValue::ReadJsonValue(rapidjson::Value& value)
		: m_value(value), m_valid(true) 
	{ 
	
	}

	ReadJsonValue::ReadJsonValue(const rapidjson::Value& value)
		: m_value(const_cast<rapidjson::Value&>(value)), m_valid(true) 
	{ 

	}
	
	ReadJsonValue::ReadJsonValue(const ReadJsonValue& jsonValue)
		: m_value(jsonValue.m_value), m_valid(jsonValue.m_valid)
	{

	}

	ReadJsonValue& ReadJsonValue::operator=(const ReadJsonValue& value)
	{
		m_value = value.m_value;
		m_valid = value.m_valid;
		return *this;
	}

	bool ReadJsonValue::GetJsonObject(ReadJsonObject& jsonObject) const
	{
		if (IsValid()
			&& m_value.IsObject())
		{
			jsonObject = ReadJsonObject(m_value.GetObject());
			return true;
		}
		return false;
	}

	bool ReadJsonValue::GetJsonArray(ReadJsonArray& jsonArray) const
	{
		if (IsArray())
		{
			jsonArray = ReadJsonArray(m_value.GetArray());
			return true;
		}
		return false;
	}
}