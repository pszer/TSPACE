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
	if (state == TIMER_GOING) {
		pause_time = ch::high_resolution_clock::now();
		state = TIMER_PAUSED;
	}
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

std::string Timer::Format(const std::string& str) {
	auto pad = [](std::string s, int count) -> std::string {
		int c = count - s.length();
		if (c <= 0) return s;
		std::string pad = "";
		for (int i = 0; i < c; ++i) pad += '0';
		return pad + s;
	};

	long long ns = GetTime().count();

	int h = ns / 3600000000000;
	int m = (ns / 60000000000) % 60;
	int s = (ns / 1000000000) % 60;
	int d = (ns / 10000000) % 100;
	int D = (ns / 1000000) % 1000;

	std::string result = "";

	bool escape = false;
	for (auto& c : str) {
		if (escape) {
			switch (c) {
			case 'h': result += pad(std::to_string(h), 2); break;
			case 'm': result += pad(std::to_string(m), 2); break;
			case 's': result += pad(std::to_string(s), 2); break;
			case 'd': result += pad(std::to_string(d), 2); break;
			case 'D': result += pad(std::to_string(D), 3); break;
			case '%': result += '%'; break;
			default:  result += "<format error>" ; break;
			}

			escape = false;
			continue;
		}

		if (c == '%')
			escape = true;
		else
			result += c;
	}

	return result;
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
