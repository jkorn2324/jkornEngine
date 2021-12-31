#pragma once

#ifdef WINDOWS

#define PLATFORM_WINDOWS

#ifdef WIN32
#define PLATFORM_WINDOWS_X86
#else
#define PLATFORM_WINDOWS_X64
#endif

#define GLFW_EXPOSE_NATIVE_WIN32

#endif