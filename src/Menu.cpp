#include "Menu.hpp"
#include "Core.hpp"

Rect Menu_Element::GetRect() {
	Rect result = Rect(0.0, 0.0, size.x, size.y);

	result.x = pos.x * Event.win_w + offset.x;
	result.y = pos.y * Event.win_h + offset.y;

	return result;
}

void Menu_Screen::Update(Menu * m) {
	for (auto h = elements.begin(); h != elements.end(); ++h) {
		(*h)->Update(m);
	}

	for (auto h = elements.begin(); h != elements.end();) {
		if ((*h)->destroy) {
			elements.erase(h);
		} else ++h;
	}
}

void Menu_Screen::Render() {
	for (auto h = elements.begin(); h != elements.end(); ++h) {
		(*h)->Render();
	}
}

void Menu_Screen::Add(std::unique_ptr<Menu_Element> element) {
	elements.push_back(std::move(element));
}

void Menu_Screen::Click(Menu * m, int button, Keypress_State state, Vec2 mpos) {
	for (auto e = elements.begin(); e != elements.end(); ++e) {
		if (CheckCollision((*e)->GetRect(), mpos)) {
			(*e)->Click(m, button, state, mpos);
		}
	}
}

void Menu::Update() {
	auto scr = GetActiveScreen();
	if (!scr) return;

	scr->Update(this);
	Keypress_State state = Event.GetKey(MOUSE1);
	if (state == KEY_UP) {
		Click(MOUSE1, state, Vec2(Event.mouse_x, Event.mouse_y));
	}
}

void Menu::Render() {
	auto scr = GetActiveScreen();
	if (!scr) return;
	scr->Render();
}

void Menu::Click(int button, Keypress_State state, Vec2 mpos) {
	auto scr = GetActiveScreen();
	if (!scr) return;
	scr->Click(this, button, state, mpos);
}

void Menu::AddScreen(const std::string& str, Menu_Screen& screen) {
	screens[str] = std::move(screen);	
}

Menu_Screen* Menu::GetActiveScreen() {
	if (active_menu.empty()) return nullptr;

	auto find = screens.find(active_menu);
	if (find == screens.end())
		return nullptr;
	else
		return &find->second;
}

void __PlayButton(Menu * menu) {
	Core.SetGamestate(GAMESTATE_GAME);
	Commands::CallCommand("map test");
}

void __QuitButton(Menu * menu) {
	Commands::CallCommand("quit");
}

void __KeyConfigScreenButton(Menu * menu) {
	menu->active_menu = "key_config";
}

void __MainScreenButton(Menu * menu) {
	menu->active_menu = "main";
}

void CreateCoreMenu(Menu * menu) {
	Menu_Screen main_screen;
	Menu_Screen key_config;

	/*
	main_screen.Add(std::make_unique<Menu_Elements::Decal>(
		Vec2(0.5,0.2), Vec2(-297.5,-48), Vec2(595,96), std::string("title.png")));

	main_screen.Add(std::make_unique<Menu_Elements::Button>(
		Vec2(0.5, 0.45), Vec2(-125,-22), Vec2(250,45), std::string("button.png"), std::string("button_hover.png"),
		std::string("Quakem"), std::string("DooM.ttf"),
		FONT_P32, (SDL_Color){0xff,0xff,0xff,0xff}, __PlayButton));
	main_screen.Add(std::make_unique<Menu_Elements::Button>(
		Vec2(0.5, 0.6), Vec2(-125,-22), Vec2(250,45), std::string("button.png"), std::string("button_hover.png"),
		std::string("Options"), std::string("DooM.ttf"),
		FONT_P32, (SDL_Color){0xff,0xff,0xff,0xff}, __KeyConfigScreenButton));
	main_screen.Add(std::make_unique<Menu_Elements::Button>(
		Vec2(0.5, 0.75), Vec2(-125,-22), Vec2(250,45), std::string("button.png"), std::string("button_hover.png"),
		std::string("Wuss Out"), std::string("DooM.ttf"),
		FONT_P32, (SDL_Color){0xff,0xff,0xff,0xff}, __QuitButton));

	Menu_Screen key_config;
	key_config.Add(std::make_unique<Menu_Elements::Button>(Vec2(0.0, 0.0), Vec2(25, 25), Vec2(200,45),
		std::string("button.png"), std::string("button_hover.png"),
		std::string("Back"), std::string("DooM.ttf"),
		FONT_P32, (SDL_Color){0xff,0xff,0xff,0xff}, __MainScreenButton));

	// move left key
	key_config.Add(std::make_unique<Menu_Elements::Label>(Vec2(0.25, 0.5), Vec2(-10, -100), 
		std::string("Move Left"), std::string("inconsolata.ttf"), FONT_P32, ALIGN_RIGHT,
		(SDL_Color){0xff,0xff,0xff,0xff}));
	key_config.Add(std::make_unique<Menu_Elements::KeyConfigButton>(Vec2(0.25, 0.5), Vec2(10, -100), Vec2(175, 40),
		std::string("button.png"), std::string("button_hover.png"), std::string("-"),
		std::string("inconsolata.ttf"), FONT_P32, (SDL_Color){0xff,0xff,0xff,0xff}, nullptr,
		std::string(PLAYER_LEFT)));

	// move right key
	key_config.Add(std::make_unique<Menu_Elements::Label>(Vec2(0.25, 0.5), Vec2(-10, -50), 
		std::string("Move Right"), std::string("inconsolata.ttf"), FONT_P32, ALIGN_RIGHT,
		(SDL_Color){0xff,0xff,0xff,0xff}));
	key_config.Add(std::make_unique<Menu_Elements::KeyConfigButton>(Vec2(0.25, 0.5), Vec2(10, -50), Vec2(175, 40),
		std::string("button.png"), std::string("button_hover.png"), std::string("-"),
		std::string("inconsolata.ttf"), FONT_P32, (SDL_Color){0xff,0xff,0xff,0xff}, nullptr,
		std::string(PLAYER_RIGHT)));

	// jump key
	key_config.Add(std::make_unique<Menu_Elements::Label>(Vec2(0.25, 0.5), Vec2(-10, 0), 
		std::string("Jump"), std::string("inconsolata.ttf"), FONT_P32, ALIGN_RIGHT,
		(SDL_Color){0xff,0xff,0xff,0xff}));
	key_config.Add(std::make_unique<Menu_Elements::KeyConfigButton>(Vec2(0.25, 0.5), Vec2(10, 0), Vec2(175, 40),
		std::string("button.png"), std::string("button_hover.png"), std::string("-"),
		std::string("inconsolata.ttf"), FONT_P32, (SDL_Color){0xff,0xff,0xff,0xff}, nullptr,
		std::string(PLAYER_JUMP)));

	// pistol
	key_config.Add(std::make_unique<Menu_Elements::Label>(Vec2(0.75, 0.5), Vec2(10, -200), 
		std::string("Pistol"), std::string("inconsolata.ttf"), FONT_P32, ALIGN_LEFT,
		(SDL_Color){0xff,0xff,0xff,0xff}));
	key_config.Add(std::make_unique<Menu_Elements::KeyConfigButton>(Vec2(0.75, 0.5), Vec2(-185, -200), Vec2(175, 40),
		std::string("button.png"), std::string("button_hover.png"), std::string("-"),
		std::string("inconsolata.ttf"), FONT_P32, (SDL_Color){0xff,0xff,0xff,0xff}, nullptr,
		std::string(PLAYER_WEP0)));
	// machine gun
	key_config.Add(std::make_unique<Menu_Elements::Label>(Vec2(0.75, 0.5), Vec2(10, -150), 
		std::string("Machine Gun"), std::string("inconsolata.ttf"), FONT_P32, ALIGN_LEFT,
		(SDL_Color){0xff,0xff,0xff,0xff}));
	key_config.Add(std::make_unique<Menu_Elements::KeyConfigButton>(Vec2(0.75, 0.5), Vec2(-185, -150), Vec2(175, 40),
		std::string("button.png"), std::string("button_hover.png"), std::string("-"),
		std::string("inconsolata.ttf"), FONT_P32, (SDL_Color){0xff,0xff,0xff,0xff}, nullptr,
		std::string(PLAYER_WEP1)));
	// rocket launcher
	key_config.Add(std::make_unique<Menu_Elements::Label>(Vec2(0.75, 0.5), Vec2(10, -100), 
		std::string("Rockets"), std::string("inconsolata.ttf"), FONT_P32, ALIGN_LEFT,
		(SDL_Color){0xff,0xff,0xff,0xff}));
	key_config.Add(std::make_unique<Menu_Elements::KeyConfigButton>(Vec2(0.75, 0.5), Vec2(-185, -100), Vec2(175, 40),
		std::string("button.png"), std::string("button_hover.png"), std::string("-"),
		std::string("inconsolata.ttf"), FONT_P32, (SDL_Color){0xff,0xff,0xff,0xff}, nullptr,
		std::string(PLAYER_WEP2)));
	// grenades
	key_config.Add(std::make_unique<Menu_Elements::Label>(Vec2(0.75, 0.5), Vec2(10, -50), 
		std::string("Grenades"), std::string("inconsolata.ttf"), FONT_P32, ALIGN_LEFT,
		(SDL_Color){0xff,0xff,0xff,0xff}));
	key_config.Add(std::make_unique<Menu_Elements::KeyConfigButton>(Vec2(0.75, 0.5), Vec2(-185, -50), Vec2(175, 40),
		std::string("button.png"), std::string("button_hover.png"), std::string("-"),
		std::string("inconsolata.ttf"), FONT_P32, (SDL_Color){0xff,0xff,0xff,0xff}, nullptr,
		std::string(PLAYER_WEP3)));*/

	menu->AddScreen("main", main_screen);
	menu->AddScreen("key_config", key_config);
	menu->active_menu = "main";
}

namespace Menu_Elements {

void Label::Update(Menu * m) { ; }
void Label::Render() {
	Rect r = GetRect();
	Renderer.RenderText(font, text, r.x, r.y, font_size, text_colour, text_align);
}

void Decal::Update(Menu * m) { ; }
void Decal::Click(Menu * m, int button, Keypress_State state, Vec2 mpos) { ; }
void Decal::Render() {
	Rect r = GetRect();
	Renderer.RenderTexture(img, nullptr, &r);
}

void Button::Update(Menu * m) {
	Rect r = GetRect();
	if (CheckCollision(r, Vec2(Event.mouse_x, Event.mouse_y))) {
		hovered = 1;
	} else hovered = 0;
}
void Button::Render() {
	Rect r = GetRect();
	if (hovered) {
		Renderer.RenderTexture(img_hover, nullptr, &r);
	} else {
		Renderer.RenderTexture(img, nullptr, &r);
	}
	Renderer.RenderText(font, str, r.x+r.w/2, r.y, font_size, font_color, ALIGN_MIDDLE);
}
void Button::Click(Menu * m, int button, Keypress_State state, Vec2 mpos) {
	if (Function) Function(m);
}

void KeyConfigButton::Update(Menu * m) {
	Rect r = GetRect();
	if (CheckCollision(r, Vec2(Event.mouse_x, Event.mouse_y))) {
		hovered = 1;
	} else hovered = 0;

	if (clicked) {
		if (!Event.Keypresses.empty()) {
			auto i = Event.Keypresses.begin();
			for (; i < Event.Keypresses.end(); ++i) {
				if (i->code == SDLK_BACKSPACE) {
					clicked = false;
					return;
				}

				if (i->state == KEY_DOWN) break;
			}

			if (i == Event.Keypresses.end()) return;
			Keys.SetBoundKey(bind, i->code);
			clicked = false;
		}
	} else {
		str = Keys.GetStringFromKey(Keys.GetBoundKey(bind)); 
	}
}

void KeyConfigButton::Render() {
	Rect r = GetRect();
	if (hovered || clicked) {
		Renderer.RenderTexture(img_hover, nullptr, &r);
	} else {
		Renderer.RenderTexture(img, nullptr, &r);
	}
	Renderer.RenderText(font, str, r.x+r.w/2, r.y, font_size, font_color, ALIGN_MIDDLE);
	
}

void KeyConfigButton::Click(Menu * m, int button, Keypress_State state, Vec2 mpos) {
	if (Function) Function(m);
	clicked = true;
}

};
