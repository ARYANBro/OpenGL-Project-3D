#pragma once

#include <chrono>

class DeltaTime
{
public:
	using Clock = std::chrono::high_resolution_clock;

public:
	static float Process() noexcept;

private:
	static std::chrono::duration<float> s_DeltaTime;
	static std::chrono::time_point<Clock> s_Previous;
};