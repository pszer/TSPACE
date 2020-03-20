#include "Log.hpp"

using namespace Log;

const int Log::HISTORY_SIZE = 256;
std::vector<std::string> Log::History;

void Log::Add(const std::string& str) {
	const std::size_t lim = str.length();
	std::size_t b = 0;

	if (lim==1) {
		History.push_back(str);
	} else {

	// splits str into individual lines
	for (std::size_t i = 0; i < lim; ++i) {
		if (str[i] == '\n' || i == lim-1) {
			if (b != i) {
				if (i == lim - 1) ++i;
				History.push_back(str.substr(b, i-b));
			} else {
				History.push_back("");
			}
			b = i+1;
		}
	}

	}

	// shrink History to size
	if (History.size() > HISTORY_SIZE)
		History.erase(
		  History.begin(),
		  History.begin() + (History.size()-HISTORY_SIZE) );
}

void Log::Log(const std::string& str) {
	std::string msg = "[LOG]: " + str;
	Add(msg);
	std::cerr << msg << std::endl;
}

void Log::Error(const std::string& str) {
	std::string msg = "[ERR]: " + str;
	Add(msg);
	std::cerr << msg << std::endl;
}

void Log::ErrorSDL() {
	std::string msg = "[SDL]: " + std::string(SDL_GetError()); 
	Add(msg);
	std::cerr << msg << std::endl;
}

void Log::ErrorTTF() {
	std::string msg = "[TTF]: " + std::string(TTF_GetError());
	Add(msg);
	std::cerr << msg << std::endl;
}

void Log::ErrorIMG() {
	std::string msg = "[IMG]: " + std::string(IMG_GetError()); 
	Add(msg);
	std::cerr << msg << std::endl;
}

void Log::ErrorMix() {
	std::string msg = "[Mix]: " + std::string(Mix_GetError());
	Add(msg);
	std::cerr << msg << std::endl;
}
