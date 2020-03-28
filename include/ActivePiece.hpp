#pragma once

#include "Tetrion.hpp"

struct ActivePiece {
	ActivePiece(int _x, int _y, Shape s): x(_x), y(_y), shape(s) { ; }

	int x = 0, y = 0;
	Shape shape;

	// Handles a given action such as "Move Right"
	virtual void HandleAction(const std::string& action, Keypress_State state, Tetrion& tetrion) { ; }
	virtual bool Update(Tetrion& tetrion) = 0;

	std::map<std::string, int> parameters;
	int PARAMETER_UNDEFINED = 0;

	void SetParameter(const std::string& str, int val) { parameters[str] = val; }
	int GetParameter(const std::string& str) {
		auto f = parameters.find(str);
		if (f == parameters.end()) return PARAMETER_UNDEFINED;
		return f->second;
	}
};

enum { STANDARD_WALL_KICK=0x1, STANDARD_FLOOR_KICK=0x2 };
// all functions return false if no movement occured
bool StandardMoveLeft(ActivePiece& piece, Tetrion& tetrion);
bool StandardMoveRight(ActivePiece& piece, Tetrion& tetrion);

bool StandardRotateLeft(ActivePiece& piece, Tetrion& tetrion, int flags = 0);
bool StandardRotateRight(ActivePiece& piece, Tetrion& tetrion, int flags = 0);
bool StandardWallKick(ActivePiece& piece, Tetrion& tetrion, ROTATION rot_type, int flags);
bool StandardFloorKick(ActivePiece& piece, Tetrion& tetrion, ROTATION rot_type, int flags);

struct ClassicActivePiece : public ActivePiece {
	ClassicActivePiece(int x, int y, Shape s, int speed, bool slide_p, bool floor_kick_p, bool wall_kick_p):
	  ActivePiece(x,y,s),
	  slide_enabled(slide_p), floor_kick_enabled(floor_kick_p), wall_kick_enabled(wall_kick_p)
	  { SetParameter("speed",speed); frame.Start(); }

	Timer frame;
	double soft_drop_mult = 1.0;
	bool left_pressed = false, right_pressed = false;
	bool soft_drop_pressed = false;
	bool hard_drop_pressed = false;
	Timer held_timer;

	bool slide_enabled;
	bool floor_kick_enabled;
	bool wall_kick_enabled;

	bool slide = false;
	Timer slide_timer;
	int max_slide_resets = 16;
	void SetSlideTimer();
	bool SlideDone();

	void HandleAction(const std::string& action, Keypress_State state, Tetrion& tetrion);
	bool Update(Tetrion& tetrion); // if false, stop being active.

	void MoveLeft(Tetrion& tetrion);
	void MoveRight(Tetrion& tetrion);
	void RotateRight(Tetrion& tetrion);
	void RotateLeft(Tetrion& tetrion);
	void HardDrop(Tetrion& tetrion);

	void GetGhostPiece(int& ghost_x, int& ghost_y, Shape& ghost_shape, Tetrion& tetrion);
};
