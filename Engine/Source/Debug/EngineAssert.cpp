#include "EnginePCH.h"

#ifdef PLATFORM_WINDOWS

#include <Windows.h>

bool EngineAssertFunction(bool expression, const wchar_t* string, 
	const wchar_t* desc, int line_num, const wchar_t* file_name)
{
	bool bShouldHalt = !expression;
	if (bShouldHalt)
	{
		static wchar_t szBuffer[1024];
		_snwprintf_s(szBuffer, 1024, _TRUNCATE,
			L"Assertion Failed!\n\nDescription: %s\nExpression: %s\nFile: %s\nLine: %d\n\nPress Retry to debug.",
			desc, string, file_name, line_num);
		int msgbox = MessageBox(NULL, szBuffer, L"Assert", MB_ICONERROR | MB_ABORTRETRYIGNORE | MB_DEFBUTTON2);
		switch (msgbox)
		{
		case IDABORT:
			std::exit(1);
			break;
		case IDRETRY:
			// Do nothing which will cause it to break into the debugger
			break;
		case IDIGNORE:
			bShouldHalt = false; // If we're ignoring the assert, the show must go on!
			break;
		}
	}
	return false;
}

#endif
