#pragma once

#include <string>
#include <filesystem>

#include <stdio.h>
#include <limits.h>

#ifdef PLATFORM_WINDOWS

#include <direct.h>

#elif PLATFORM_LINUX

#include <unistd.h>

#endif


namespace Engine
{

	static std::string GetWorkingDirectory()
	{
		char cwdBuffer[_MAX_PATH];
#ifdef PLATFORM_WINDOWS
		_getcwd(cwdBuffer, _MAX_PATH);
#elif PLATFORM_LINUX
		getcwd(cwdBuffer, _MAX_PATH);
#endif
		return (std::string)cwdBuffer;
	}

	static std::wstring LGetWorkingDirectory()
	{
#ifdef PLATFORM_WINDOWS
		wchar_t cwdBuffer[_MAX_PATH];
		_wgetcwd(cwdBuffer, sizeof(wchar_t) * _MAX_PATH);
		return (std::wstring)cwdBuffer;
#else
		return L"";
#endif
	}
}