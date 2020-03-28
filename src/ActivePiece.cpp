#include "ActivePiece.hpp"

bool StandardMoveLeft(ActivePiece& piece, Tetrion& tetrion) {
	if (!tetrion.ShapeCollides(piece.x-1, piece.y, piece.shape)) {
		piece.x--;
		return true;
	}

	return false;
}

bool StandardMoveRight(ActivePiece& piece, Tetrion& tetrion) {
	if (!tetrion.ShapeCollides(piece.x+1, piece.y, piece.shape)) {
		piece.x++;
		return true;
	}

	return false;

}

bool StandardRotateLeft(ActivePiece& piece, Tetrion& tetrion, int flags) {
	Shape rot = piece.shape.Rotate(ROTATE_LEFT);
	if (!tetrion.ShapeCollides(piece.x, piece.y, rot)) {
		piece.shape = rot;
		return true;
	}

	if (flags & STANDARD_WALL_KICK) {
		if (StandardWallKick(piece, tetrion, ROTATE_LEFT, flags)) return true;
	}

	if (flags & STANDARD_FLOOR_KICK) {
		if (StandardFloorKick(piece, tetrion, ROTATE_LEFT, flags)) return true;
	}

	return false;
}

bool StandardRotateRight(ActivePiece& piece, Tetrion& tetrion, int flags) {
	Shape rot = piece.shape.Rotate(ROTATE_RIGHT);
	if (!tetrion.ShapeCollides(piece.x, piece.y, rot)) {
		piece.shape = rot;
		return true;
	}

	if (flags & STANDARD_WALL_KICK) {
		if (StandardWallKick(piece, tetrion, ROTATE_RIGHT, flags)) return true;
	}

	if (flags & STANDARD_FLOOR_KICK) {
		if (StandardFloorKick(piece, tetrion, ROTATE_RIGHT, flags)) return true;
	}

	return false;
}

bool StandardWallKick(ActivePiece& piece, Tetrion& tetrion, ROTATION rot_type, int flags) {
	Shape rot = piece.shape.Rotate(rot_type);
	int wall = tetrion.ShapeCollidesWall(piece.x, piece.y, rot);

	if (wall == LEFT_WALL) {
		piece.x++;
		if (rot_type == ROTATE_RIGHT && StandardRotateRight(piece, tetrion, flags - (flags&STANDARD_WALL_KICK))) {
			return true;
		} else
		if (rot_type == ROTATE_LEFT && StandardRotateLeft(piece, tetrion, flags - (flags&STANDARD_WALL_KICK))) {
			return true;
		}
		piece.x--;
		return false;
	} else if (wall == RIGHT_WALL) {
		piece.x--;
		if (rot_type == ROTATE_RIGHT && StandardRotateRight(piece, tetrion, flags - (flags&STANDARD_WALL_KICK))) {
			return true;
		} else
		if (rot_type == ROTATE_LEFT && StandardRotateLeft(piece, tetrion, flags - (flags&STANDARD_WALL_KICK))) {
			return true;
		}
		piece.x++;
		return false;
	}

	return false;
}

bool StandardFloorKick(ActivePiece& piece, Tetrion& tetrion, ROTATION rot_type, int flags) {
	Shape rot = piece.shape.Rotate(rot_type);

	int x = piece.x + (rot_type == ROTATE_RIGHT ? -1 : 1), y;
	for (y = -2; y <= -1; ++y) {
		if (!tetrion.ShapeCollides(x, piece.y + y, rot)) {
			piece.x = x; piece.y = piece.y + y; piece.shape = rot;
			return true;
		}
	}

	x = piece.x;
	for (y = 1; y <= 3; ++y) {
		if (!tetrion.ShapeCollides(x, piece.y + y, rot)) {
			piece.x = x; piece.y = piece.y + y; piece.shape = rot;
			return true;
		}
	}

	return false;
}

void ClassicActivePiece::HandleAction(const std::string& action, Keypress_State state, Tetrion& tetrion) {
	auto held_time = [&]() -> double {
		return std::min((double)GetParameter("speed"), 200.0);
	};

	auto held_check = [&]() -> bool {
		return (held_timer.state == TIMER_STOPPED && state == KEY_HELD) ||
	           (held_timer.GetSeconds()*1500.0 >= held_time() && state == KEY_HELD);
	};

	if (state == KEY_UP) return;

	if (action == "Move Right" && held_check()) {
		MoveRight(tetrion);
	} else if (action == "Move Left" && held_check()) {
		MoveLeft(tetrion);
	} else if (action == "Rotate Right" && state == KEY_DOWN) {
		RotateRight(tetrion);
	} else if (action == "Rotate Left" && state == KEY_DOWN) {
		RotateLeft(tetrion);
	} else if (action == "Soft Drop") {
		soft_drop_pressed = true;
		soft_drop_mult = 12.0;
	} else if (action == "Hard Drop" && state == KEY_DOWN) {
		HardDrop(tetrion);
	}
}

void ClassicActivePiece::SetSlideTimer() {
	if (!slide_enabled) return;

	if (slide && max_slide_resets) {
		slide_timer.Reset();
		--max_slide_resets;
	}
}

bool ClassicActivePiece::SlideDone() {
	if (!slide_enabled) return true;
	return (slide_timer.GetSeconds()*1000.0 > GetParameter("speed") * 3.0);
}

bool ClassicActivePiece::Update(Tetrion& tetrion) {
	if (hard_drop_pressed) return false;

	if (!(right_pressed || left_pressed))
		held_timer.Stop();

	if (frame.GetSeconds()*1000.0*soft_drop_mult > GetParameter("speed")) {
		frame.Reset();
		if (!tetrion.ShapeCollides(x, y+1, shape)) {
			++y;
			soft_drop_pressed = 1.0;
			slide = false;
			return true;
		} else if (!slide) {
			slide = true;
			SetSlideTimer();
		}
	}

	if (slide) {
		if (SlideDone())
			return false;
	}

	if (!soft_drop_pressed)
		soft_drop_mult = 1.0;
	soft_drop_pressed = false;
	return true;
}

void ClassicActivePiece::MoveLeft(Tetrion& tetrion) {
	left_pressed = true;
	if (soft_drop_mult != 1.0) return;
	if (StandardMoveLeft(*this, tetrion)) {
		held_timer.Reset();
		SetSlideTimer();
	}
}

void ClassicActivePiece::MoveRight(Tetrion& tetrion) {
	right_pressed = true;
	if (soft_drop_mult != 1.0) return;
	if (StandardMoveRight(*this, tetrion)) {
		held_timer.Reset();
		SetSlideTimer();
	}
}

void ClassicActivePiece::RotateRight(Tetrion& tetrion) {
	int flags = (floor_kick_enabled ? STANDARD_FLOOR_KICK : 0) |
                (wall_kick_enabled  ? STANDARD_WALL_KICK  : 0);
	StandardRotateRight(*this, tetrion, flags);
	SetSlideTimer();
}

void ClassicActivePiece::RotateLeft(Tetrion& tetrion) {
	int flags = (floor_kick_enabled ? STANDARD_FLOOR_KICK : 0) |
                (wall_kick_enabled  ? STANDARD_WALL_KICK  : 0);
	StandardRotateLeft(*this, tetrion, flags);
	SetSlideTimer();
}

void ClassicActivePiece::HardDrop(Tetrion& tetrion) {
	hard_drop_pressed = true;
	while (y < tetrion.height) {
		if (tetrion.ShapeCollides(x, y, shape)) {
			--y;
			return;
		}
		++y;
	}
}

void ClassicActivePiece::GetGhostPiece(int& ghost_x, int& ghost_y, Shape& ghost_shape, Tetrion& tetrion) {
	ghost_x = x;
	ghost_shape = shape;

	ghost_y = y;
	while (ghost_y < tetrion.height) {
		if (tetrion.ShapeCollides(ghost_x, ghost_y, ghost_shape)) {
			--ghost_y;
			return;
		}
		++ghost_y;
	}
}
