#pragma once

#include <chrono>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#endif

// NEED C++11 for any of this to work!
namespace Common
{

#ifdef _WIN32
	// Because MSVC 2013 and below don't have a high resolution timer. So stupid.
	// http://stackoverflow.com/questions/16299029/resolution-of-stdchronohigh-resolution-clock-doesnt-correspond-to-measureme
	const long long g_Frequency = []() -> long long
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency.QuadPart;
	}();

	struct HighResClock
	{
		typedef long long                               rep;
		typedef std::nano                               period;
		typedef std::chrono::duration<rep, period>      duration;
		typedef std::chrono::time_point<HighResClock>   time_point;
		static const bool is_steady = true;

		static time_point now()
		{
			LARGE_INTEGER count;
			QueryPerformanceCounter(&count);
			return time_point(duration(count.QuadPart * static_cast<rep>(period::den) / g_Frequency));
		};
	};
#endif

	// Get the current Epoch time in microseconds
	inline uint64_t getCurrentEpochTime(void)
	{
		uint64_t etime;
#ifdef _WIN32
		HighResClock WindowsHighResClock;
		etime = std::chrono::duration_cast<std::chrono::microseconds>(WindowsHighResClock.now().time_since_epoch()).count();
#else
		etime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
#endif
		return etime;
	};

	// Sleep for milliseconds
	inline void sleep(int MilliSeconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(MilliSeconds));
	};

	// Sleep for microseconds (may be useful during profiling)
	inline void sleep_us(uint64_t MicroSeconds)
	{
		// Won't work on windows
		std::this_thread::sleep_for(std::chrono::microseconds(MicroSeconds));
	};

	const static float DEG2RAD = 0.01745329251;
	const static float RAD2DEG = 57.2957795131;
} // namespace Utilities
