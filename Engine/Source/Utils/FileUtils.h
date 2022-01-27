#pragma once

#include <string>
#include <filesystem>

namespace Engine
{
	class FileUtils
	{
	public:
		static std::string GetWorkingDirectory();
		static std::wstring LGetWorkingDirectory();
		
		static bool ContainsSubdirectories(const std::string& path);
		static bool ContainsSubdirectories(const std::wstring& path);
		static bool ContainsSubdirectories(const std::filesystem::path& path);
	};
}