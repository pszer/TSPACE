#pragma once

#include "Media.hpp"

extern struct Music {

	// returns 1 on success, 0 on error
	int Play(const std::string& name, int loops=1);

	void Stop();
	void Pause();
	void Resume();
	void Rewind();

	// volume from 0.0-1.0
	void Volume(float vol);
	float GetVolume();

} Music;
