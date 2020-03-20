#include "Config.hpp"

std::vector<std::string> Config::open_files;

#define VAL(a,b) CVARS[a]=Argument(b)
#define STRVAL(a,b) CVARS[a]=Argument(ARG_STRING,b)
void Config::DefaultValues() {
	VAL("maxfps",121ll);
	VAL("showfps",1ll);
	VAL("console",1ll);
}

int Config::ExecFile(const std::string& filename) {
	for (auto& s : Config::open_files)
		if (s == filename) return 1;

	std::ifstream file(filename);
	if (!file) return 0;

	open_files.push_back(filename);

	std::string line;
	while (!file.eof()) {
		std::getline(file, line);
		if (line.empty()) continue;
		Commands::CallCommand(line);
	}

	for (auto s = Config::open_files.begin(); s != Config::open_files.end(); ++s)
	{
		if (*s == filename) {
			Config::open_files.erase(s);
			break;
		}
	}

	return 1;
}

std::string Config::BindToConfigLine(const std::string& bind_name, SDL_Keycode kc) {
	return "bind " + Keys.GetStringFromKey(kc) + " \"" + bind_name + "\"";
}

std::string Config::CVarToConfigLine(const std::string& cvar, Argument value) {
	std::string val_str;
	if (value.type == ARG_STRING)
		val_str = "\"" + value.ToString() + "\"";
	else if (value.type == ARG_CVAR)
		val_str = "$" + value.str;
	else
		val_str = value.ToString();
	return "set $" + cvar + " " + val_str;
}

std::string Config::CurrentConfig(void) {
	std::stringstream ss;
	for (auto& bind : Keys.Bindings)
		ss << Config::BindToConfigLine(bind.first, bind.second) << "\n";
	for (auto& cvar : CVARS)
		ss << Config::CVarToConfigLine(cvar.first, cvar.second) << "\n";
	return ss.str();
}

void Config::WriteConfig(const std::string& fname) {
	std::ofstream f(fname);
	if (!f) return;
	f << Config::CurrentConfig();
	f.close();
}
