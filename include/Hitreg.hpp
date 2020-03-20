#pragma once

#include "Vec2.hpp"

bool CheckCollision(const Rect& rect, const Vec2& vec);
bool CheckCollision(const Rect& a, const Rect& b);
bool CheckCollision(const Rect& rect, const Line& line);
bool CheckCollision(const Line& a, const Line& b);
