#include "Timer.hpp"
#include "Cmd.hpp"

void Timer::Start() {
	if (state == TIMER_STOPPED) {
		start = ch::high_resolution_clock::now();
		pause_offset = ch::nanoseconds(0);
	} else if (state == TIMER_PAUSED) {
		pause_offset += ch::high_resolution_clock::now() - pause_time;	
	}

	state = TIMER_GOING;
}

void Timer::Pause() {
	pause_time = ch::high_resolution_clock::now();
}

void Timer::Stop() {
	state = TIMER_STOPPED;
}

void Timer::Reset() {
	Stop();
	Start();
}

ch::nanoseconds Timer::GetTime() {
	switch (state) {
	case TIMER_STOPPED:
		return ch::nanoseconds(0);
	case TIMER_GOING:
		return ch::high_resolution_clock::now() - start - pause_offset;
	case TIMER_PAUSED:
		return (pause_time - start) - pause_offset;
	default:
		return ch::nanoseconds(0);
	}
}

double Timer::GetSeconds() {
	return GetTime().count()/1000000000.0;
}

struct FrameLimit FrameLimit;

void FrameLimit::SetLimit(const int fps) {
	frame_duration = ch::nanoseconds(1000000000ll / fps);
}

void FrameLimit::FrameStart() {
	if (t.state != TIMER_STOPPED) {
		deltatime_n = ch::high_resolution_clock::now() - t.start;
		deltatime_n_ = deltatime_n;
		deltatime = deltatime_n.count() / 1000000000.0;		

		double timescale = GetCVarFloat("timescale");
		if (timescale < 0.0) timescale = -timescale;
		deltatime *= timescale;
		deltatime_n *= timescale;
	}

	t.Stop();
	t.Start();
}

void FrameLimit::FrameEnd() {
	computation_duration = t.GetTime();
}

void FrameLimit::Sleep() {
	auto diff = frame_duration - computation_duration;
	if (diff.count() < 0) return;
	std::this_thread::sleep_for(diff);
}

int FrameLimit::FPS() {
	if (deltatime_n_.count() == 0) return 0;
	else return 1000000000ll / deltatime_n_.count();
}
