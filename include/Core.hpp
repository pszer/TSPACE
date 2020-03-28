#pragma once

#include <string>
#include <algorithm>

#include <SDL2/SDL.h>

#include "Log.hpp"
#include "Render.hpp"
#include "Commands.hpp"
#include "Music.hpp"
#include "Config.hpp"
#include "Game.hpp"
#include "Menu.hpp"

#define DEF_WIN_W 800
#define DEF_WIN_H 600
#define DEF_WIN_TITLE "TSPACE"
#define DEF_RENDER_FLAG SDL_RENDERER_ACCELERATED

#define DEF_CONSOLE_FONT "inconsolata.ttf"

#define WIN_SCALE_STEP 0.250

struct InitParameters {
	int win_w = DEF_WIN_W,
	    win_h = DEF_WIN_H;
	std::string win_title = DEF_WIN_TITLE;
	SDL_WindowFlags win_flags = SDL_WINDOW_RESIZABLE; 
	SDL_RendererFlags render_flags = DEF_RENDER_FLAG;
};

enum CORE_GAMESTATE { GAMESTATE_MENU , GAMESTATE_GAME };

extern struct Core {
	Core() { ; }

	int  Init(InitParameters p = InitParameters()); // returns 0 on success, -1 for error
	void Quit();

	void HandleEvents();
	void MainLoop();
	void MainUpdate();
	void MainRender();

	bool going = true; // set to false to exit game main loop

	SDL_Window * window;

	void SetGamestate(CORE_GAMESTATE gs);
	CORE_GAMESTATE gamestate = GAMESTATE_MENU;
	Menu core_menu;

	// CONSOLE
	struct Console {
		void Toggle();

		std::string font = DEF_CONSOLE_FONT;
		SDL_Color bg = {0x08,0x08,0x08,0xEF},
		          fg = {0xFF,0xFF,0xFF,0xFF};
		bool open = false;
		int bottom_offset = 100; // how many pixels from the bottom
		                         // of the screen console is offset

		int cursor = 0;
		int page_scroll = 0; // how many lines to scroll up
		const int page_scroll_step = 5;
		std::string text = "";

		const int MAX_COMMAND_HISTORY = 64;
		std::vector<std::string> CommandHistory;
		int history_scroll = 0;
		void SetHistoryText();

		void HandleKeypresses();

		void Enter();
		void Reset();
		void Backspace();
		void TextInput(const std::string& input);

		void Left();
		void Right();
		void Up();
		void Down();
		void PageUp();
		void PageDown();
		void Home();
		void End();

		void Render();
		void Update();
	} Console;

	void RenderFPS();

} Core;

