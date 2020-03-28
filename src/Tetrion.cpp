#include "Tetrion.hpp"
#include "Cmd.hpp"

Mino& Shape::At(int8_t x, int8_t y) {
	return minos.at(x + width*y);
}

std::string Mino::GetTypeTexture() {
	std::string skin = GetCVarString("skin");
	if (skin.empty()) return "";
	std::string tex = skin + "/" + GetCVarString("t" + std::to_string(type));
	return tex;
}

Shape Shape::Rotate(ROTATION direction) {
	switch (direction) {
	case ROTATE_RIGHT: {
		Shape s(height, width, std::vector<Mino>(width * height, Mino()));
		for (int x = 0; x < s.width; ++x)
			for (int y = 0; y < s.height; ++y)
				s.At(x, y) = At(y, s.width-x-1);
		return s;
		}

	case ROTATE_LEFT: {
		Shape s(height, width, std::vector<Mino>(width * height, Mino()));
		for (int x = 0; x < s.width; ++x)
			for (int y = 0; y < s.height; ++y)
				s.At(x, y) = At(s.height-y-1, x);
		return s;
		}

	case ROTATE_180: {
		Shape s(width, height, std::vector<Mino>(width * height, Mino()));
		for (int x = 0; x < s.width; ++x)
			for (int y = 0; y < s.height; ++y)
				s.At(x, y) = At(s.width-x-1, s.height-y-1);
		return s;
		}

	default:
		return *this;
	}
}

void Tetrion::AssignSize(int w, int h) {
	minos = std::vector<Mino>(w * h);
	width = w;
	height = h;
}

Mino Tetrion::Get(int x, int y) {
	if (x < 0 || y < 0 || x >= width || y >= height)
		return Mino(1,0);
	return minos.at(x + y*width);
}

Mino* Tetrion::At(int x, int y) {
	if (x < 0 || y < 0 || x >= width || y >= height)
		return nullptr;
	return &minos.at(x + y*width);
}

void Tetrion::EmplaceShape(int x, int y, Shape shape) {
	int xi, yi;

	for (xi = 0; xi < shape.width; ++xi) {
		for (yi = 0; yi < shape.height; ++yi) {
			Mino m = shape.At(xi, yi);

			if (m.type == 0) continue;

			Mino * loc = At(x + xi, y + yi);
			if (!loc) continue;
			*loc = m;
		}
	}
}

bool Tetrion::ShapeCollides(int x, int y, Shape shape) {
	int xi, yi;

	for (xi = 0; xi < shape.width; ++xi) {
		for (yi = 0; yi < shape.height; ++yi) {
			Mino m = shape.At(xi, yi);

			if (m.type == 0) continue;

			Mino loc = Get(x + xi, y + yi);
			if (loc.solid) return true;
		}
	}

	return false;
}

int Tetrion::ShapeCollidesWall(int x, int y, Shape shape) {
	int xi, yi;

	for (xi = 0; xi < shape.width; ++xi) {
		for (yi = 0; yi < shape.height; ++yi) {
			Mino m = shape.At(xi, yi);
			if (m.type == 0) continue;
			if (x + xi < 0) return LEFT_WALL;
			else if (x +xi >= width) return RIGHT_WALL;
		}
	}

	return 0;
}
