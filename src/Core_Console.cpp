#include "Core.hpp"

void Core::Console::Update() {
	if (open) {
		Event.EnableTextInput();
		Event.EnableRepeatedKeys();
	} else {
		Event.DisableTextInput();
		Event.DisableRepeatedKeys();
	}
}

void Core::Console::Render() {
	if (!open) return;

	Font * font_struct = Media.GetFont(font);
	if (font_struct == nullptr) return;
	TTF_Font * ttf_font = font_struct->GetTTFSize(FONT_P16);

	Rect bg_rect = { 0 , 0 , (double)Event.win_w , (double)Event.win_h - bottom_offset };
	SDL_Rect rect = { 5, Event.win_h - bottom_offset, 0, 0 };
	TTF_SizeText(ttf_font, " ", &rect.w, &rect.h);
	rect.y -= rect.h*2 + 5;

	// background
	SDL_Rect sdl_bg_rect = bg_rect.ToSDLRect();
	Renderer.SetColor(bg);
	SDL_RenderFillRect(Renderer.renderer, &sdl_bg_rect);

	// render text
	int i = Log::History.size() - 1 - page_scroll;
	while (rect.y >= 0 && i >= 0) {
		const std::string& str = Log::History.at(i);

		SDL_Rect r = Renderer.RenderText(font, str, rect.x, rect.y, FONT_P16, fg, ALIGN_LEFT);
		rect.w = r.w;
		rect.h = r.h;

		rect.y -= (rect.h+1);
		--i;
	}

	int width;
	// render text being typed
	TTF_SizeText(ttf_font, text.c_str(), &rect.w, &rect.h);
	TTF_SizeText(ttf_font, "H", &width, nullptr);
	rect.x = 2;
	rect.y = Event.win_h - bottom_offset - rect.h - 2;
	Renderer.RenderText(ttf_font, text, rect.x, rect.y, fg);

	rect.x += (cursor) * width + 1;
	rect.w = 2;
	Renderer.SetColor(fg);
	SDL_RenderFillRect(Renderer.renderer, &rect);
}

void Core::Console::HandleKeypresses() {
	if (Event.GetKey(SDLK_BACKQUOTE) == KEY_DOWN)
		Toggle();

	if (!open) return;

	if (Event.GetKey(SDLK_LEFT) == KEY_DOWN)
		Left();
	if (Event.GetKey(SDLK_RIGHT) == KEY_DOWN)
		Right();
	if (Event.GetKey(SDLK_UP) == KEY_DOWN)
		Up();
	if (Event.GetKey(SDLK_DOWN) == KEY_DOWN)
		Down();

	if (Event.GetKey(SDLK_BACKSPACE) == KEY_DOWN)
		Backspace();
	if (Event.GetKey(SDLK_RETURN) == KEY_DOWN)
		Enter();

	if (Event.GetKey(SDLK_PAGEUP) == KEY_DOWN)
		PageUp();
	if (Event.GetKey(SDLK_PAGEDOWN) == KEY_DOWN)
		PageDown();

	if (Event.GetKey(SDLK_END) == KEY_DOWN)
		End();
	if (Event.GetKey(SDLK_HOME) == KEY_DOWN)
		Home();
}

void Core::Console::Toggle() {
	if (!GetCVarInt("console")) return;

	if (open)
		Event.DisableTextInput();
	else
		Event.EnableTextInput();
	open = !open;
}

void Core::Console::Enter() {
	if (!open) return;

	if (!text.empty()) {
		Log::Add(text);
		Commands::CallCommand(text);

		CommandHistory.insert(CommandHistory.begin(), text);
		if (CommandHistory.size() > MAX_COMMAND_HISTORY)
			CommandHistory.pop_back();
	} else {
		Log::Add(" ");
	}

	Reset();
}

void Core::Console::Reset() {
	text = "";
	cursor = 0;
	history_scroll = 0;
	page_scroll = 0;
}

void Core::Console::Backspace() {
	if (!open) return;
	if (!text.empty()) {
		int _cursor = cursor-1;
		if (cursor == 0) _cursor = 0;
		text.erase(text.begin() + _cursor);
		Left();
	}
}

void Core::Console::TextInput(const std::string& input) {
	if (!open) return;
	if (input == "`") return;
	text = text.insert(cursor, input);
	cursor += input.length();
}

void Core::Console::Left() {
	if (!open) return;
	cursor--;
	if (cursor < 0) cursor = 0;
}

void Core::Console::Right() {
	if (!open) return;
	cursor++;
	if (cursor > text.length()) cursor = text.length();
}

void Core::Console::Up() {
	if (!open) return;
	history_scroll++;
	if (history_scroll > CommandHistory.size())
		history_scroll = CommandHistory.size();
	SetHistoryText();
}

void Core::Console::Down() {
	if (!open) return;
	history_scroll--;
	if (history_scroll < 0) history_scroll = 0;
	SetHistoryText();
}

void Core::Console::PageUp() {
	if (Log::History.size() < page_scroll_step) return;
	page_scroll += page_scroll_step;
	if (page_scroll > Log::History.size() - page_scroll_step)
		page_scroll = Log::History.size() - page_scroll_step;
}

void Core::Console::PageDown() {
	page_scroll -= page_scroll_step;
	if (page_scroll < 0) page_scroll = 0;
}

void Core::Console::Home() {
	cursor = 0;
}

void Core::Console::End() {
	cursor = text.length();
}

void Core::Console::SetHistoryText() {
	std::size_t old_length = text.length();
	if (history_scroll == 0)
		text = "";
	else
		text = CommandHistory.at(history_scroll-1);

	if (cursor == old_length)
		cursor = text.length();
	else
		cursor = std::min((int)cursor, (int)text.length());
}
