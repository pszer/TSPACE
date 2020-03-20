#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Commands.hpp"
#include "Keys.hpp"

#define CONFIG_PATH "config"

namespace Config {

	// config values are stored in CVARs

	// maxfps 121
	void DefaultValues();

	extern std::vector<std::string> open_files;

	// returns 1 on success, 0 on error
	int ExecFile(const std::string& filename);

	std::string BindToConfigLine(const std::string& bind_name, SDL_Keycode kc);
	std::string CVarToConfigLine(const std::string& cvar, Argument value);

	std::string CurrentConfig(void);
	void WriteConfig(const std::string& fname);
};
