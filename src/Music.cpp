#include "Music.hpp"

struct Music Music;

int Music::Play(const std::string& name, int loops) {
	Log::Log("Playing song \"" + name + "\" (" + std::to_string(loops) + " loops)");

	auto mus = Media.GetMusic(name);
	if (mus == nullptr) {
		Log::Error("Music file \"" + name + "\" not found");
		return 0;
	}

	Mix_PlayMusic(mus, loops);
	return 1;
}

void Music::Stop() {
	Mix_HaltMusic();
}

void Music::Pause() {
	Mix_PauseMusic();
}

void Music::Resume() {
	Mix_ResumeMusic();
}

void Music::Rewind() {
	Mix_RewindMusic();
}

void Music::Volume(float vol) {
	int volume = static_cast<int>(vol * MIX_MAX_VOLUME);
	if (volume < 0) volume = 0;
	Mix_VolumeMusic(volume);
}

float Music::GetVolume() {
	return Mix_VolumeMusic(-1);
}
