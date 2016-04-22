#pragma once
#include <atomic>
#include <mutex>
#include <condition_variable>

//commentaries:
// Consider a home computer.
// Let the number of threads doesn't exceed 256, so set type uint8_t.
// But the number of threads can be any. I chose the indexes according to the type uint8_t.

typedef uint8_t uint;

class IBarrier {
public:
	virtual auto barrier()  -> void const = 0;
	virtual auto setNumTreads(uint numThreds)  -> void const = 0;
	IBarrier(uint numThreads);
protected:
	uint numThreads;
};

class BarrierCond final : public IBarrier {
public:
	BarrierCond();
	auto setNumTreads(uint numThreds)-> void const override final;
	auto barrier() -> void const override final;

private:
	std::mutex mutex;
	std::condition_variable conditionVariable;
	enum State : bool 
	{
		Direct, Forvard
	} state;
	size_t counter;
};