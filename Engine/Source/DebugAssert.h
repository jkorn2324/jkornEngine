#pragma once
#include <intrin.h>

#ifdef _DEBUG
#ifdef _WINDOWS

extern bool DbgAssertFunction(bool expression, const wchar_t* string,
	const wchar_t* desc, int line_num, const wchar_t* file_name);

#define DBG_WIDEN2(x) L##x
#define DBG_WIDEN(x) DBG_WIDEN2(x)
#define __WFILE__ DBG_WIDEN(__FILE__)

#define DebugAssert(expr, description) { if (DbgAssertFunction((expr), L#expr, L##description, __LINE__, __WFILE__)) {__debugbreak(); }
#else
#include <assert.h>
#define DebugAssert(expr, description) { assert(expr); }
#endif
#else
#define DebugAssert(expr, description)
#endif