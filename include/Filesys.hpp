#pragma once

#include <filesystem>
#include <vector>
#include <string>

std::vector<std::string> FilesInDir(const std::string& dir);
std::vector<std::string> FilesInSubdirs(const std::string& base_dir, const std::vector<std::string>& sub_dirs);

// gets rid of "./" in a filename
// "./img/test" -> "img/test"
std::string CleanFilename(const std::string& file);

// gets rid of base folder in filename(if one)
// "img/dev/test" -> "dev/test"
std::string StripFolder(const std::string& file);
