#include <fstream>   // std::ifstream
#include <optional>  // std::optional
#include <utility>   // std::move
#include <vector>    // std::vector

#include "bmp.h"
#include "error.h"

/// @file = bmp.cpp
/// Implements bmp.h

std::optional<BMP> loadBMP(const std::string& file_path) {
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

	// BMP files start with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		makeError() << "Not a bmp file: " << file_path << '\n';
		fin.close();
		return std::nullopt;
	}

	// Read ints from a char array. This is not my fault, blame bitmap for being like this
	unsigned int data_pos = *(int*)&(header[0x0A]);    // Start of the image pixel data
	unsigned int width = *(int*)&(header[0x12]);       // Width of the image
	unsigned int height = *(int*)&(header[0x16]);      // Height of the image
	unsigned int image_size = *(int*)&(header[0x22]);  // Image data size
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
	res.data = std::move(data);  // Avoid copying
	return std::optional<BMP>{std::move(res)};
}
