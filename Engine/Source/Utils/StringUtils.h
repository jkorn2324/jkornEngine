#pragma once

#include <string>
#include <cstdlib>
#include <stdlib.h>

#include "PlatformString.h"

namespace Engine
{

	static void StringToWString(const std::string& in, std::wstring& out)
	{
		const size_t size = in.size();
		if (size <= 0) return;
		out.resize(size + 1);
        Platform::String::ConvertCharStrToWideCharStr(&in[0], &out[0], size + 1);
		out.resize(size);
	}

	static void WStringToString(const std::wstring& in, std::string& out)
	{
		const size_t size = in.size();
		if (size <= 0) return;
		out.resize(size + 1);
        Platform::String::ConvertWideCharStrToCharStr(&in[0], &out[0], size + 1);
		out.resize(size);
	}
}
