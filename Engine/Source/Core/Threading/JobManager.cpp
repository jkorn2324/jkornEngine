#include "EnginePCH.h"
#include "JobManager.h"

#include "Worker.h"
#include "Job.h"

namespace Engine
{
	static const uint32_t MAX_NUM_WORKERS = 8;

	static bool s_initialized = false;
	static Worker* s_workers = nullptr;

	std::atomic<uint32_t> JobManager::s_numJobs = 0;
	std::queue<Job*> JobManager::s_jobs = std::queue<Job*>();
	std::mutex JobManager::s_mutex;

	void JobManager::Init()
	{
		if (s_initialized)
		{
			return;
		}
		s_workers = new Worker[MAX_NUM_WORKERS];
		s_initialized = true;

		for (uint32_t i = 0; i < MAX_NUM_WORKERS; i++)
		{
			s_workers[i].Begin();
		}
	}

	void JobManager::Release()
	{
		for (uint32_t i = 0; i < MAX_NUM_WORKERS; i++)
		{
			s_workers[i].End();
		}
		delete[] s_workers;
		s_initialized = false;
	}

	void JobManager::Wait()
	{
		if (!s_initialized) return;

		while (s_numJobs > 0)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		}
	}
	
	void JobManager::InternalAddJob(Job* job)
	{
		if (!s_initialized) return;

		s_mutex.lock();
		s_numJobs++;
		s_jobs.push(job);
		s_mutex.unlock();
	}
}