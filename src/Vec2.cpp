#include "Vec2.hpp"

Vec2 Rect::Pos() const {
	return Vec2(x,y);
}

Vec2 Rect::Size() const {
	return Vec2(w,h);
}

void Rect::SetPos(const Vec2& v) {
	x = v.x;
	y = v.y;
}

void Rect::SetSize(const Vec2& s) {
	w = s.x;
	h = s.y;
}

SDL_Rect Rect::ToSDLRect() {
	return {(int)x,(int)y,(int)w,(int)h};
}


Vec2 Rect::Middle() {
	return Vec2(x+w/2.0,y+h/2.0);
}

Rect Rect::Absolute() {
	Rect r = *this;
	if (r.w < 0.0) {
		r.x += r.w;
		r.w *= -1.0;
	}
	if (r.h < 0.0) {
		r.y += r.h;
		r.h *= -1.0;
	}
	return r;
}

Vec2 operator+(const Vec2& a, const Vec2& b) {
	return Vec2(a.x + b.x, a.y + b.y);
}
Vec2 operator-(const Vec2& a, const Vec2& b) {
	return Vec2(a.x - b.x, a.y - b.y);
}
bool operator==(const Vec2& a, const Vec2& b) {
	return a.x==b.x && a.y==b.y;
}
Vec2 operator*(const Vec2& a, const double& s) {
	return Vec2(a.x * s, a.y * s);
}
Vec2 operator/(const Vec2& a, const double& s) {
	return Vec2(a.x / s, a.y / s);
}

Rect operator+(const Rect& r, const Vec2& v) {
	return Rect(r.x+v.x, r.y+v.y, r.w, r.h);
}

Rect operator-(const Rect& r, const Vec2& v) {
	return Rect(r.x-v.x, r.y-v.y, r.w, r.h);
} 
