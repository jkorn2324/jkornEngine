#pragma once

#ifdef PLATFORM_WINDOWS
#include <intrin.h>
#endif

#ifndef DEBUG
#ifdef _DEBUG
#define DEBUG
#endif
#endif

#ifdef DEBUG
#ifdef PLATFORM_WINDOWS

extern bool EngineAssertFunction(bool expression, const wchar_t* string,
	const wchar_t* desc, int line_num, const wchar_t* file_name);

#define DBG_WIDEN2(x) L##x
#define DBG_WIDEN(x) DBG_WIDEN2(x)
#define __WFILE__ DBG_WIDEN(__FILE__)

#define JKORN_ENGINE_ASSERT(expr, description) { if (EngineAssertFunction((expr), L#expr, L##description, __LINE__, __WFILE__)) { __debugbreak(); } }
#else
#include <assert.h>
#define JKORN_ENGINE_ASSERT(expr, description) { assert(expr); }
#endif
#else
#define JKORN_ENGINE_ASSERT(expr, description)
#endif
