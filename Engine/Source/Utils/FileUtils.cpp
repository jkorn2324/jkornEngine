#include "EnginePCH.h"
#include "FileUtils.h"

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

	std::string FileUtils::GetWorkingDirectory()
	{
			char* cwdBuffer;
#if PLATFORM_WINDOWS
			cwdBuffer = _getcwd(nullptr, _MAX_PATH);
			if (cwdBuffer == nullptr) return "";
			std::string outBuffer(cwdBuffer);
			delete cwdBuffer;
			return outBuffer;
#elif PLATFORM_LINUX
			getcwd(cwdBuffer, _MAX_PATH);
#endif
			return "";
	}

	std::wstring FileUtils::LGetWorkingDirectory()
	{
#ifdef PLATFORM_WINDOWS
			wchar_t cwdBuffer[_MAX_PATH];
			_wgetcwd(cwdBuffer, sizeof(wchar_t) * _MAX_PATH);
			return (std::wstring)cwdBuffer;
#else
			return L"";
#endif
	}

	bool FileUtils::ContainsSubdirectories(const std::string& path)
	{
		std::filesystem::path p(path);
		return ContainsSubdirectories(p);
	}

	bool FileUtils::ContainsSubdirectories(const std::wstring& path)
	{
		std::filesystem::path p(path);
		return ContainsSubdirectories(p);
	}

	bool FileUtils::ContainsSubdirectories(const std::filesystem::path& path)
	{
		if (!std::filesystem::is_directory(path))
		{
			return false;
		}
		auto directoryIterator = std::filesystem::directory_iterator(path);
		for (const auto& v : directoryIterator)
		{
			if (std::filesystem::is_directory(v.path()))
			{
				return true;
			}
		}
		return false;
	}
}