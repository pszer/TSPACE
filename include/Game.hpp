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

	void SetRuleset(Ruleset * r);
	Ruleset * ruleset = &CLASSIC_TETRIS;
	bool playing = false;
	void BeginGame();
private:
} Game;
