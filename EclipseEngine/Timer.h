#pragma once
#include <chrono>

class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> Start;
	std::chrono::time_point<std::chrono::steady_clock> End;
	std::chrono::duration<float> Duration;
public:

	Timer();
	~Timer();

	void StartTimer();
	void EndTimer();
	float GetTimerDurationSeconds();
	float GetTimerDurationMilliseconds();
};

