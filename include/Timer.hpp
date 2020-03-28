#pragma once

#include <SDL2/SDL.h>

#include <chrono>
#include <thread>

enum Timer_State { TIMER_STOPPED , TIMER_GOING , TIMER_PAUSED };

namespace ch = std::chrono;

struct Timer {
	Timer() { }

	Timer_State state = TIMER_STOPPED;
	ch::time_point<ch::high_resolution_clock> start, pause_time;
	ch::nanoseconds pause_offset = ch::nanoseconds(0);

	void Reset();
	void Start();
	void Pause();
	void Stop();

	ch::nanoseconds GetTime();
	double GetSeconds();

	// %h is hours
	// %m is minutes
	// %s is seconds
	// %d is milliseconds (2 places)
	// %D is milliseconds (3 places)
	// %% is a percent sign
	std::string Format(const std::string& str);
};

extern struct FrameLimit {
	void SetLimit(const int fps);
	ch::nanoseconds frame_duration = ch::nanoseconds(0);

	void FrameStart();
	void FrameEnd();
	void Sleep();

	int FPS();

	ch::nanoseconds deltatime_n = ch::nanoseconds(0);
	double deltatime = 0.0; // deltatime in seconds
private:
	ch::nanoseconds computation_duration;
       
	// deltatime not affected by timescale for FPS calculation
	ch::nanoseconds deltatime_n_ = ch::nanoseconds(0);
	Timer t;
} FrameLimit;
