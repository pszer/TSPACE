#include "Tetris.hpp"
#include "Core.hpp"


Ruleset PLACEHOLDER_RULESET = Ruleset("PLACEHOLDER");
ClassicTetris CLASSIC_TETRIS = ClassicTetris();

#define m0 Mino(0,0)
#define m1 Mino(1,1)
#define m2 Mino(1,2)
#define m3 Mino(1,3)
#define m4 Mino(1,4)
#define m5 Mino(1,5)
#define m6 Mino(1,6)
#define m7 Mino(1,7)
const std::vector<Shape> STANDARD_POOL = {
// I
Shape(2,4,{m0,m1,
           m0,m1,
           m0,m1,
           m0,m1}),
// T
Shape(3,3,{m0,m2,m0,
           m2,m2,m2,
		   m0,m0,m0}),
// O
Shape(2,2,{m3,m3,
           m3,m3}),
// L	  
Shape(2,3,{m4,m0,
           m4,m0,
           m4,m4}),
// J	  
Shape(2,3,{m0,m5,
           m0,m5,
           m5,m5}),
// S
Shape(3,3,{m0,m0,m0,
           m0,m6,m6,
		   m6,m6,m0}),
// Z
Shape(3,3,{m0,m0,m0,
           m7,m7,m0,
		   m0,m7,m7})
};
#undef m0
#undef m1
#undef m2
#undef m3
#undef m4
#undef m5
#undef m6
#undef m7

void Ruleset::TetrionViewDetails(SDL_Rect& viewport, SDL_Rect& mino) {
	float scale = Core.GetWindowScale();

	SDL_Rect r = { 0 , 0 , (int)(tetrion_view_w * mino_w * scale) ,
	                       (int)(tetrion_view_h * mino_h * scale)};
	r.x = (int)(Event.win_w * tetrion_align_x);
	r.y = (int)(Event.win_h * tetrion_align_y);

	switch (tetrion_align_horizontal) {
	case TETRION_ALIGN_LEFT:
		break;
	case TETRION_ALIGN_RIGHT:
		r.x -= r.w; break;
	case TETRION_ALIGN_MIDDLE:
		r.x -= r.w/2; break;
	default: break;
	}

	switch (tetrion_align_vertical) {
	case TETRION_ALIGN_TOP:
		break;
	case TETRION_ALIGN_BOTTOM:
		r.y -= r.h; break;
	case TETRION_ALIGN_MIDDLE:
		r.y -= r.h/2; break;
	default: break;
	}

	if (r.x < tetrion_pad_x) r.x = tetrion_pad_x;
	if (r.y < tetrion_pad_y) r.y = tetrion_pad_y;

	viewport = r;
	mino = { 0 , 0 , (int)(mino_w * scale) , (int)(mino_h * scale) };
}

Mino Ruleset::TetrionGet(int8_t x, int8_t y) {
	return tetrion.Get(x,y);
}

bool Ruleset::TetrionGetSolid(int8_t x, int8_t y) {
	return TetrionGet(x, y).solid;
}

RULESET_STATE Ruleset::Update(double dt, RULESET_SIGNAL sig) {
	return RULESET_GOING;
}

std::string Ruleset::GetTypeTexture(uint8_t type) {
	std::string skin = GetCVarString("skin");
	if (skin.empty()) return "";
	std::string tex = skin + "/" + GetCVarString("t" + std::to_string(type));
	return tex;
}

void Ruleset::DrawTetrion() {
	SDL_Rect viewport;
	SDL_Rect mino_r;
	TetrionViewDetails(viewport, mino_r);

	std::string bg_tex  = GetTypeTexture(0);

	SDL_RenderSetViewport(Renderer.renderer, &viewport);
	for (int x = tetrion_view_x; x < tetrion_view_x + tetrion_view_w; ++x) {
		for (int y = tetrion_view_y; y < tetrion_view_y + tetrion_view_h; ++y) {
			mino_r.x = (x - tetrion_view_x) * mino_r.w;
			mino_r.y = (y - tetrion_view_y) * mino_r.h;
			Renderer.RenderTexture(bg_tex  , nullptr, &mino_r);

			Mino m = TetrionGet(x, y);
			if (m.type == 0) continue;

			std::string mino_tex = GetTypeTexture(m.type);

			if (mino_tex.empty()) continue;

			Renderer.RenderTexture(mino_tex, nullptr, &mino_r);
		}
	}
	SDL_RenderSetViewport(Renderer.renderer, nullptr);
}

void Ruleset::ActivateKeyBindings() {
	for (auto& p : key_bindings)
		Keys.SetBoundKey(p.first, p.second);	
}

int ClassicTetris::LevelToSpeed() {
	int base = 500;
	int exponent = -level/10.0;
	int speed = (int)(base * std::exp(exponent));
	return speed;
}
