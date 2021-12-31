#pragma once

#include "PlatformDetector.h"

#ifdef PLATFORM_WINDOWS 
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <algorithm>
#include <memory>
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <string>
#include <unordered_map>
#include <chrono>

#include "DebugAssert.h"