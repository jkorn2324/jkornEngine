#include "EnginePCH.h"
#include "GlfwPlatformInput.h"

#include <Glfw/glfw3.h>

namespace Engine
{

	static InputKeyCode s_keyCodes[GLFW_KEY_MENU + 1];
	int s_platformKeyCodes[GLFW_KEY_MENU + 1];

	static bool s_initialized = false;


	static InputKeyCode ConvertToKeyCodeDynamic(int keyCode)
	{
		// Covers 0 - 9
		if (keyCode >= GLFW_KEY_0
			&& keyCode <= GLFW_KEY_9)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_0) + KEY_CODE_0);
		}
		// Covers A - Z
		if (keyCode >= GLFW_KEY_A
			&& keyCode <= GLFW_KEY_Z)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_A) + KEY_CODE_A);
		}

		// Covers Space - Slash
		if (keyCode >= GLFW_KEY_SPACE
			&& keyCode <= GLFW_KEY_SLASH)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_SPACE) + KEY_CODE_SPACE);
		}

		if (keyCode == GLFW_KEY_SEMICOLON
			|| keyCode == GLFW_KEY_EQUAL)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_SEMICOLON) + KEY_CODE_SEMICOLON);
		}

		if (keyCode >= GLFW_KEY_LEFT_BRACKET
			&& keyCode <= GLFW_KEY_RIGHT_BRACKET)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_LEFT_BRACKET) + KEY_CODE_LEFT_BRACKET);
		}

		if (keyCode == GLFW_KEY_GRAVE_ACCENT)
		{
			return KEY_CODE_GRAVE_ACCENT;
		}

		if (keyCode == GLFW_KEY_WORLD_1
			|| keyCode == GLFW_KEY_WORLD_2)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_WORLD_1) + KEY_CODE_WORLD_1);
		}

		if (keyCode >= GLFW_KEY_ESCAPE
			&& keyCode <= GLFW_KEY_END)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_ESCAPE) + KEY_CODE_ESCAPE);
		}

		if (keyCode >= GLFW_KEY_CAPS_LOCK
			&& keyCode <= GLFW_KEY_PAUSE)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_CAPS_LOCK) + KEY_CODE_CAPS_LOCK);
		}

		if (keyCode >= GLFW_KEY_F1
			&& keyCode <= GLFW_KEY_F25)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_F1) + KEY_CODE_F1);
		}

		if (keyCode >= GLFW_KEY_KP_0
			&& keyCode <= GLFW_KEY_KP_EQUAL)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_KP_0) + KEY_CODE_NUMPAD_0);
		}
		return (InputKeyCode)((keyCode - GLFW_KEY_LEFT_SHIFT) + KEY_CODE_LEFT_SHIFT);
	}

	static InputKeyCode ConvertToKeyCode(int keyCode)
	{
		if (keyCode == GLFW_KEY_UNKNOWN)
		{
			return KEY_CODE_UNKNOWN;
		}

		if (s_keyCodes[keyCode] != KEY_CODE_UNKNOWN)
		{
			return s_keyCodes[keyCode];
		}
		InputKeyCode code = ConvertToKeyCodeDynamic(keyCode);
		s_keyCodes[keyCode] = code;
		s_platformKeyCodes[(int)code] = keyCode;
		return code;
	}

	static int ConvertFromKeyCodeDynamic(const InputKeyCode& keyCode)
	{
		// 0 - 9
		if (keyCode >= KEY_CODE_0
			&& keyCode <= KEY_CODE_9)
		{
			return (int)(keyCode - KEY_CODE_0) + GLFW_KEY_0;
		}

		// Covers A - Z
		if (keyCode >= KEY_CODE_A
			&& keyCode <= KEY_CODE_Z)
		{
			return (int)((keyCode - KEY_CODE_A) + GLFW_KEY_A);
		}

		// Covers Space - Slash
		if (keyCode >= KEY_CODE_SPACE
			&& keyCode <= KEY_CODE_SLASH)
		{
			return (int)((keyCode - KEY_CODE_SPACE) + GLFW_KEY_SPACE);
		}

		if (keyCode == KEY_CODE_SEMICOLON
			|| keyCode == KEY_CODE_EQUAL)
		{
			return (int)((keyCode - KEY_CODE_SEMICOLON) + GLFW_KEY_SEMICOLON);
		}

		if (keyCode >= KEY_CODE_LEFT_BRACKET
			&& keyCode <= KEY_CODE_RIGHT_BRACKET)
		{
			return (int)((keyCode - KEY_CODE_LEFT_BRACKET) + GLFW_KEY_LEFT_BRACKET);
		}

		if (keyCode == KEY_CODE_GRAVE_ACCENT)
		{
			return GLFW_KEY_GRAVE_ACCENT;
		}

		if (keyCode == KEY_CODE_WORLD_1
			|| keyCode == KEY_CODE_WORLD_2)
		{
			return (int)((keyCode - KEY_CODE_WORLD_1) + GLFW_KEY_WORLD_1);
		}

		if (keyCode >= KEY_CODE_ESCAPE
			&& keyCode <= KEY_CODE_END)
		{
			return (int)((keyCode - KEY_CODE_ESCAPE) + GLFW_KEY_ESCAPE);
		}

		if (keyCode >= KEY_CODE_CAPS_LOCK
			&& keyCode <= KEY_CODE_PAUSE)
		{
			return (int)((keyCode - KEY_CODE_CAPS_LOCK) + GLFW_KEY_PAUSE);
		}

		if (keyCode >= KEY_CODE_F1
			&& keyCode <= KEY_CODE_F25)
		{
			return (int)((keyCode - KEY_CODE_F1) + GLFW_KEY_F1);
		}

		if (keyCode >= KEY_CODE_NUMPAD_0
			&& keyCode <= KEY_CODE_NUMPAD_EQUAL)
		{
			return (int)((keyCode - KEY_CODE_NUMPAD_0) + GLFW_KEY_0);
		}
		return (int)((keyCode - KEY_CODE_LEFT_SHIFT) + GLFW_KEY_LEFT_SHIFT);
	}

	static int ConvertFromKeyCode(const InputKeyCode& keyCode)
	{
		if (keyCode == KEY_CODE_UNKNOWN)
		{
			return GLFW_KEY_UNKNOWN;
		}
		if (s_platformKeyCodes[keyCode] != (int)GLFW_KEY_UNKNOWN)
		{
			return s_platformKeyCodes[keyCode];
		}
		int keyCodeDynamic = ConvertFromKeyCodeDynamic(keyCode);
		s_keyCodes[keyCodeDynamic] = keyCode;
		s_platformKeyCodes[(int)keyCode] = keyCodeDynamic;
		return keyCodeDynamic;
	}

	GlfwPlatformInput::GlfwPlatformInput()
	{
		if (!s_initialized)
		{
			size_t arraySize = sizeof(s_platformKeyCodes) / sizeof(int);
			for (size_t i = 0; i < arraySize; i++)
			{
				s_platformKeyCodes[i] = -1;
			}
			s_initialized = true;
		}
	}

	InputKeyCode GlfwPlatformInput::ToKeyCode(int platformKeyCode) const
	{
		return ConvertToKeyCode(platformKeyCode);
	}
	
	int GlfwPlatformInput::FromKeyCode(const InputKeyCode& keyCode) const
	{
		return ConvertFromKeyCode(keyCode);
	}
	
	InputMouseButton GlfwPlatformInput::ToMouseButton(int platformMouseCode) const
	{
		switch (platformMouseCode)
		{
		case GLFW_MOUSE_BUTTON_1: return MOUSE_BUTTON_LEFT;
		case GLFW_MOUSE_BUTTON_2: return MOUSE_BUTTON_RIGHT;
		case GLFW_MOUSE_BUTTON_MIDDLE: return MOUSE_BUTTON_MIDDLE;
		}
		return MOUSE_BUTTON_UNKNOWN;
	}
	
	int GlfwPlatformInput::FromMouseButton(const InputMouseButton& mouseButton) const
	{
		switch (mouseButton)
		{
		case MOUSE_BUTTON_LEFT: return GLFW_MOUSE_BUTTON_1;
		case MOUSE_BUTTON_RIGHT: return GLFW_MOUSE_BUTTON_2;
		case MOUSE_BUTTON_MIDDLE: return GLFW_MOUSE_BUTTON_MIDDLE;
		}
		return -1;
	}

	InputAction GlfwPlatformInput::ToAction(int platformInputAction) const
	{
		switch (platformInputAction)
		{
		case GLFW_PRESS: return ACTION_PRESSED;
		case GLFW_RELEASE: return ACTION_RELEASED;
		}
		return ACTION_UNKNOWN;
	}

	int GlfwPlatformInput::FromAction(const InputAction& action) const
	{
		switch (action)
		{
		case ACTION_PRESSED: return GLFW_PRESS;
		case ACTION_RELEASED: return GLFW_RELEASE;
		}
		return -1;
	}
}
