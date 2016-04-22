#include "job.h"


int main()
{
	uint8_t nThreads = std::thread::hardware_concurrency();
	//nThreads = 32;
	const uint32_t nAttempts = 5000;
	OmpJob ompJob(nThreads, nAttempts);
	//ompJob.run();
	
	nThreads = 255;
	StdJob stdJob(std::make_shared<BarrierCond>(), nThreads, nAttempts);
	stdJob.run();

	printf("barrier OpenMP: time = %3.15lf sec\n", ompJob.elapsedTime());

	printf("barrier cond: time = %3.15lf sec\n", stdJob.elapsedTime());

	system("pause");
	return EXIT_SUCCESS;
}