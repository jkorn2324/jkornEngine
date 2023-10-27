#include "EnginePCH.h"
#include "PlatformString.h"

namespace Engine
{
namespace Platform::String
{

bool ConvertWideCharStrToCharStr(const wchar_t* src, char* dst, size_t dstLen)
{
#if defined(PLATFORM_MACOS)
    // TODO: Implementation
    return false;
#elif defined(PLATFORM_WINDOWS)
    size_t converted = 0;
    errno_t err = wcstombs_s(&converted, dst, dstLen, src, _TRUNCATE);
    return err != 0;
#else
    return false;
#endif
}

bool ConvertCharStrToWideCharStr(const char* src, wchar_t* dst, size_t dstLen)
{
#if defined(PLATFORM_MACOS)
    // TODO: Implmentation
    return false;
#elif defined(PLATFORM_WINDOWS)
    size_t converted = 0;
    errno_t err = mbstowcs_s(&converted, dst, dstLen, src, _TRUNCATE);
    return err != 0;
#else
    return false;
#endif
}

}
}
