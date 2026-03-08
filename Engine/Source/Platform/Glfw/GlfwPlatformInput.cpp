#include "EnginePCH.h"
#include "GlfwPlatformInput.h"

#include <Glfw/glfw3.h>

namespace Engine
{

	static InputKeyCode s_keyCodes[GLFW_KEY_MENU + 1];
	int s_platformKeyCodes[GLFW_KEY_MENU + 1];

	static bool s_initialized = false;

	/**
	 * Taken from the ImGui Glfw Backends Itself.
	 */
	static ImGuiKey GlfwKeyToImGuiKey(int key)
	{
		switch (key)
		{
		case GLFW_KEY_TAB: return ImGuiKey_Tab;
		case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
		case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
		case GLFW_KEY_UP: return ImGuiKey_UpArrow;
		case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
		case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
		case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
		case GLFW_KEY_HOME: return ImGuiKey_Home;
		case GLFW_KEY_END: return ImGuiKey_End;
		case GLFW_KEY_INSERT: return ImGuiKey_Insert;
		case GLFW_KEY_DELETE: return ImGuiKey_Delete;
		case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
		case GLFW_KEY_SPACE: return ImGuiKey_Space;
		case GLFW_KEY_ENTER: return ImGuiKey_Enter;
		case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
		case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
		case GLFW_KEY_COMMA: return ImGuiKey_Comma;
		case GLFW_KEY_MINUS: return ImGuiKey_Minus;
		case GLFW_KEY_PERIOD: return ImGuiKey_Period;
		case GLFW_KEY_SLASH: return ImGuiKey_Slash;
		case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
		case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
		case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
		case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
		case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
		case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
		case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
		case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
		case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
		case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
		case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
		case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
		case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
		case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
		case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
		case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
		case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
		case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
		case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
		case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
		case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
		case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
		case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
		case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
		case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
		case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
		case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
		case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
		case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
		case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
		case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
		case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
		case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
		case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
		case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
		case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
		case GLFW_KEY_MENU: return ImGuiKey_Menu;
		case GLFW_KEY_0: return ImGuiKey_0;
		case GLFW_KEY_1: return ImGuiKey_1;
		case GLFW_KEY_2: return ImGuiKey_2;
		case GLFW_KEY_3: return ImGuiKey_3;
		case GLFW_KEY_4: return ImGuiKey_4;
		case GLFW_KEY_5: return ImGuiKey_5;
		case GLFW_KEY_6: return ImGuiKey_6;
		case GLFW_KEY_7: return ImGuiKey_7;
		case GLFW_KEY_8: return ImGuiKey_8;
		case GLFW_KEY_9: return ImGuiKey_9;
		case GLFW_KEY_A: return ImGuiKey_A;
		case GLFW_KEY_B: return ImGuiKey_B;
		case GLFW_KEY_C: return ImGuiKey_C;
		case GLFW_KEY_D: return ImGuiKey_D;
		case GLFW_KEY_E: return ImGuiKey_E;
		case GLFW_KEY_F: return ImGuiKey_F;
		case GLFW_KEY_G: return ImGuiKey_G;
		case GLFW_KEY_H: return ImGuiKey_H;
		case GLFW_KEY_I: return ImGuiKey_I;
		case GLFW_KEY_J: return ImGuiKey_J;
		case GLFW_KEY_K: return ImGuiKey_K;
		case GLFW_KEY_L: return ImGuiKey_L;
		case GLFW_KEY_M: return ImGuiKey_M;
		case GLFW_KEY_N: return ImGuiKey_N;
		case GLFW_KEY_O: return ImGuiKey_O;
		case GLFW_KEY_P: return ImGuiKey_P;
		case GLFW_KEY_Q: return ImGuiKey_Q;
		case GLFW_KEY_R: return ImGuiKey_R;
		case GLFW_KEY_S: return ImGuiKey_S;
		case GLFW_KEY_T: return ImGuiKey_T;
		case GLFW_KEY_U: return ImGuiKey_U;
		case GLFW_KEY_V: return ImGuiKey_V;
		case GLFW_KEY_W: return ImGuiKey_W;
		case GLFW_KEY_X: return ImGuiKey_X;
		case GLFW_KEY_Y: return ImGuiKey_Y;
		case GLFW_KEY_Z: return ImGuiKey_Z;
		case GLFW_KEY_F1: return ImGuiKey_F1;
		case GLFW_KEY_F2: return ImGuiKey_F2;
		case GLFW_KEY_F3: return ImGuiKey_F3;
		case GLFW_KEY_F4: return ImGuiKey_F4;
		case GLFW_KEY_F5: return ImGuiKey_F5;
		case GLFW_KEY_F6: return ImGuiKey_F6;
		case GLFW_KEY_F7: return ImGuiKey_F7;
		case GLFW_KEY_F8: return ImGuiKey_F8;
		case GLFW_KEY_F9: return ImGuiKey_F9;
		case GLFW_KEY_F10: return ImGuiKey_F10;
		case GLFW_KEY_F11: return ImGuiKey_F11;
		case GLFW_KEY_F12: return ImGuiKey_F12;
		case GLFW_KEY_F13: return ImGuiKey_F13;
		case GLFW_KEY_F14: return ImGuiKey_F14;
		case GLFW_KEY_F15: return ImGuiKey_F15;
		case GLFW_KEY_F16: return ImGuiKey_F16;
		case GLFW_KEY_F17: return ImGuiKey_F17;
		case GLFW_KEY_F18: return ImGuiKey_F18;
		case GLFW_KEY_F19: return ImGuiKey_F19;
		case GLFW_KEY_F20: return ImGuiKey_F20;
		case GLFW_KEY_F21: return ImGuiKey_F21;
		case GLFW_KEY_F22: return ImGuiKey_F22;
		case GLFW_KEY_F23: return ImGuiKey_F23;
		case GLFW_KEY_F24: return ImGuiKey_F24;
		default: return ImGuiKey_None;
		}
	}

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

	int GlfwPlatformInput::FromKeyCode(const InputKeyCode keyCode) const
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

	int GlfwPlatformInput::FromMouseButton(const InputMouseButton mouseButton) const
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

	int GlfwPlatformInput::FromAction(const InputAction action) const
	{
		switch (action)
		{
		case ACTION_PRESSED: return GLFW_PRESS;
		case ACTION_RELEASED: return GLFW_RELEASE;
		}
		return -1;
	}

	ImGuiKey GlfwPlatformInput::PlatformKeyToImGuiKey(int platformKeyCode) const
	{
		return GlfwKeyToImGuiKey(platformKeyCode);
	}
}
