#include "DeltaTime.h"

std::chrono::duration<float> DeltaTime::s_DeltaTime;
std::chrono::time_point<DeltaTime::Clock> DeltaTime::s_Previous = DeltaTime::Clock::now();

float DeltaTime::Process() noexcept
{
	auto current = Clock::now();
	s_DeltaTime = current - s_Previous;
	s_Previous = current;
	
	return s_DeltaTime.count();
}
