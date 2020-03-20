#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <array>

enum FONT_TYPE { FONT_GLYPH , FONT_TTF };
enum FONT_SIZE { FONT_P10 , FONT_P12 , FONT_P16, FONT_P32 , FONT_P48 };

struct Font_Cache {
	Font_Cache();
	Font_Cache(FONT_SIZE _size, const std::string& _str, SDL_Texture * _tex):
	  size(_size), str(_str), tex(_tex) { ; }

	FONT_SIZE size = FONT_P10;
	std::string str = "";
	SDL_Texture * tex = nullptr;

	void Close() {
		if (tex)
			SDL_DestroyTexture(tex);
	}
};

struct Font {
	Font(TTF_Font ** _ttfs)
	  : type(FONT_TTF), ttfs(_ttfs), cache(new Font_Cache[CACHE_SIZE]) {}
	Font(SDL_Texture * _glyph)
	  : type(FONT_GLYPH), glyph(_glyph) {}

	const FONT_TYPE type;
	union {
		TTF_Font ** ttfs; // array[5] of TTF_Font's for different font sizes
		SDL_Texture * glyph;
	};

	TTF_Font * GetTTFSize(FONT_SIZE size);

	const static int CACHE_SIZE = 128;
	Font_Cache * cache = nullptr; // gets allocated in constructor Font(TTF_Font ** _ttfs)
	std::size_t cache_count=0, cache_front=0;
	SDL_Texture * CacheLookup(FONT_SIZE fsize, const std::string& str);
	void CacheInsert(FONT_SIZE fsize, const std::string& str, SDL_Texture * tex);

	~Font();
};
