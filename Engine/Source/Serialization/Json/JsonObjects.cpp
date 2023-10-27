#include "EnginePCH.h"
#include "JsonObjects.h"

namespace Engine
{


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

	bool ReadJsonValue::GetJsonArray(ReadJsonArray& jsonArray) const
	{
		if (IsArray())
		{
            auto arr = m_value.GetArray();
			jsonArray = ReadJsonArray(arr);
			return true;
		}
		return false;
	}
}
