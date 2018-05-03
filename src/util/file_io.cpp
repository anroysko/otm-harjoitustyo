#include <fstream>   // std::ifstream
#include <optional>  // std::optional
#include <sstream>   // std::stringstream
#include <string>    // std::string
#include <utility>   // std::move
#include <vector>    // std::vector

#include "error.h"  // makeError()
#include "file_io.h"

/// @file = file_io.cpp
/// Implements file_io.h

// reads contents of file to a string
std::optional<std::string> readFile(const std::string& file_path) {
	std::ifstream in;
	in.open(file_path);
	if (!in.is_open()) {
		makeError() << "Failed to open file " << file_path << '\n';
		return std::nullopt;
	}
	std::stringstream sstr;
	sstr << in.rdbuf();
	in.close();
	std::string res = sstr.str();
	return std::optional<std::string>{std::move(res)};
}

// Prompts the user to select a file, and returns that file's path.
std::optional<std::string> promptFile() {
	FILE* file = popen("zenity --file-selection --filename=./assets/levels --file-filter=*.txt", "r");
	char buffer[1024];
	std::stringstream ss;
	while (true) {
		char* res = fgets(buffer, 1024, file);
		if (res == nullptr) break;
		ss << buffer;
	}
	pclose(file);
	std::string res;
	ss >> res;
	if (res.size() == 0)
		return std::nullopt;
	else
		return std::optional<std::string>{std::move(res)};
}
