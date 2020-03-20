#pragma once

#include <SDL2/SDL.h>

#include <map>
#include <string>

#include "Events.hpp"

#define PLAYER_LEFT  "+rotleft"
#define PLAYER_RIGHT "+rotright"

extern struct Keys {
	void DefaultBindings();

	std::map<std::string, SDL_Keycode> Bindings;

	Keypress_State GetKeyState(const std::string& str);
	double GetKeyDuration(const std::string& str);

	// returns SDLK_UNKNOWN if invalid string
	SDL_Keycode GetKeyFromString(const std::string& str);
	std::string GetStringFromKey(SDL_Keycode key);

	// returns SDLK_UNKNOWN if key is unbound
	SDL_Keycode GetBoundKey(const std::string& str);

	void SetBoundKey(const std::string& str, SDL_Keycode code);
	void UnbindBoundKey(SDL_Keycode code);

	// returns 1 for success, 0 if keyname is an invalid key
	int SetBoundKeyFromString(const std::string& str, const std::string& keyname);
	int UnbindBoundKeyFromString(const std::string& keyname);
} Keys;
