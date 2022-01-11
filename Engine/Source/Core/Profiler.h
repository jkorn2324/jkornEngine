#pragma once

#include <chrono>
#include <string>

namespace Engine
{
	using ProfilerClock = std::chrono::high_resolution_clock;
	using ProfilerTimePoint = ProfilerClock::time_point;

	enum ProfileTimeUnit
	{
		TIME_MILLIS,
		TIME_MILLISECONDS = TIME_MILLIS,

		TIME_SECS,
		TIME_SECONDS = TIME_SECS,

		TIME_MICROSECS,
		TIME_MICROSECONDS = TIME_MICROSECS
	};

	class ProfileTimer
	{
	public:
		ProfileTimer();
		~ProfileTimer();

		void Reset();

		void Begin();
		void End();

		double GetCurrentTime(ProfileTimeUnit unit = TIME_MILLIS) const;
		double GetAverageTime(ProfileTimeUnit unit = TIME_MILLIS) const;
		double GetHighestTime(ProfileTimeUnit unit = TIME_MILLIS) const;

	private:
		ProfilerTimePoint m_beginTime;
		double m_totalTime;
		double m_highestFrame;
		double m_prevFrame;
		uint32_t m_numFrames;
	};

	class ProfileScope
	{
	public:
		ProfileScope(const char* name,
			const char* category = "Default");
		~ProfileScope();

		const std::string& GetName() const { return m_name; }
		const std::string& GetCategory() const { return m_category; }

	private:
		std::string m_name;
		std::string m_category;
	};

	class Profiler
	{
	public:
		static void Init();
		static void Release();

		static void Reset();
		static void SetOutputFile(const std::string& file);

		static void BeginProfile(const std::string& name,
			const std::string& category);
		static void EndProfile(const std::string& name,
			const std::string& category);

	private:
		static void EmplaceToJSON(const std::string& name,
			const std::string& category,
			const ProfileTimer& timer, bool begin);

		static void BeginScope(const ProfileScope& scope);
		static void EndScope(const ProfileScope& scope);

		friend class ProfileScope;
	};

#define PROFILE_SCOPE(name, category) Engine::ProfileScope newScope_##name(#name, #category)
#define BEGIN_PROFILE(name) Engine::Profiler::BeginProfile(name)
#define END_PROFILE(name) Engine::Profiler::EndProfile(name)
}