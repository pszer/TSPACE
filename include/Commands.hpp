#pragma once

#include "Cmd.hpp"

// COMMANDS
using CMD_FUNC = std::string(*)(const std::vector<Argument>&);

namespace Commands {

	extern std::map<std::string, CMD_FUNC> COMMANDS;

	void Init();

	CMD_FUNC GetCommand(const std::string& id); // returns nullptr if no command found
	std::string Execute(const struct Command& com);
	void CallCommand(const std::string& str);

	// echos its arguments as strings
	// echo {args}
	std::string _echo(const std::vector<Argument>&); 

	// clears log history
	std::string _clear(const std::vector<Argument>&);

	// prints all command names
	std::string _help(const std::vector<Argument>&);

	// quits game
	std::string _quit(const std::vector<Argument>&);

	// sets a consolve variable to some value
	// set $var val
	std::string _set(const std::vector<Argument>&);

	// binds action to a key
	// bind keyname action
	std::string _bind(const std::vector<Argument>&);

	// unbinds a key
	// unbind keyname
	std::string _unbind(const std::vector<Argument>&);

	// lists binds
	// list_binds [search]
	std::string _list_binds(const std::vector<Argument>& args);

	// lists all set cvars
	std::string _list_cvars(const std::vector<Argument>&);

	// plays loaded audio chunk
	// play_wav name
	std::string _play_wav(const std::vector<Argument>&);

	// plays loaded music file
	// play_mus name [volume] [loops]
	std::string _play_mus(const std::vector<Argument>&);

	// stops music
	std::string _stop_mus(const std::vector<Argument>&);

	// lists loaded textures
	// list_tex [search]
	std::string _list_tex(const std::vector<Argument>&);

	// lists loaded fonts
	// list_fnt [search]
	std::string _list_fnt(const std::vector<Argument>&);

	// lists loaded sfx chunks
	// list_wav [search]
	std::string _list_wav(const std::vector<Argument>&);

	// lists loaded music files
	// list_mus [search]
	std::string _list_mus(const std::vector<Argument>&);

	// executes config file
	// exec file
	std::string _exec(const std::vector<Argument>&);
};
