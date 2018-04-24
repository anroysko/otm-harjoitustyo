#include <fstream>   // std::ifstream
#include <sstream>   // std::stringstream
#include <utility>   // std::move
#include <vector>    // std::vector

#include <optional>
#include "error.h"
#include "file_io.h"

// reads contents of file to a string
std::optional<std::string> readFile(std::string file_path) {
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


