#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <string>
#include <vector>

namespace Log {

	extern const int HISTORY_SIZE;
	extern std::vector<std::string> History;

	void Add(const std::string& str);   // add to History without printing to cout

	void Log(const std::string& str);   // shows up as [LOG]: str
	void Error(const std::string& str); // shows up as [ERR]: str
	void ErrorSDL();                    // shows up as [SDL]: SDL_GetError();
	void ErrorTTF();                    // shows up as [TTF]: TTF_GetError();
	void ErrorIMG();                    // shows up as [IMG]: IMG_GetError();
	void ErrorMix();                    // shows up as [Mix]: Mix_GetError();

};
