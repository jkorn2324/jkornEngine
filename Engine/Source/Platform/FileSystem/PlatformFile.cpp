#include "EnginePCH.h"
#include "PlatformFile.h"

#include <filesystem>

namespace Engine
{
namespace Platform::File
{
bool FOpenFile(FILE** file, const wchar_t* filePath, const wchar_t* mode)
{
#if defined(PLATFORM_MACOSX)
    // TODO: Implement MacOS for wide characters
    return false;
#elif defined(PLATFORM_WINDOWS)
    errno_t err = _wfopen_s(&filePath, fileName, mode);
    if (err != 0)
    {
        fclose(*file);
        *file = nullptr;
        return false;
    }
    return true;
#else
    return false;
#endif
}

bool FOpenFile(FILE** file, const char* filePath, const char* mode)
{
    // TODO: Implement Windows
#if defined(PLATFORM_MACOSX)
    *file = fopen(filePath, mode);
    // If its null that means that it failed.
    return *file != nullptr;
#elif defined(PLATFORM_WINDOWS)
    errno_t err = fopen_s(file, filePath, mode);
    if (err != 0)
    {
        fclose(*file);
        *file = nullptr;
        return false;
    }
    return true;
#else
    return false;
#endif
}
}
}
