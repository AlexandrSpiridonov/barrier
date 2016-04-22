#pragma once
#include "barrier.h"
#include <chrono>
#include <vector>

class IJob {
public:
	/**
	* Run tasks
	*/
	auto run() -> void const;
	/**
	* Elapsed time at seconds
	*/
	auto elapsedTime() -> double const;

protected:
	IJob(uint numThread, uint64_t numTask);
	virtual auto run_intern()->void const = 0;
	uint numThread;
	uint64_t numTask;
	std::chrono::time_point<std::chrono::system_clock> start, end;

private:
	explicit IJob() = delete;
	explicit IJob(const IJob &) = delete;
	explicit IJob(IJob &&) = delete;
	auto operator =(const IJob& )->IJob& = delete;
	auto operator =(IJob&& )->IJob& = delete;
};

class OmpJob final : public IJob
{
public:
	OmpJob(uint numThread, uint64_t numTask);

protected:
	auto run_intern()->void const override final;

private:
	explicit OmpJob() = delete;
	explicit OmpJob(const OmpJob &) = delete;
	explicit OmpJob(OmpJob &&) = delete;
	auto operator =(const OmpJob&)->OmpJob& = delete;
	auto operator =(OmpJob&&)->OmpJob& = delete;
};

class StdJob final : public IJob
{
public:
	StdJob(std::shared_ptr<IBarrier> barrier, uint numThread, uint64_t numTask);
	auto work(uint threadId) -> void const;
	auto work2(uint threadId, std::shared_ptr<IBarrier> barrier) -> void const;

protected:
	auto run_intern()->void const override final;

private:
	explicit StdJob() = delete;
	explicit StdJob(const StdJob &) = delete;
	explicit StdJob(StdJob &&) = delete;
	auto operator =(const StdJob&)->StdJob& = delete;
	auto operator =(StdJob&&)->StdJob& = delete;

	std::shared_ptr<IBarrier> barrier;
};