#pragma once

#include "Media.hpp"

#define CHANNELS 16

namespace Sound {
	struct SoundInstance {
		SoundInstance(Mix_Chunk* c, int ch, void (*u)(int,int)=nullptr, int r=0):
		  chunk(c), channel(ch), update(u), reg(r) { ; }

		Mix_Chunk * chunk;

		void (*update)(int ch, int r);

		int reg, channel;
		bool del = false;
	};

	extern std::vector<SoundInstance> PlayingSounds;

	void Init();

	// hook function used by SDL2_Mixer
	void ChannelFinished(int channel);

	void PlaySound(const std::string& fname, double volume=1.0, void (*update)(int,int)=nullptr, int reg=0);
	void Update();
};

