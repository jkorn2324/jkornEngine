#pragma once

#include <rapidjson/document.h>
#include <string>

namespace Engine
{

	class ReadJsonValue;

	/// <summary>
	/// Parsed Json Object - Wrapper for the rapidjson object.
	/// </summary>
    template<bool TIsConst = false>
	class ReadJsonObject
	{
	private:
		using GenericObject = rapidjson::GenericObject<TIsConst, rapidjson::Value>;
		static GenericObject s_defaultObject;

	public:
		ReadJsonObject()
			: m_object(s_defaultObject), m_valid(false)
		{
		}

		ReadJsonObject(GenericObject& object)
            : m_object(object), m_valid(true)
        {
            
        }
        
		ReadJsonObject(const ReadJsonObject& jsonObject)
            : m_object(jsonObject.m_object), m_valid(jsonObject.m_valid)
        {
            
        }

		ReadJsonObject& operator=(const ReadJsonObject& jsonObject)
        {
            m_object = jsonObject.m_object;
            m_valid = jsonObject.m_valid;
            return *this;
        }

		const bool IsValid() const { return m_valid; }

		bool GetValue(const std::string& inString, ReadJsonValue& inValue) const
        {
            if (!IsValid()) return false;
            if (m_object.HasMember(inString.c_str())) return false;
            inValue = ReadJsonValue(m_object[inString.c_str()]);
            return true;
        }

	private:
		GenericObject& m_object;
		bool m_valid;
	};

	/// <summary>
	/// Parsed Json Array - Used to get Elements of the Array.
	/// </summary>
	class ReadJsonArray
	{
		using GenericArray = rapidjson::GenericArray<false, rapidjson::Value>;

	private:
		static GenericArray s_defaultArray;

	public:
        ReadJsonArray()
			: m_array(s_defaultArray), m_valid(false)
		{

		}

		ReadJsonArray(GenericArray& inArray);
		ReadJsonArray(const ReadJsonArray& jsonArray);

		ReadJsonArray& operator=(const ReadJsonArray& jsonArray);

		const bool IsValid() const { return m_valid; }
		size_t GetSize() const { return m_array.Size(); }
		bool GetElementAt(size_t index, ReadJsonValue& inValue) const;

	private:
		GenericArray& m_array;
		bool m_valid;
	};

	/// <summary>
	/// Parsed Json Value - Wrapper for the rapidjson Value.
	/// </summary>
	class ReadJsonValue
	{
	private:
		static rapidjson::Value s_defaultValue;

	public:
		ReadJsonValue();
		ReadJsonValue(rapidjson::Value& value);
		ReadJsonValue(const rapidjson::Value& value);
		ReadJsonValue(const ReadJsonValue& jsonObject);

		ReadJsonValue& operator=(const ReadJsonValue& jsonObject);

		const bool IsValid() const { return m_valid; }

		const bool IsInt() const { return IsValid() && m_value.IsInt(); }

		const bool IsInt64() const { return IsValid() && m_value.IsInt64(); }

		const bool IsUint() const { return IsValid() && m_value.IsUint(); }

		const bool IsUInt64() const { return IsValid() && m_value.IsUint64(); }

		const bool IsBool() const { return IsValid() && m_value.IsBool(); }

		const bool IsFloat() const { return IsValid() && m_value.IsFloat(); }

		const bool IsString() const { return IsValid() && m_value.IsString(); }

		const bool IsObject() const { return IsValid() && m_value.IsObject(); }

		const bool IsArray() const { return IsValid() && m_value.IsArray(); }

		bool GetUInt(uint32_t& inInt) const
		{
			if (IsUint())
			{
				inInt = m_value.GetUint();
				return true;
			}
			return false;
		}

		bool GetUInt64(uint64_t& inInt) const
		{
			if (IsUInt64())
			{
				inInt = m_value.GetUint64();
				return true;
			}
			return false;
		}

		bool GetInt(int32_t& inInt) const
		{
			if (IsInt())
			{
				inInt = m_value.GetInt();
				return true;
			}
			return false;
		}

		bool GetInt64(int64_t& inInt) const
		{
			if (IsInt64())
			{
				inInt = m_value.GetInt64();
				return true;
			}
			return false;
		}

		bool GetBool(bool& inBool) const
		{
			if (IsBool())
			{
				inBool = m_value.GetBool();
				return true;
			}
			return false;
		}

		bool GetFloat(float& inFloat) const
		{
			if (IsFloat())
			{
				inFloat = m_value.GetFloat();
				return true;
			}
			return false;
		}

		bool GetString(std::string& inString) const
		{
			if (IsString())
			{
				// TODO: See if this breaks anything, etc...
				inString = m_value.GetString();
				return true;
			}
			return false;
		}

        template<bool TIsConst = false>
		bool GetJsonObject(ReadJsonObject<TIsConst>& jsonObject) const
        {
            if (IsValid()
                && m_value.IsObject())
            {
                auto obj = m_value.GetObject();
                jsonObject = ReadJsonObject<TIsConst>(obj);
                return true;
            }
            return false;
        }
        
		bool GetJsonArray(ReadJsonArray& jsonArray) const;

	private:
		rapidjson::Value& m_value;
		bool m_valid;
	};

}
