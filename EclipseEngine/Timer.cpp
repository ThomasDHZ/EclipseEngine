#include "Timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::StartTimer()
{
	Start = std::chrono::high_resolution_clock::now();
}

void Timer::EndTimer()
{
	End = std::chrono::high_resolution_clock::now();
	Duration = End - Start;
}

float Timer::GetTimerDurationSeconds()
{
	return Duration.count();
}

float Timer::GetTimerDurationMilliseconds()
{
	return Duration.count() * 1000.0f;
}

