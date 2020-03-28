#include "Tetris.hpp"
#include "Core.hpp"
#include "Sound.hpp"

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
Shape(4,4,{m0,m0,m0,m0,
           m1,m1,m1,m1,
		   m0,m0,m0,m0,
		   m0,m0,m0,m0}),
// T
Shape(3,3,{m0,m2,m0,
           m2,m2,m2,
		   m0,m0,m0}),
// O
Shape(2,2,{m3,m3,
           m3,m3}),
// L	  
Shape(3,3,{m0,m4,m0,
           m0,m4,m0,
           m0,m4,m4}),
// J	  
Shape(3,3,{m0,m5,m0,
           m0,m5,m0,
           m5,m5,m0}),
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

Ruleset PLACEHOLDER_RULESET = Ruleset("PLACEHOLDER");
ClassicTetris CLASSIC_TETRIS = ClassicTetris();

void Ruleset::TetrionViewDetails(SDL_Rect& viewport, SDL_Rect& mino) {
	float scale = Renderer.GetWindowScale();

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

void Ruleset::DrawTetrion() {
	SDL_Rect viewport;
	SDL_Rect mino_r;
	TetrionViewDetails(viewport, mino_r);

	std::string bg_tex  = Mino(0,0).GetTypeTexture();

	SDL_RenderSetViewport(Renderer.renderer, &viewport);
	for (int x = tetrion_view_x; x < tetrion_view_x + tetrion_view_w; ++x) {
		for (int y = tetrion_view_y; y < tetrion_view_y + tetrion_view_h; ++y) {
			mino_r.x = (x - tetrion_view_x) * mino_r.w;
			mino_r.y = (y - tetrion_view_y) * mino_r.h;
			Renderer.RenderTexture(bg_tex  , nullptr, &mino_r);

			Mino m = TetrionGet(x, y);
			if (m.type == 0) continue;

			std::string mino_tex = m.GetTypeTexture();

			if (mino_tex.empty()) continue;

			Renderer.RenderTexture(mino_tex, nullptr, &mino_r);
		}
	}
	SDL_RenderSetViewport(Renderer.renderer, nullptr);
}

std::string Ruleset::KeyBindingToString(const std::string& str) {
	return ruleset_name + "::" + str;
}

void Ruleset::ActivateKeyBindings() {
	for (auto& p : key_bindings) {
		Keys.SetBoundKey(KeyBindingToString(p.first), p.second);
	}
}

void Ruleset::RenderHud() {
	for (auto& h : hud_elements) {
		h->Render();
	}
}

void DrawShape(Shape& s, int x, int y, SDL_Rect& mino) {
	int xi, yi;
	SDL_Rect r = {0,0, mino.w, mino.h};

	for (xi = 0; xi < s.width; ++xi) {
		for (yi = 0; yi < s.height; ++yi) {
			Mino m = s.At(xi, yi);

			if (m.type == 0) continue;

			std::string mino_tex = m.GetTypeTexture();

			r.x = x + xi * r.w;
			r.y = y + yi * r.h;

			Renderer.RenderTexture(mino_tex, nullptr, &r);
		}
	}
}

void DrawShapeInTetrion(Shape& s, int x, int y, int tvx, int tvy, SDL_Rect& mino) {
	DrawShape(s, (x-tvx)*mino.w, (y-tvy)*mino.h, mino);
}

RULESET_STATE ClassicTetris::Update(double dt, RULESET_SIGNAL sig) {
	if (sig == RULESET_ACTION && !game_over) play_time.Start();
	else play_time.Pause();
	UpdateTimeString();

	if (!active_piece) {
		bool state = NewActivePiece();
		if (!state) return RULESET_TERMINATE;
	} else {
		for (auto& bind : key_bindings) {
			Keypress_State state = Keys.GetKeyState(KeyBindingToString(bind.first));
			if (state != KEY_NULL)
				active_piece->HandleAction(bind.first, state, tetrion);
		}

		bool active_piece_status = active_piece->Update(tetrion);

		if (!active_piece_status) {
			int lines_cleared;
			tetrion.EmplaceShape(active_piece->x, active_piece->y, active_piece->shape);
			switch (scorer.Check(tetrion, &lines_cleared)) {
			case SCORE_SMALL:
				Sound::PlaySound(GetCVarString("score0"));
				break;
			case SCORE_MEDIUM:
				Sound::PlaySound(GetCVarString("score1"));
				break;
			case SCORE_BIG:
				Sound::PlaySound(GetCVarString("score2"));
				break;
			case SCORE_MASSIVE:
				Sound::PlaySound(GetCVarString("score3"));
				break;
			}

			if (!NewActivePiece())
				return RULESET_TERMINATE;

			lines += lines_cleared;
			UpdateLevel();
		}
	}

	return RULESET_GOING;
}

#define InViewport(viewport,body) {SDL_RenderSetViewport(Renderer.renderer,&viewport); \
	body; \
	SDL_RenderSetViewport(Renderer.renderer, nullptr);}


void ClassicTetris::Render() {
	SDL_Rect viewport;
	SDL_Rect mino_r;
	TetrionViewDetails(viewport, mino_r);

	DrawTetrion();
	if (ghost_flag && GetCVarInt("ghost"))
		RenderGhostPiece(viewport, mino_r);
	RenderActivePiece(viewport, mino_r);

	RenderHud();
}

void ClassicTetris::RenderGhostPiece(SDL_Rect& viewport, SDL_Rect& mino_r) {
	if (!active_piece) return;

	int ghost_x, ghost_y;
	Shape ghost_shape;
	active_piece->GetGhostPiece(ghost_x, ghost_y, ghost_shape, tetrion);

	for (auto& mino : ghost_shape.minos) {
		if (mino.type) mino.type = 0x7F;
	}

	InViewport(viewport, DrawShapeInTetrion(ghost_shape, ghost_x, ghost_y,
	  tetrion_view_x, tetrion_view_y, mino_r));
}

void ClassicTetris::RenderActivePiece(SDL_Rect& viewport, SDL_Rect& mino_r) {
	if (!active_piece) return;

	InViewport(viewport, DrawShapeInTetrion(active_piece->shape,
	  active_piece->x, active_piece->y, tetrion_view_x, tetrion_view_y, mino_r));
}

int ClassicTetris::LevelToSpeed() {
	double base = 500.0;
	double exponent = -level / 10.0;
	int speed = (int)(base * std::exp(exponent));
	return speed;
}

bool ClassicTetris::UpdateLevel() {
	if (!update_levels) return false;
	int new_level = start_level + lines / 10;
	if (new_level != level) {
		level = new_level;
		return true;
	}
	return false;
}

void ClassicTetris::FillQueue(void) {
	for (int i = 0; i < next_queue.size(); ++i)
		next_queue[i] = randomizer.Next();
}

Shape ClassicTetris::PushDownQueue(void) {
	Shape s = next_queue[0];

	for (int i = 0; i < next_queue.size() - 1; ++i)
		next_queue[i] = next_queue[i+1];
	next_queue[next_queue.size() - 1] = randomizer.Next();

	return s;
}

bool ClassicTetris::NewActivePiece() {
	Shape s = PushDownQueue();
	int x = tetrion.width/2 - (s.width+1)/2;
	int y = 4;

	while (tetrion.ShapeCollides(x, y, s)) {
		if (y == -4) return false;
		--y;
	}

	active_piece = std::make_unique<ClassicActivePiece>(x, y, s, LevelToSpeed(), long_slide, floor_kick, wall_kick);
	return true;
}

void ClassicTetris::CreateHud() {
	hud_elements.clear();

	hud_elements.push_back(std::make_unique<HudCounter<long long>>(
	  score, "monospace.ttf", FONT_P32, ALIGN_RIGHT, "SCORE:  ", '0', 7, 0.5, 0.5, -130, -230));
	hud_elements.push_back(std::make_unique<HudCounter<int>>(
	  level, "monospace.ttf", FONT_P32, ALIGN_RIGHT, "LEVEL:     ", '0', 4, 0.5, 0.5, -130, -200));
	hud_elements.push_back(std::make_unique<HudCounter<int>>(
	  lines, "monospace.ttf", FONT_P32, ALIGN_RIGHT, "LINES:     ", '0', 4, 0.5, 0.5, -130, -170));

	hud_elements.push_back(std::make_unique<HudString>(
	  time_string, "monospace.ttf", FONT_P32, ALIGN_RIGHT, "TIMER: ", ' ', 8, 0.5, 0.5, -130, -140));
}

void ClassicTetris::UpdateTimeString() {
	time_string = play_time.Format("%m:%s:%d");
}
