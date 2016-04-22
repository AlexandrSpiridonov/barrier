#pragma once
#include "job.h"
#include <omp.h>
#include <cstdlib>

IJob::IJob(uint numThread, uint64_t numTask) : numThread(numThread), numTask(numTask) {}

auto IJob::run() -> void const
{
	start = std::chrono::system_clock::now();
	run_intern();
	end = std::chrono::system_clock::now();
}

auto IJob::elapsedTime() -> double const
{
	return (std::chrono::duration<double>(end - start)).count();
}


OmpJob::OmpJob(uint numThread, uint64_t numTask): IJob(numThread, numTask) {  }


auto OmpJob::run_intern() -> void const
{
#pragma omp parallel num_threads((int)numThread)
	{
		auto threadId = omp_get_thread_num();
		for (auto i = 0; i< numTask; ++i) {
			if ((i % 100) == 0)
				printf("%d ", i);
#pragma omp barrier
			if (threadId == 0 && (i % 100) == 0)
				printf("\n");
#pragma omp barrier
		}
	}
}

StdJob::StdJob(std::shared_ptr<IBarrier> barrier, uint numThread, uint64_t numTask): barrier(barrier), IJob(numThread, numTask) {
	
	barrier->setNumTreads(numThread);
}


auto StdJob::work(uint threadId) -> void const
{
	for (auto i = 0; i < numTask; ++i) {
		if ((i % 100) == 0)
			printf("%u ", i);
		barrier->barrier();

		if (threadId == 0 && (i % 100) == 0)
			printf("\n");

		barrier->barrier();
	}
}

auto StdJob::work2(uint threadId, std::shared_ptr<IBarrier> barrier) -> void const
{
	for (auto i = 0; i < numTask; ++i) {
		if ((i % 100) == 0)
			printf("%u ", i);
		barrier->barrier();

		if (threadId == 0 && (i % 100) == 0)
			printf("\n");

		barrier->barrier();
	}
}


auto StdJob::run_intern() -> void const
{
	std::vector<std::thread> workers;
	workers.reserve(numThread); //reserve memory, capacity == nThreads
	for (auto i = 0; i < numThread; ++i)
		workers.push_back(std::thread(&StdJob::work, this, i));
		//workers.push_back(std::thread(&StdJob::work2, this, i, std::ref(barrier)));
		
	for(auto &worker: workers)
		worker.join();
}


