#pragma once

#include <cstdint>
#include <memory>
#include <array>
#include <vector>
#include <string>
#include <random>
#include <ctime>

#include "Render.hpp"
#include "Tetrion.hpp"
#include "ActivePiece.hpp"
#include "Randomizer.hpp"
#include "Scorer.hpp"
#include "Hud.hpp"

enum RULESET_STATE  { RULESET_GOING , RULESET_TERMINATE };
enum RULESET_SIGNAL { RULESET_DELAY , RULESET_ACTION };

enum TETRION_ALIGN_HORIZONTAL { TETRION_ALIGN_LEFT=0 , TETRION_ALIGN_MIDDLE=1 , TETRION_ALIGN_RIGHT =2  };
enum TETRION_ALIGN_VERTICAL   { TETRION_ALIGN_TOP =0 , TETRION_ALIGN_BOTTOM=2 };

class Ruleset {
public:
	Ruleset(const std::string& name): ruleset_name(name)
	  { ; }

	const std::string ruleset_name;

	// The rectangle of what parts of the tetrion
	// is actually rendered. Can be used to clip out
	// buffer zones.
	uint8_t tetrion_view_x = 0;
	uint8_t tetrion_view_y = 4;
	uint8_t tetrion_view_w = 10;
	uint8_t tetrion_view_h = 20;

	// Decides where to draw the tetrion
	float tetrion_align_x = 0.5;
	float tetrion_align_y = 0.5;
	int tetrion_pad_x = 50;
	int tetrion_pad_y = 0;
	TETRION_ALIGN_HORIZONTAL tetrion_align_horizontal = TETRION_ALIGN_MIDDLE;
	TETRION_ALIGN_HORIZONTAL tetrion_align_vertical   = TETRION_ALIGN_MIDDLE;

	void TetrionViewDetails(SDL_Rect& viewport, SDL_Rect& mino);

	short mino_w = 24;
	short mino_h = 24;

	void DrawTetrion();

	// The field of Minos
	Tetrion tetrion;
	Mino TetrionGet(int8_t x, int8_t y);
	bool TetrionGetSolid(int8_t x, int8_t y);

	// A pool of every single possible piece shape.
	std::vector<Shape> shape_pool;

	// A vector of names for different key bindings.
	std::map<std::string, SDL_Keycode> key_bindings;
	std::string KeyBindingToString(const std::string& str);
	void ActivateKeyBindings();

	std::vector<std::unique_ptr<HudElement>> hud_elements;
	void RenderHud();

	// The players score
	long long score = 0;
	Timer play_time;
	bool game_over = false;

	virtual RULESET_STATE Update(double dt, RULESET_SIGNAL sig = RULESET_ACTION);

	virtual void Render() { DrawTetrion(); }
} extern PLACEHOLDER_RULESET;

// The standard pool of tetriminos
extern const std::vector<Shape> STANDARD_POOL;
void DrawShape(Shape& s, int x, int y, SDL_Rect& mino);
void DrawShapeInTetrion(Shape& s, int x, int y, int tvx, int tvy, SDL_Rect& mino);

class ClassicTetris : public Ruleset {
public:
	ClassicTetris():
	  Ruleset("Classic Tetris")
	{
		tetrion.AssignSize(10, 24);

		tetrion_view_x = 0;
		tetrion_view_y = 4;
		tetrion_view_w = 10;
		tetrion_view_h = 20;

		shape_pool = STANDARD_POOL;
		randomizer = StandardRandomizer(&shape_pool, std::time(nullptr));
		FillQueue();

		key_bindings = {
		 std::make_pair("Move Left",  SDLK_LEFT),
		 std::make_pair("Move Right", SDLK_RIGHT),
		 std::make_pair("Rotate Right", SDLK_UP),
		 std::make_pair("Rotate Left", SDLK_z),
		 std::make_pair("Soft Drop", SDLK_DOWN),
		 std::make_pair("Hard Drop", SDLK_SPACE),
		};

		CreateHud();

		level = start_level;
	}

	RULESET_STATE Update(double dt, RULESET_SIGNAL sig = RULESET_ACTION);
	void Render();

	void DrawGhostPiece();
protected:
	int start_level = 0;
	int level  = 0;
	int lines  = 0;
	int pieces = 0;

	void CreateHud();

	void RenderGhostPiece(SDL_Rect& viewport, SDL_Rect& mino_r);
	void RenderActivePiece(SDL_Rect& viewport, SDL_Rect& mino_r);

	StandardRandomizer randomizer;
	std::unique_ptr<ClassicActivePiece> active_piece = nullptr;
	std::array<Shape, 3> next_queue;

	void FillQueue(void);
	Shape PushDownQueue(void);

	LeveledScorer scorer = LeveledScorer(score, level);

	// returns false if cannot spawn new active piece without collision
	bool NewActivePiece();

	int LevelToSpeed();
	bool UpdateLevel(); // returns true if level change

	// gameplay flags
	bool long_slide = true;
	bool wall_kick  = true;
	bool floor_kick = true;
	bool ghost_flag = true;
	bool update_levels = true;

	void UpdateTimeString();
	std::string time_string;
} extern CLASSIC_TETRIS;

