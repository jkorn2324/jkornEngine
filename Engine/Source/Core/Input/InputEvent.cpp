#include "EnginePCH.h"
#include "InputEvent.h"

#include "Input.h"

namespace Engine
{

	InputMouseMoveEvent::InputMouseMoveEvent(const MathLib::Vector2& mousePos)
		: mousePos(mousePos), prevMousePos(Input::GetMouseScreenPos()) { }
}