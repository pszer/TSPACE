#include "Hitreg.hpp"
#include "Timer.hpp"

bool CheckCollision(const Rect& rect, const Vec2& vec) {
	return (vec.x >= rect.x && vec.x <= rect.x + rect.w)
	    && (vec.y >= rect.y && vec.y <= rect.y + rect.h);
}

bool CheckCollision(const Rect& a, const Rect& b) {
	return !(a.x+a.w < b.x) &&
	       !(a.x > b.x+b.w) &&
	       !(a.y+a.h < b.y) &&
	       !(a.y > b.y+b.h);
}

bool CheckCollision(const Rect& rect, const Line& line) {
	const Vec2 pos = rect.Pos(), size = rect.Size();

	const Vec2 topleft = pos, topright = pos + Vec2(size.x, 0.0),
	        bottomleft = topleft + Vec2(0.0, size.y), bottomright = bottomleft + Vec2(size.x, 0.0);

	// lines of the rectangle
	const Line top = Line(topleft, topright),
	           bottom = Line(bottomleft, bottomright),
	           left = Line(topleft, bottomleft),
	           right = Line(topright, bottomright);

	// check one by one so not every side has to be calculated if there is a collision
	if (CheckCollision(line, top)) return true;
	if (CheckCollision(line, bottom)) return true;
	if (CheckCollision(line, left)) return true;
	if (CheckCollision(line, right)) return true;

	return false;
}

bool CheckCollision(const Line& _a, const Line& _b) {
	const Vec2& a = _a.a, &b = _a.b, &c = _b.a, &d = _b.b;

	Rect P = Rect(a.x, a.y, b.x-a.x, b.y-a.y).Absolute(),
	     Q = Rect(c.x, c.y, d.x-c.x, d.y-c.y).Absolute();
	if (!CheckCollision(P,Q)) return false;

	// check overlapping points
	//if ((a == c) || (a == d) || (b == c) || (b == d))
	//	return true;

	// check parallel
	if ( ((a.x - b.x) * (c.y - d.y) + (a.y - b.y) * (d.x - c.x) == 0.0) && 
	     ((a.x - d.x) * (c.y - b.y) + (a.y - d.y) * (b.x - c.x) == 0.0) ) return true;

	double uA, uB;
	uA = ((d.x-c.x)*(a.y-c.y) - (d.y-c.y)*(a.x-c.x)) / ((d.y-c.y)*(b.x-a.x) - (d.x-c.x)*(b.y-a.y));
	uB = ((b.x-a.x)*(a.y-c.y) - (b.y-a.y)*(a.x-c.x)) / ((d.y-c.y)*(b.x-a.x) - (d.x-c.x)*(b.y-a.y)); 

	if (uA >= 0.0 && uA <= 1.0 && uB >= 0.0 && uB <= 1.0) {
		return true;
	}

	return false;
}
