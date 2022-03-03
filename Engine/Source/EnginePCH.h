#pragma once

#include "PlatformDetector.h"

#include <iostream>
#include <algorithm>
#include <memory>
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <string>
#include <unordered_map>
#include <chrono>
#include <cstdint>

#ifdef PLATFORM_WINDOWS

#include <Windows.h>

#ifdef GetObject
	#undef GetObject
#endif

#ifndef COINITIALIZE

#ifdef USE_WINDOWS_COINITIALIZE

#include <wrl\wrappers\corewrappers.h>
#include <combaseapi.h>

#if 0
#if _WIN32_WINNT >= 0x0A00
#define COINITIALIZE(success) \
		Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);  \
		bool success = ((HRESULT)initialize) > 0
#else
#define COINITIALIZE(success) \
		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED); \
		bool success = !FAILED(hr)
#endif
#endif

#else

#define COINITIALIZE(success) bool success = true

#endif

#endif
#else

#define COINITIALIZE(success) bool success = true

#endif

#include "DebugAssert.h"
#include "Logger.h"
#include "EngineMacros.h"