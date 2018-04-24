#include <fstream>   // std::ifstream
#include <optional>  // std::optional
#include <utility>   // std::move
#include <vector>    // std::vector

#include "bmp.h"
#include "error.h"

std::optional<BMP> loadBMP(std::string file_path) {
	std::vector<unsigned char> header(54);
	std::ifstream fin;
	fin.open(file_path, std::ios::in | std::ios::binary);
	if (!fin.is_open()) {
		makeError() << "Failed to open bmp file " << file_path << '\n';
		return std::nullopt;
	}
	fin.read(reinterpret_cast<char*>(&header[0]), 54);
	if (!fin.good()) {
		makeError() << "Failed to read bmp header from file " << file_path << '\n';
		fin.close();
		return std::nullopt;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		makeError() << "Not a bmp file: " << file_path << '\n';
		fin.close();
		return std::nullopt;
	}

	// Bit tricks, since that's how BMP's are represented
	unsigned int data_pos = *(int*)&(header[0x0A]);
	unsigned int width = *(int*)&(header[0x12]);
	unsigned int height = *(int*)&(header[0x16]);
	unsigned int image_size = *(int*)&(header[0x22]);
	if (image_size == 0) image_size = width * height * 3;
	if (data_pos == 0) data_pos = 54;

	std::vector<unsigned char> data(image_size);
	fin.seekg(data_pos);
	fin.read(reinterpret_cast<char*>(&data[0]), image_size);
	if (!fin.good()) {
		makeError() << "Failed to read bmp data from file " << file_path << '\n';
		fin.close();
		return std::nullopt;
	}
	fin.close();

	BMP res;
	res.width = width;
	res.height = height;
	res.data = std::move(data);
	return std::optional<BMP>{std::move(res)};
}
