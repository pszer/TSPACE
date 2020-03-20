#include "Filesys.hpp"

namespace fs = std::filesystem;

std::vector<std::string> FilesInDir(const std::string& dir_name) {
	std::vector<std::string> result;
	fs::recursive_directory_iterator dir(dir_name);
	
	for (auto& f : dir) {
		if (fs::is_regular_file(f)) {
			result.push_back(f.path());
		}
	}

	return result;
}

std::vector<std::string> FilesInSubdirs(const std::string& base_dir, const std::vector<std::string>& sub_dirs) {
	std::vector<std::string> result;
	fs::directory_iterator dir(base_dir);

	/*
	 *
	 * do this
	*/

	for (auto f : dir) {
		if (fs::is_directory(f)) {
			std::string fname = f.path().filename();
			auto sdir = sub_dirs.begin();
			for (;sdir != sub_dirs.end(); ++sdir) {
				if (*sdir == fname)
					break;
			}

			if (sdir == sub_dirs.end()) continue;

			auto files = FilesInDir(f.path());
			result.insert(result.begin(), files.begin(), files.end());
		}
	}

	return result;
}

std::string CleanFilename(const std::string& file) {
	if (file.length() < 2) return file;
	if (file.substr(0,2) == "./")
		return file.substr(2);
	return file;
}

std::string StripFolder(const std::string& file) {
	std::string str = file;
	for (auto c = str.begin(); c != str.end(); ++c) {
		if (*c == '/') {
			str.erase(str.begin(), c+1);
			break;
		}
	}
	return str;
}
