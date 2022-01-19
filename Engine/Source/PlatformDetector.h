#pragma once

#ifndef PLATFORM_WINDOWS
#ifdef WINDOWS
#define PLATFORM_WINDOWS
#endif
#endif

#ifdef PLATFORM_WINDOWS

#ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#endif