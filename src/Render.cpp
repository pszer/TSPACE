#include "Core.hpp"
#include "Render.hpp"
#include "Events.hpp"

struct Renderer Renderer;

void Renderer::SetColor(const SDL_Color& c) {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}
void Renderer::SetColor(const int r, const int g, const int b, const int a) {
	SDL_SetRenderDrawColor(renderer, r,g,b,a);
}

void Renderer::Clear(const SDL_Color c) {
	SetColor(c);
	SDL_RenderClear(renderer);
}

void Renderer::Update() {
	SDL_RenderPresent(renderer);
}

double hue2rgb(double p, double q, double t) {
	if(t < 0.0) t += 1.0;
	if(t > 1.0) t -= 1.0;
	if(t < 1.0/6.0) return p + (q - p) * 6 * t;
	if(t < 1.0/2.0) return q;
	if(t < 2.0/3.0) return p + (q - p) * (2.0/3.0 - t) * 6.0;
	return p;
}

SDL_Color Renderer::HSLToRGB(double h, double s, double l){
	double r, g, b;

	if (s == 0.0) {
		r = l;
		g = l;
		b = l;
	} else {
		double q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
		double p = 2.0 * l - q;
		r = hue2rgb(p, q, h + 1.0/3.0);
		g = hue2rgb(p, q, h);
		b = hue2rgb(p, q, h - 1.0/3.0);
	}

	return {(unsigned char)(r*255.0),(unsigned char)(g*255.0),(unsigned char)(b*255.0),
	  0xff};
}

void Renderer::RenderPixel(Vec2 p) {
	SDL_RenderDrawPoint(renderer, p.x, p.y);
}
void Renderer::RenderPixel(Vec2 p, const SDL_Color& c) {
	SetColor(c);
	RenderPixel(p);
}

void Renderer::RenderLine(Vec2 a, Vec2 b) {
	SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
}
void Renderer::RenderLine(Vec2 a, Vec2 b, const SDL_Color& c) {
	SetColor(c);
	RenderLine(a, b);
}

void Renderer::RenderLine(Line line) {
	RenderLine(line.a, line.b);
}

void Renderer::RenderLine(Line line, const SDL_Color& c){
	RenderLine(line.a, line.b, c);
}

void Renderer::RenderRect(Rect rect) {
	SDL_Rect sdlrect = rect.ToSDLRect();
	SDL_RenderDrawRect(renderer, &sdlrect);
}
void Renderer::RenderRect(Rect rect, const SDL_Color& c) {
	SetColor(c);
	RenderRect(rect);
}

void Renderer::RenderFillRect(Rect rect) {
	SDL_Rect sdlrect = rect.ToSDLRect();
	SDL_RenderFillRect(renderer, &sdlrect);
}
void Renderer::RenderFillRect(Rect rect, const SDL_Color& c) {
	SetColor(c);
	RenderFillRect(rect);
}

void Renderer::RenderTexture(const std::string& img_name, Rect* src, Rect* dest) {
	auto texture = Media.GetTexture(img_name);
	if (texture == nullptr) return;

	SDL_Rect sdlsrc, sdldest;

	if (src)  sdlsrc  = src->ToSDLRect();
	if (dest) sdldest = dest->ToSDLRect();

	SDL_RenderCopy(renderer, texture, src ? &sdlsrc : nullptr, dest ? &sdldest : nullptr);
}

void Renderer::RenderTexture(const std::string& img_name, Rect* src, Rect* dest,
  double angle, SDL_RendererFlip flip, SDL_Point * rot_centre)
{
	auto texture = Media.GetTexture(img_name);
	if (texture == nullptr) return;

	SDL_Rect sdlsrc, sdldest;

	if (src)  sdlsrc  = src->ToSDLRect();
	if (dest) sdldest = dest->ToSDLRect();

	SDL_RenderCopyEx(renderer, texture, src ? &sdlsrc : nullptr, dest ? &sdldest : nullptr,
	  angle, rot_centre, flip);
}

void Renderer::RenderTexture(const std::string& img_name, SDL_Rect* src, SDL_Rect* dest) {
	auto texture = Media.GetTexture(img_name);
	if (texture == nullptr) return;

	SDL_RenderCopy(renderer, texture, src, dest);
}

void Renderer::RenderTiledTexture(const std::string& tex_name, Rect _rect, Vec2 scale, Vec2 offset) {
	int win_w, win_h;

	auto texture = Media.GetTexture(tex_name);
	if (texture == nullptr) return;

	Rect rect = _rect.Absolute();

	int tex_w, tex_h;
	SDL_QueryTexture(texture, nullptr, nullptr, &tex_w, &tex_h);

	// scale cant be 0
	if (scale.x == 0.0 || scale.y == 0.0) return;

	// make scale absolute but flip texture as appropiate
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (scale.x < 0.0) {
		flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
		scale.x *= -1.0;
	}
	if (scale.y < 0.0) {
		flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
		scale.y *= -1.0;
	}

	double tile_w = tex_w * scale.x,
	       tile_h = tex_h * scale.y;

	offset.x -= rect.x;
	offset.y -= rect.y;

	// wrap offset around and make negative
	offset.x = std::fmod(offset.x, tile_w);
	if (offset.x != 0.0)
		offset.x -= tile_w;

	offset.y = std::fmod(offset.y, tile_h);
	if (offset.y != 0.0)
		offset.y -= tile_h;

	SDL_Rect viewport = rect.ToSDLRect();
	if (viewport.y + viewport.h > win_h)
		viewport.h = viewport.y + viewport.h - win_h;
	if (viewport.x + viewport.w > win_w)
		viewport.w = viewport.x + viewport.w - win_w;
	SDL_RenderSetViewport(renderer, &viewport);

	Rect draw_rect = { 0, 0, (tile_w + 0.5), (tile_h + 0.5) };

	for (double x = offset.x; x < rect.w; x += tile_w) {
		for (double y = offset.y; y < rect.h; y += tile_h) {
			draw_rect.x = x;
			draw_rect.y = y;

			SDL_Rect r = draw_rect.ToSDLRect();
			if (std::fmod(draw_rect.x,1.0) && x != offset.x) r.w+=1;
			if (std::fmod(draw_rect.y,1.0) && y != offset.y) r.h+=1;

			SDL_Rect src = {0,0,tex_w, tex_h};
			if (x < 0.0) {
				src.x = -x;
				src.w += x;
				r.x = 0;
				r.w += x;
			}
			if (y < 0.0) {
				src.y = -y;
				src.h += y;
				r.y = 0;
				r.h += y;
			}

			SDL_RenderCopyEx(renderer, texture, &src, &r,
			  0.0, nullptr, flip);
		}
	}

	SDL_RenderSetViewport(renderer, nullptr);
}

#include <iostream>
SDL_Rect Renderer::RenderText(const std::string& font_name, const std::string& text, int x, int y,
  FONT_SIZE size, SDL_Color c, TEXT_ALIGN align)
{
	auto font = Media.GetFont(font_name);
	if (font == nullptr) return {0,0,0,0};

	if (font->type == FONT_TTF) {
		SDL_Texture * t;

		SDL_Texture * lookup = font->CacheLookup(size, text);
		if (lookup) {
			t = lookup;
		} else {
			SDL_Surface * surface = TTF_RenderUTF8_Blended(font->GetTTFSize(size),
				text.c_str(), {0xff,0xff,0xff,0xff});
			t = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
		}

		SDL_SetTextureColorMod(t, c.r, c.g, c.b);
		SDL_SetTextureAlphaMod(t, c.a);

		int w,h;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);

		SDL_Rect r = Rect(x,y,w,h).ToSDLRect();

		if (align == ALIGN_RIGHT)
			r.x -= w;
		else if (align == ALIGN_MIDDLE)
			r.x -= w/2;

		SDL_RenderCopy(renderer, t, nullptr, &r);

		if (!lookup) {
			font->CacheInsert(size, text, t);
		}

		return r;
	} else {
		SDL_SetTextureColorMod(font->glyph, c.r, c.g, c.b);

		int w,h;
		SDL_QueryTexture(font->glyph, NULL, NULL, &w, &h);

		Rect r = Rect(x,y,h,h);

		for (char c : text) {
			SDL_Rect src  = { h*c, 0, h, h };
			SDL_Rect dest = r.ToSDLRect();

			SDL_RenderCopy(renderer, font->glyph, &src, &dest);

			r.x += r.w;
		}

		return {0,0,0,0};
	}
}

void Renderer::RenderText(TTF_Font * font, const std::string& text, int x, int y, SDL_Color c) {
	SDL_Surface * surface = TTF_RenderUTF8_Blended(font, text.c_str(), c);
	SDL_Texture * t = SDL_CreateTextureFromSurface(renderer, surface);

	int w,h;
	TTF_SizeText(font, text.c_str(), &w, &h);

	SDL_Rect r = {x,y,w,h};
	SDL_RenderCopy(renderer, t, nullptr, &r);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(t);

}
