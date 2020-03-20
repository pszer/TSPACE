#pragma once

#include <SDL2/SDL.h>

#include <vector>

#include "Vec2.hpp"
#include "Timer.hpp"

//                  not pressed
enum Keypress_State { KEY_NULL , KEY_DOWN , KEY_HELD , KEY_UP };
enum { MOUSE1=0x00ffffff , MOUSE2=0x00ffffff+1 , MOUSE3=0x00ffffff+2 , MOUSE4=0x00ffffff+3, MOUSE5=0x00ffffff+4 };
struct Keypress {
	Keypress(SDL_Keycode c, Keypress_State s): code(c), state(s) { timer.Start(); }
	SDL_Keycode code;
	Keypress_State state;
	Timer timer;

	double GetDuration();
};

extern struct Event {

	void Init();
	void Update();

	SDL_Event event;
	void HandleEvents();

	std::vector<Keypress> Keypresses;
	Keypress_State GetKey(SDL_Keycode c);
	double GetKeyDuration(SDL_Keycode c);
	void EnableRepeatedKeys();
	void DisableRepeatedKeys();
	bool repeated_keys = false;

	int win_w, win_h;
	int mouse_x, mouse_y,
	    mouse_dx, mouse_dy, wheel_y;

	void HandleQuitEvent();
	void HandleWindowEvent();

	void HandleKeyDownEvent();
	void HandleKeyUpEvent();

	void HandleMouseDownEvent();
	void HandleMouseUpEvent();
	void HandleMouseMotionEvent();
	void HandleMouseWheelEvent();

	void HandleTextInputEvent();

	bool text_input_enabled = false;
	void EnableTextInput();
	void DisableTextInput();

} Event;

// transforms position coordinates from (0.0-1.0, 0.0-1.0) to screen coordinates
Vec2 PosToScreen(Vec2 pos);
