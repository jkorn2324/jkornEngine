#pragma once


#include <rapidjson\PrettyWriter.h>
#include <rapidjson\stringbuffer.h>

#include <rapidjson\document.h>
#include <rapidjson\filereadstream.h>

#include "Source\Vector.h"
#include "Source\Quaternion.h"

#include <fstream>
#include <string>

namespace Engine
{

#pragma region write_funcs
	
	static void WriteString(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const std::string& string)
	{
		writer.String(string.c_str(), (rapidjson::SizeType)string.size());
	}


	static void WriteVector2(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const MathLib::Vector2& vector)
	{
		writer.StartObject();

		writer.String("x", 1);
		writer.Double(vector.x);

		writer.String("y", 1);
		writer.Double(vector.y);

		writer.EndObject();
	}

	static void WriteVector3(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const MathLib::Vector3& vector)
	{
		writer.StartObject();

		writer.String("x", 1);
		writer.Double(vector.x);

		writer.String("y", 1);
		writer.Double(vector.y);

		writer.String("z", 1);
		writer.Double(vector.z);

		writer.EndObject();
	}

	static void WriteVector4(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const MathLib::Vector4& vector)
	{
		writer.StartObject();

		writer.String("x", 1);
		writer.Double(vector.x);

		writer.String("y", 1);
		writer.Double(vector.y);

		writer.String("z", 1);
		writer.Double(vector.z);

		writer.String("w", 1);
		writer.Double(vector.w);

		writer.EndObject();
	}

	static void WriteQuaternion(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
		const MathLib::Quaternion& quaternion)
	{
		writer.StartObject();

		writer.String("x", 1);
		writer.Double(quaternion.x);

		writer.String("y", 1);
		writer.Double(quaternion.y);

		writer.String("z", 1);
		writer.Double(quaternion.z);

		writer.String("w", 1);
		writer.Double(quaternion.w);

		writer.EndObject();
	}

#pragma endregion

#pragma region read_funcs


	static bool ReadUInt32(rapidjson::Value& value, const char* key, uint32_t& output)
	{
		if (value.HasMember(key)
			&& value[key].IsUint())
		{
			output = value[key].GetUint();
			return true;
		}
		return false;
	}

	static bool ReadInt32(rapidjson::Value& value, const char* key, int32_t& output)
	{
		if (value.HasMember(key)
			&& value[key].IsInt())
		{
			output = value[key].GetInt();
			return true;
		}
		return false;
	}

	static bool ReadInt64(rapidjson::Value& value, const char* key, int64_t& output)
	{
		if (value.HasMember(key)
			&& value[key].IsInt64())
		{
			output = value[key].GetInt64();
			return true;
		}
		return false;
	}

	static bool ReadBool(rapidjson::Value& value, const char* key, bool& output)
	{
		if (value.HasMember(key)
			&& value[key].IsBool())
		{
			output = value[key].GetBool();
			return true;
		}
		return false;
	}

	static bool ReadFloat(rapidjson::Value& value, const char* key, float& output)
	{
		if (value.HasMember(key))
		{
			if (value[key].IsDouble())
			{
				output = (float)value[key].GetDouble();
				return true;
			}
			else if (value[key].IsFloat())
			{
				output = (float)value[key].GetFloat();
				return true;
			}
		}
		return false;
	}

	static bool ReadString(rapidjson::Value& value, const char* key, std::string& output)
	{
		if (value.HasMember(key)
			&& value[key].IsString())
		{
			output = value[key].GetString();
			return true;
		}
		return false;
	}

	static bool ReadVector2(rapidjson::Value& value, const char* key, MathLib::Vector2& vec)
	{
		if (value.HasMember(key)
			&& value[key].IsObject())
		{
			auto vecData = value[key].GetObject();
			ReadFloat(vecData, "x", vec.x);
			ReadFloat(vecData, "y", vec.y);
			return true;
		}
		return false;
	}


	static bool ReadVector3(rapidjson::Value& value, const char* key, MathLib::Vector3& vec)
	{
		if (value.HasMember(key)
			&& value[key].IsObject())
		{
			auto vecData = value[key].GetObject();
			ReadFloat(vecData, "x", vec.x);
			ReadFloat(vecData, "y", vec.y);
			ReadFloat(vecData, "z", vec.z);
			return true;
		}
		return false;
	}

	static bool ReadVector4(rapidjson::Value& value, const char* key, MathLib::Vector4& vec)
	{
		if (value.HasMember(key)
			&& value[key].IsObject())
		{
			auto vecData = value[key].GetObject();
			ReadFloat(vecData, "x", vec.x);
			ReadFloat(vecData, "y", vec.y);
			ReadFloat(vecData, "z", vec.z);
			ReadFloat(vecData, "w", vec.w);
			return true;
		}
		return false;
	}

	static bool ReadQuaternion(rapidjson::Value& value, const char* key, MathLib::Quaternion& quat)
	{
		if (value.HasMember(key)
			&& value[key].IsObject())
		{
			auto vecData = value[key].GetObject();
			ReadFloat(vecData, "x", quat.x);
			ReadFloat(vecData, "y", quat.y);
			ReadFloat(vecData, "z", quat.z);
			ReadFloat(vecData, "w", quat.w);
			return true;
		}
		return false;
	}

#pragma endregion

}