#include "barrier.h"

#pragma region IBarrier
IBarrier::IBarrier(uint numThreads): numThreads(numThreads) {}
#pragma endregion Interface Barrier

#pragma region BarrierCond
BarrierCond::BarrierCond() : IBarrier(0), counter(0), state(State::Forvard){ }

auto BarrierCond::setNumTreads(uint _numThreads) -> void const
{
	numThreads = _numThreads;
	counter = _numThreads;
}

auto BarrierCond::barrier()  -> void const
{
	std::unique_lock<std::mutex> lock{ mutex };

	if (state == State::Forvard)
	{
		if (--counter == 0) {
			state = State::Direct;
			conditionVariable.notify_all();
		}
		else {
			conditionVariable.wait(lock, [&] { return state == State::Direct; });
		}
	}

	else // State::Direct
	{
		if (++counter == numThreads) {
			state = State::Forvard;
			conditionVariable.notify_all();
		}
		else {
			conditionVariable.wait(lock, [&] { return state == State::Forvard; });
		}
	}
}
#pragma endregion Barrier with use condition variable

