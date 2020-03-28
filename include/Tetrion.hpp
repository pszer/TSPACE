#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>

#include "Timer.hpp"
#include "Events.hpp"

struct Mino {
	Mino() { ; }
	Mino(bool s, int8_t t): solid(s), type(t) { ; }
	bool solid  = false;
	int8_t type = 0;

	std::string GetTypeTexture();
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

enum { LEFT_WALL , RIGHT_WALL };

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
	bool ShapeCollides(int x, int y, Shape shape);
	int  ShapeCollidesWall(int x, int y, Shape shape);
};
