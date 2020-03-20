#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>

struct Mino {
	Mino() { ; }
	Mino(bool s, int8_t t): solid(s), type(t) { ; }
	bool solid  = false;
	int8_t type = 0;
};

enum ROTATION { ROTATE_LEFT , ROTATE_RIGHT , ROTATE_180 };

struct Shape {
	Shape() { ; }
	Shape(const uint8_t w, const uint8_t h,
	      std::vector<Mino> minos_description):
		  width(w), height(h), minos(minos_description)
		  { minos.shrink_to_fit(); }
	uint8_t width=0, height=0;
	std::vector<Mino> minos;

	Shape Rotate(ROTATION direction);
	Mino& At(int8_t x, int8_t y);
};

// Tetrion is an entire playfield.
struct Tetrion {
	int width  = 10,
	    height = 24;
	std::vector<Mino> minos;

	// Assigns a size to the tetrion.
	// Completely overwrites all minos, do not use
	// it for resizing dynamically sized tetrions during play.
	void AssignSize(int w, int h);
	Mino  Get(int x, int y);
	Mino* At(int x, int y);

	void EmplaceShape(int x, int y, Shape shape);
};

struct ActivePiece {
	ActivePiece(int _x, int _y, Shape s): x(_x), y(_y), shape(s) { ; }

	int x = 0, y = 0;
	Shape shape;

	// Handles a given action such as "Move Right"
	virtual void HandleAction(const std::string& action, Keypress_State state, Tetrion& tetrion) { ; }

	std::map<std::string, int> parameters;
	int PARAMETER_UNDEFINED = 0;

	void SetParameter(const std::string& str, int val) { parameters[str] = val; }
	int GetParameter(const std::string& str) {
		auto f = parameters.find(str);
		if (f == parameters.end()) return PARAMETER_UNDEFINED;
		return f->second;
	}

	virtual bool Done() { return false; }
};

struct ClassicActivePiece : public ActivePiece {
	ClassicActivePiece(int x, int y, Shape s, int speed): ActivePiece(x,y,s)
	  { SetParameter("speed",speed); }

	void HandleAction(const std::string& action, Tetrion& tetrion) { ; }
};
