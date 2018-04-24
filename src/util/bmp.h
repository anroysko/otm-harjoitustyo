#ifndef __UTIL_BMP_H_
#define __UTIL_BMP_H_

#include <vector>
#include <string>
#include <optional>

struct BMP {
	int width;
	int height;
	std::vector<unsigned char> data;
};

std::optional<BMP> loadBMP(std::string file_path);

#endif // __UTIL_BMP_H_
