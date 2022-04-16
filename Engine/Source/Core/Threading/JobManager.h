#pragma once

#include <atomic>
#include <mutex>
#include <queue>

namespace Engine
{
	class Job;

	class JobManager
	{
	public:
		template<typename TJob, typename... Args>
		static void Add(Args&&... args)
		{
			TJob* job = new TJob(std::forward<Args>(args)...);
			Job* dynamicCasted = dynamic_cast<Job*>(job);
			if (dynamicCasted != nullptr)
			{
				InternalAddJob(dynamicCasted);
				return;
			}
			delete job;
		}

		template<typename TJob>
		static void Add()
		{
			TJob* job = new TJob();
			Job* dynamicCasted = dynamic_cast<Job*>(job);
			if (dynamicCasted != nullptr)
			{
				InternalAddJob(dynamicCasted);
				return;
			}
			delete job;
		}

	private:
		static void Init();
		static void Release();
		static void Wait();

		static void InternalAddJob(Job* job);

	private:
		static std::atomic<uint32_t> s_numJobs;
		// TODO: Need to turn these into lock-free queue.
		static std::queue<Job*> s_jobs;

		friend class Application;
		friend class Worker;
	};
}