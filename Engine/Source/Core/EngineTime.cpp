#include "EnginePCH.h"
#include "EngineTime.h"

namespace Engine
{
	static float s_timeScale = 1.0f;

	float Time::GetTimeScale()
	{
		return s_timeScale;
	}

	void Time::SetTimeScale(const float& timeScale)
	{
		s_timeScale = timeScale;
	}
}