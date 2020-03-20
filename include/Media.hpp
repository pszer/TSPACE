#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <map>

#include "Log.hpp"
#include "Filesys.hpp"
#include "Font.hpp"

extern struct Media {
	const std::vector<std::string> IMG_EXTS = { ".jpg",".png",".bmp" };
	const std::vector<std::string> FNT_EXTS = { ".ttf",".png" };
	const std::vector<std::string> SFX_EXTS = { ".wav" };
	const std::vector<std::string> MUS_EXTS = { ".mp3",".ogg" };

	// f : filename
	// exts : list of acceptable extensions
	bool CheckExtension(const std::string& f, const std::vector<std::string>& exts);

	// Textures
	std::map<std::string, SDL_Texture*> textures;
	SDL_Texture * GetTexture(const std::string& str);

	// Fonts
	std::map<std::string, Font*> fonts;
	Font * GetFont(const std::string& str);

	// SFX chunks
	std::map<std::string, Mix_Chunk*> chunks;
	Mix_Chunk * GetChunk(const std::string& str);

	// Music
	std::map<std::string, Mix_Music*> music;
	Mix_Music * GetMusic(const std::string& str);

	int LoadMedia(); // returns amount of files loaded
	void FreeMedia();

	// returns amount of files loaded
	int LoadTextures(const std::vector<std::string>& files);
	int LoadFonts(const std::vector<std::string>& files);
	int LoadChunks(const std::vector<std::string>& files);
	int LoadMusic(const std::vector<std::string>& files);

	SDL_Texture * LoadTexture(const std::string& path);
	Font * LoadFont(const std::string& path);
	Mix_Chunk * LoadChunk(const std::string& path);
	Mix_Music * LoadMusicFile(const std::string& path);
} Media;
