#pragma once

#include <vector>
#include <memory>

#include "Hitreg.hpp"
#include "Commands.hpp"
#include "Sound.hpp"
#include "Timer.hpp"
#include "Tetris.hpp"

extern struct Game {
	void Init();
	void Quit();

	void Update();
	void Render();

	Ruleset * ruleset = &CLASSIC_TETRIS;
private:
} Game;
