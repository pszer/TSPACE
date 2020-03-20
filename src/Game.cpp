#include "Game.hpp"

struct Game Game;

void Game::Init() {
}

void Game::Update() {
	if (ruleset)
		ruleset->Update(FrameLimit.deltatime);

	//ruleset->tetrion.EmplaceShape(3, 8, STANDARD_POOL[3]);
}

void Game::Render() {
	if (ruleset)
	ruleset->Render();
}

void Game::Quit() {
}

