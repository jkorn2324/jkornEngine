#pragma once

#include <queue>

namespace Engine
{

	class Job;

	class Worker
	{
	public:
		Worker();

		void Begin();
		void End();

	private:
		void Run();

	private:
		std::thread m_workerThread;
		bool m_running;
	};
}