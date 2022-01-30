#pragma once

namespace Engine
{

	class Time
	{
	public:
		static float GetTimeScale();
		static void SetTimeScale(const float& timeScale);
	};

	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_time(time), m_timeScale(1.0f)
		{

		}

		Timestep(float time, float timeScale)
			: m_time(time), m_timeScale(timeScale) { }

		operator float() const { return m_time * m_timeScale; }

		float GetSeconds() const { return GetRawSeconds() * m_timeScale; }
		float GetMilliseconds() const { return GetRawMilliseconds() * m_timeScale; }

		float GetRawSeconds() const { return m_time; }
		float GetRawMilliseconds() const { return m_time * 1000.0f; }

	private:
		float m_time;
		float m_timeScale;
	};
}