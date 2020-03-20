#include "Core.hpp"

#include <iostream>

int main(int argc, char ** argv) {
	Config::DefaultValues();

	if (Core.Init()) {
		Log::Error("QUITTING");
		Core.Quit();
		return -1;
	} else {
		Core.MainLoop();
		Config::WriteConfig("config");
		Core.Quit();
		return 0;
	}
}
