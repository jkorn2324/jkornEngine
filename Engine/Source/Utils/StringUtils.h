#pragma once

#include <string>
#include <cstdlib>
#include <stdlib.h>

namespace Engine
{

	static void StringToWString(const std::string& in, std::wstring& out)
	{
		const size_t size = in.size();
		if (size <= 0) return;
		out.resize(size + 1);
		size_t converted = 0;
		mbstowcs_s(&converted, &out[0], size + 1, &in[0], _TRUNCATE);
		out.resize(size);
	}

	static void WStringToString(const std::wstring& in, std::string& out)
	{
		const size_t size = in.size();
		if (size <= 0) return;
		out.resize(size + 1);
		size_t converted = 0;
		wcstombs_s(&converted, &out[0], size + 1, &in[0], _TRUNCATE);
		out.resize(size);
	}
}