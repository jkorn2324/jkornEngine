#pragma once

#include <cstdio>

namespace Engine
{
namespace Platform::File
{

#ifndef FPrintFile
#if defined(PLATFORM_MACOS)
#define FPrintFile(...) fprintf(__VA_ARGS__)
#elif defined(PLATFORM_WINDOWS)
#define FPrintFile(...) fprintf_s(__VA_ARGS__)
#else
#define FPrintFile(...)
#endif
#endif

bool FOpenFile(FILE** file, const wchar_t* filePath, const wchar_t* mode);
bool FOpenFile(FILE** file, const char* filePath, const char* mode);
}
}
