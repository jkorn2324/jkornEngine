#include "EnginePCH.h"
#include "Profiler.h"

#include "Logger.h"

#include <stdio.h>
#include <unordered_map>

#include <rapidjson\prettywriter.h>
#include <rapidjson\stringbuffer.h>

#include <thread>

namespace Engine
{

	static uint64_t GetNowInMicroSeconds()
	{
		return std::chrono::high_resolution_clock::now().time_since_epoch().count() / 1000;
	}

	static double Convert(double t, ProfileTimeUnit unit)
	{
		switch (unit)
		{
		case TIME_MICROSECONDS: return t * 1000.0;
		case TIME_MILLIS: return t;
		case TIME_SECS: return t / 1000.0;
		}
		return t;
	}

	// Profile Timer Data Structure.

	ProfileTimer::ProfileTimer()
		: m_numFrames(0),
		m_highestFrame(0.0), 
		m_totalTime(0.0),
		m_prevFrame(0.0),
		m_beginTime()
	{
	}

	ProfileTimer::~ProfileTimer()
	{
	}

	void ProfileTimer::Reset()
	{
		m_highestFrame = 0.0;
		m_totalTime = 0.0;
		m_numFrames = 0;
		m_prevFrame = 0.0;
	}

	void ProfileTimer::Begin()
	{
		m_beginTime = ProfilerClock::now();
	}

	void ProfileTimer::End()
	{
		std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(
			ProfilerClock::now() - m_beginTime);
		m_prevFrame = (double)duration.count() / 1000.0;
		m_totalTime += m_prevFrame;
		m_numFrames++;
		if (m_prevFrame > m_highestFrame)
		{
			m_highestFrame = m_prevFrame;
		}
	}

	double ProfileTimer::GetCurrentDuration(ProfileTimeUnit time) const
	{
		return Convert(m_prevFrame / (double)m_numFrames, time);
	}

	double ProfileTimer::GetAverageDuration(ProfileTimeUnit time) const
	{
		return Convert(m_totalTime / (double)m_numFrames, time);
	}
	
	double ProfileTimer::GetHighestDuration(ProfileTimeUnit time) const
	{
		return Convert(m_highestFrame, time);
	}

	// Profile Scope Data Structure

	ProfileScope::ProfileScope(const char* name,
		const char* category)
		: m_name(name),
		m_category(category)
	{
		Profiler::BeginScope(*this);
	}

	ProfileScope::~ProfileScope()
	{
		Profiler::EndScope(*this);
	}


	// Profiler Implementation

	static std::unordered_map<std::string, ProfileTimer*> s_timers;
	static bool s_profilerInitialized = false;

	static std::string s_outputFile("Profile.json");

	static rapidjson::StringBuffer s_jsonStringBuffer;
	static rapidjson::PrettyWriter<rapidjson::StringBuffer> s_jsonStringWriter(s_jsonStringBuffer);
	
	void Profiler::Init()
	{
        JKORN_ENGINE_ASSERT(!s_profilerInitialized,
			"Profiler has already been initialized.");
		s_jsonStringWriter.StartArray();
		s_profilerInitialized = true;
	}
	
	void Profiler::Release()
	{
		s_jsonStringWriter.EndArray();

		// Writes the json output to the file.
		FILE* outputFile;
		fopen_s(&outputFile, s_outputFile.c_str(), "w");
        JKORN_ENGINE_ASSERT(outputFile, "File failed to open/write.");
		fprintf_s(outputFile, "%s", s_jsonStringBuffer.GetString());
		fclose(outputFile);

		for (const auto& timer : s_timers)
		{
			delete timer.second;
		}
		s_timers.clear();
		s_profilerInitialized = false;
	}

	void Profiler::Reset()
	{
		for (const auto& timer : s_timers)
		{
			timer.second->Reset();
		}
	}

	void Profiler::SetOutputFile(const std::string& file)
	{
		s_outputFile = file;
	}

	void Profiler::BeginProfile(const std::string& name,
		const std::string& category)
	{
#ifdef DEBUG
		const auto& found = s_timers.find(name);
		if (found != s_timers.end())
		{
			ProfileTimer* timer = found->second;
			timer->Begin();
			EmplaceToJSON(name, category,
				*timer, true);
			return;
		}
		ProfileTimer* timer = new ProfileTimer();
		timer->Begin();
		s_timers.emplace(name, timer);
		EmplaceToJSON(name, category, *timer, true);
#endif
	}

	void Profiler::EndProfile(const std::string& name,
		const std::string& category)
	{
#ifdef DEBUG
		const auto& found = s_timers.find(name);
		if (found != s_timers.end())
		{
			ProfileTimer* timer = found->second;
			timer->End();
			EmplaceToJSON(name, category,
				*timer, false);
		}
#endif
	}
	
	void Profiler::EmplaceToJSON(const std::string& name, const std::string& category,
		const ProfileTimer& timer, bool begin)
	{
		s_jsonStringWriter.StartObject();
		
		if (begin)
		{
			s_jsonStringWriter.String("name", 4);
			s_jsonStringWriter.String(name.c_str(), (rapidjson::SizeType)name.size());

			s_jsonStringWriter.String("cat", 3);
			s_jsonStringWriter.String(category.c_str(), (rapidjson::SizeType)category.size());
		}

		s_jsonStringWriter.String("ph", 2);
		s_jsonStringWriter.String(begin ? "B" : "E", 1);

		s_jsonStringWriter.String("ts", 2);
		s_jsonStringWriter.Uint64(GetNowInMicroSeconds());

		s_jsonStringWriter.String("pid", 3);
		s_jsonStringWriter.Int(1);

		s_jsonStringWriter.String("tid", 3);
		s_jsonStringWriter.Uint64(std::hash<std::thread::id>{}(
			std::this_thread::get_id()));

		s_jsonStringWriter.EndObject();
	}

	void Profiler::BeginScope(const ProfileScope& scope)
	{
		BeginProfile(scope.GetName(), scope.GetCategory());
	}

	void Profiler::EndScope(const ProfileScope& scope)
	{
		EndProfile(scope.GetName(), scope.GetCategory());
	}
}
