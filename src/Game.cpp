#include "Game.hpp"

struct Game Game;

void Game::Init() {
	SetRuleset(&CLASSIC_TETRIS);
	BeginGame();
}

void Game::Update() {
	if (ruleset) {
		bool state =
			ruleset->Update(FrameLimit.deltatime, playing ? RULESET_ACTION : RULESET_DELAY);

		if (state == RULESET_TERMINATE) playing = false;
	}
}

void Game::Render() {
	if (ruleset)
	ruleset->Render();
}

void Game::SetRuleset(Ruleset * r) {
	if (!r) return;
	ruleset = r;
	ruleset->ActivateKeyBindings();
}

void Game::BeginGame() {
	playing = true;
}

void Game::Quit() {
}

