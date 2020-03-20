#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <random>

#include "Render.hpp"
#include "Tetrion.hpp"
#include "Randomizer.hpp"

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

	std::string GetTypeTexture(uint8_t type);
	void DrawTetrion();

	// The field of Minos
	Tetrion tetrion;
	Mino TetrionGet(int8_t x, int8_t y);
	bool TetrionGetSolid(int8_t x, int8_t y);

	// A pool of every single possible piece shape.
	std::vector<Shape> shape_pool;

	// A vector of names for different key bindings.
	std::map<std::string, SDL_Keycode> key_bindings;
	void ActivateKeyBindings();

	// The players score/line count
	long long score = 0;
	long long lines = 0;

	virtual RULESET_STATE Update(double dt, RULESET_SIGNAL sig = RULESET_ACTION);

	virtual void Render() { DrawTetrion(); }
} extern PLACEHOLDER_RULESET;

// The standard pool of tetriminos
extern const std::vector<Shape> STANDARD_POOL;

class ClassicTetris : public Ruleset {
public:
	ClassicTetris(): Ruleset("Classic Tetris") {
		tetrion.AssignSize(10, 24);

		tetrion_view_x = 0;
		tetrion_view_y = 4;
		tetrion_view_w = 10;
		tetrion_view_h = 24;

		shape_pool = STANDARD_POOL;

		key_bindings = {
		 std::make_pair("Move Left",  SDLK_LEFT),
		 std::make_pair("Move Right", SDLK_RIGHT),
		 std::make_pair("Rotate Right", SDLK_UP),
		 std::make_pair("Rotate Left", SDLK_z),
		 std::make_pair("Soft Drop", SDLK_DOWN),
		 std::make_pair("Hard Drop", SDLK_SPACE),
		};
	}

protected:
	unsigned int level  = 0;
	unsigned int lines  = 0;
	unsigned int pieces = 0;

	int LevelToSpeed();
} extern CLASSIC_TETRIS;
