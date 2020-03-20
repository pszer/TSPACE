#include <iostream>
#include <filesystem>
#include <stdarg.h>

#include "Commands.hpp"
#include "Core.hpp"
#include "Game.hpp"
#include "Keys.hpp"

using namespace Commands;

std::map<std::string, CMD_FUNC> Commands::COMMANDS;

std::string _test(const std::vector<Argument>& args);

void Commands::Init() {
	COMMANDS["echo"] = _echo;
	COMMANDS["test"] = _test;
	COMMANDS["clear"] = _clear;
	COMMANDS["quit"] = _quit;
	COMMANDS["help"] = _help;
	COMMANDS["set"] = _set;
	COMMANDS["bind"] = _bind;
	COMMANDS["unbind"] = _unbind;
	COMMANDS["play_wav"] = _play_wav;
	COMMANDS["play_mus"] = _play_mus;
	COMMANDS["stop_mus"] = _stop_mus;
	COMMANDS["list_cvars"] = _list_cvars;
	COMMANDS["list_tex"] = _list_tex;
	COMMANDS["list_fnt"] = _list_fnt;
	COMMANDS["list_wav"] = _list_wav;
	COMMANDS["list_mus"] = _list_mus;
	COMMANDS["list_binds"] = _list_binds;
	COMMANDS["exec"] = _exec;
}

CMD_FUNC Commands::GetCommand(const std::string& id) {
	auto cmd = COMMANDS.find(id);
	if (cmd == COMMANDS.end()) return nullptr;
	return cmd->second;
}

std::string Commands::Execute(const struct Command& com) {
	auto cmd = GetCommand(com.command);
	if (cmd == nullptr) return "Command \"" + com.command + "\" not found";
	auto result = cmd(com.args);
	return result;
}

void Commands::CallCommand(const std::string& str) {
	struct Command com;
	if (!Parser::ParseCommand(str, com)) {
		Log::Add(Parser::ErrorMsg);
	} else {
		std::string result = Commands::Execute(com); 
		Log::Add(result);
	}
}

// COMMANDS

bool VAR_AND(int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i) {
        if (!va_arg(args, int)) return false;
    }   

    return true;
}

#define IS_NUM(x) ArgIsNum(args.at(x))
#define IS_NUM_RANGE(x, l, h) ArgIsNumRange(args.at(x), l, h)
#define IS_STRING(x) ArgIsString(args.at(x)) 
#define IS_STRING_ELEMENT(x, els) ArgIsStringElement(args.at(x), els)
#define OPT_IS_NUM(x) (args.size() <= x || IS_NUM(x))
#define OPT_IS_NUM_RANGE(x, l, h) (args.size() <= x || IS_NUM_RANGE(x, l, h))
#define OPT_IS_STRING(x) (args.size() <= x || IS_STRING(x))
#define OPT_IS_STRING_ELEMENT(x, els) (args.size() <= x || IS_STRING_ELEMENT(x, els))

#define TEST(len, n, err, ...) if (args.size() < len || !VAR_AND(n, __VA_ARGS__)) err;

#define OPT_EXISTS(x) args.size() > x
#define OPT_INT(x, def) OPT_EXISTS(x) ? args.at(x).ToInt() : def
#define OPT_FLOAT(x, def) OPT_EXISTS(x) ? args.at(x).ToFloat() : def
#define OPT_STRING(x, def) OPT_EXISTS(x) ? args.at(x).ToString() : def

std::string _test(const std::vector<Argument>& args) {
	std::vector<std::string> strs = {"foo", "bar"};
	TEST(0, 4, return "bad", OPT_IS_NUM(0), OPT_IS_STRING(1), OPT_IS_NUM_RANGE(2, 0.0, 2.0), OPT_IS_STRING_ELEMENT(3, strs));

	int a = OPT_INT(0, 1337);
	std::string str1 = OPT_STRING(1, "defstr");
	double b = OPT_FLOAT(2, 1.1);
	std::string str2 = OPT_STRING(3, "nonexist");

	std::stringstream ss;
	ss << "result: " << a << " " << str1 << " " << b << " " << str2;
	return ss.str();
}

namespace Commands {

std::string _echo(const std::vector<Argument>& args) {
	std::string str = "";
	for (auto arg : args) {
		str += arg.ToString();
	}
	return str;
}

std::string _clear(const std::vector<Argument>& args) {
	Log::History.clear();
	Core.Console.Reset();
	return "";
}

std::string _quit(const std::vector<Argument>& args) {
	Core.going = false;
	//SDL_Event ev;
	//ev.type = SDL_QUIT;
	//SDL_PushEvent(&ev);
	return "Quitting";
}

std::string _help(const std::vector<Argument>&) {
	std::string help_str = "Available commands:\n";
	for (auto c = COMMANDS.begin(); c != COMMANDS.end(); ++c) {
		help_str += c->first + "\n";
	}
	return help_str;
}

std::string _set(const std::vector<Argument>& args) {
	const std::string USE_MSG = "set cvar value - set a console variable to some value";
	if (args.size() < 2) return USE_MSG;
	if (args.at(0).type != ARG_CVAR) return USE_MSG;
	CVARS[args.at(0).str] = args.at(1);
	return "";
}

std::string _bind(const std::vector<Argument>& args) {
	const std::string USE_MSG = "bind keyname action";
	TEST(2, 2, return USE_MSG, IS_STRING(0), IS_STRING(1));
	std::string keyname = args.at(0).ToString(),
	            action = args.at(1).ToString();
	if (!Keys.SetBoundKeyFromString(action, keyname)) {
		return "Unknown key \"" + keyname + "\"";
	}
	return "";
}


std::string _unbind(const std::vector<Argument>& args) {
	const std::string USE_MSG = " unbind keyname";
	TEST(1, 1, return USE_MSG, IS_STRING(0));
	std::string keyname;
	keyname = args.at(0).ToString();
	if (!Keys.UnbindBoundKeyFromString(keyname)) {
		return "Unknown key \"" + keyname + "\"";
	}
	return "";
}

std::string _play_wav(const std::vector<Argument>& args) {
	const std::string USE_MSG = "play_wav chunkname";
	TEST(1, 1, return USE_MSG, IS_STRING(0));
	std::string chunk_str = args.at(0).ToString();

	Sound::PlaySound(chunk_str);

	return "";
}

std::string _play_mus(const std::vector<Argument>& args) {
	const std::string USE_MSG = "play_mus name [volume] [loops]";
	TEST(1, 3, return USE_MSG, IS_STRING(0), OPT_IS_NUM_RANGE(1, 0.0, 1.0), OPT_IS_NUM(2));

	std::string mus_str;
	float volume = OPT_FLOAT(1, 1.0);
	int loops = OPT_INT(2, 1);

	if (loops < 0) loops = 0;

	mus_str = args.at(0).ToString();
	Music.Play(mus_str, loops);
	Music.Volume(volume);
	return "";
}

template <typename T>
std::string __search__(const std::map<std::string, T>& v, std::string& find) {
	std::string str = "";
	bool filter = !find.empty();

	for (auto f : v) {
		std::string name = f.first;

		if (filter) {
			if (name.length() < find.length()) continue;

			for (int i = 0; i <= name.length() - find.length(); ++i) {
				if (name.substr(i, find.length()) == find) {
					str += name + "\n";
					continue;
				}
			}

		} else {
			str += name + "\n";
		}

	}

	return str;
}

std::string _stop_mus(const std::vector<Argument>&) {
	Music.Stop();
	return "";
}

std::string _list_tex(const std::vector<Argument>& args) {
	TEST(0, 1, return "bad string", OPT_IS_STRING(0));
	std::string find = OPT_STRING(0, "");
	return __search__(Media.textures, find);
}

std::string _list_fnt(const std::vector<Argument>& args) {
	TEST(0, 1, return "bad string", OPT_IS_STRING(0));
	std::string find = OPT_STRING(0, "");
	return __search__(Media.fonts, find);

}

std::string _list_wav(const std::vector<Argument>& args) {
	TEST(0, 1, return "bad string", OPT_IS_STRING(0));
	std::string find = OPT_STRING(0, "");
	return __search__(Media.chunks, find);
}

std::string _list_mus(const std::vector<Argument>& args) {
	TEST(0, 1, return "bad string", OPT_IS_STRING(0));
	std::string find = OPT_STRING(0, "");
	return __search__(Media.music, find);
}

std::string _list_cvars(const std::vector<Argument>& args) {
	TEST(0, 1, return "bad string", OPT_IS_STRING(0));
	std::string find = OPT_STRING(0, "");
	std::string str = "";
	bool filter = !find.empty();

	for (auto c : CVARS) {
		std::string name = c.first;

		if (filter) {
			if (name.length() < find.length()) continue;

			for (int i = 0; i <= name.length() - find.length(); ++i) {
				if (name.substr(i, find.length()) == find) {
					str += name + " = " + c.second.ToString() + "\n";
					continue;
				}
			}

		} else {
			str += name + " = " + c.second.ToString() + "\n";
		}

	}

	return str;
}

std::string _list_binds(const std::vector<Argument>& args) {
	TEST(0, 1, return "bad string", OPT_IS_STRING(0));
	std::string find = OPT_STRING(0, "");
	std::string str = "";
	bool filter = !find.empty();

	for (auto c : Keys.Bindings) {
		std::string name = c.first;

		if (filter) {
			if (name.length() < find.length()) continue;

			for (int i = 0; i <= name.length() - find.length(); ++i) {
				if (name.substr(i, find.length()) == find) {
					std::string codename(SDL_GetKeyName(c.second));
					str += name + " = " + codename + "\n";
					continue;
				}
			}

		} else {
			std::string codename = Keys.GetStringFromKey(c.second);
			str += name + " = " + codename + "\n";
		}

	}

	return str;
}

std::string _exec(const std::vector<Argument>& args) {
	const std::string USE_MSG = "exec path";
	TEST(1, 1, return USE_MSG, IS_STRING(0));
	std::string path = args.at(0).ToString();

	if (!Config::ExecFile(path))
		return "Config file \"" + path + "\" not found";
	else
		return "Loaded config file \"" + path + "\"";
}

}
