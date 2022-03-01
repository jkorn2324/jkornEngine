#include "EnginePCH.h"
#include "Worker.h"

#include "JobManager.h"
#include "Job.h"

namespace Engine
{
	Worker::Worker()
		: m_running(false)
	{
	}

	void Worker::Begin()
	{
		m_running = true;
		m_workerThread = std::thread(std::bind(&Worker::Run, this));
	}

	void Worker::End()
	{
		m_running = false;
		m_workerThread.join();
	}

	void Worker::Run()
	{
		bool isLocked;
		while (m_running)
		{
			isLocked = true;
			JobManager::s_mutex.lock();
			if (JobManager::s_numJobs > 0
				&& !JobManager::s_jobs.empty())
			{
				Job* currentJob = JobManager::s_jobs.front();
				JobManager::s_jobs.pop();
				JobManager::s_mutex.unlock();
				isLocked = false;
				currentJob->OnRun();
				JobManager::s_numJobs--;
				delete currentJob;
			}
			if (isLocked)
			{
				JobManager::s_mutex.unlock();
			}
		}
	}
}