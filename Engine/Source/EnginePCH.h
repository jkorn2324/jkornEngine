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

#endif

#include "DebugAssert.h"
#include "Logger.h"