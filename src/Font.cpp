#include "Font.hpp"

TTF_Font * Font::GetTTFSize(FONT_SIZE size) {
	return ttfs[size];
}

Font::~Font() {
	if (type == FONT_GLYPH && glyph != nullptr) {
		SDL_DestroyTexture(glyph);
	} else if (type == FONT_TTF && ttfs != nullptr) {
		for (int i = 0; i < 5; ++i)
			TTF_CloseFont(ttfs[i]);
		if (cache) {
			for (int i = 0; i < cache_count; ++i)
				cache[i].Close();
			delete[] cache;
		}
	}
}

Font_Cache::Font_Cache() { ; }

SDL_Texture * Font::CacheLookup(FONT_SIZE fsize, const std::string& str) {
	std::size_t i;
	for (i = 0; i < cache_count; ++i) {
		auto& c = cache[i];
		if (c.size == fsize && c.str == str)
			return c.tex;
	}
	return nullptr;
}

void Font::CacheInsert(FONT_SIZE fsize, const std::string& str, SDL_Texture * tex) {
	if (!tex) return;

	if (cache_count > cache_front)
		cache[cache_front].Close();
	cache[cache_front] = Font_Cache(fsize, str, tex);

	++cache_front;
	if (cache_front == Font::CACHE_SIZE)
		cache_front = 0;

	if (cache_count < Font::CACHE_SIZE) {
		++cache_count;
	}
}
