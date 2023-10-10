#pragma once

#include <cstdlib>

namespace Engine
{
namespace Platform::String
{
// Converts a Wide Char String to a Char String.
bool ConvertWideCharStrToCharStr(const wchar_t* src, char* dst, size_t dstLen);
// Converts a char string to a wide char string.
bool ConvertCharStrToWideCharStr(const char* src, wchar_t* dst, size_t dstLen);
}
}
