#include <iostream>	// std::cout
#include <fstream>	// std::ifstream
#include <sstream>	// std::stringstream
#include <vector>	// std::vector
#include <utility>	// std::move

#include "util.h"

Optional<BMP> loadBMP(std::string file_path) {
	std::vector<unsigned char> header (54);
	std::ifstream fin;
	fin.open(file_path, std::ios::in | std::ios::binary);
	if (! fin.is_open()) {
		std::cout << "Failed to open bmp file " << file_path << '\n';
		return Optional<BMP>::none();
	}
	fin.read(reinterpret_cast<char*>(&header[0]), 54);
	if (! fin.good()) {
		std::cout << "Failed to read bmp header from file " << file_path << '\n';
		fin.close();
		return Optional<BMP>::none();
	}

	if (header[0] != 'B' || header[1] != 'M') {
		std::cout << "Not a bmp file: " << file_path << '\n';
		fin.close();
		return Optional<BMP>::none();
	}

	unsigned int data_pos   = *(int*)&(header[0x0A]);
	unsigned int width      = *(int*)&(header[0x12]);
	unsigned int height     = *(int*)&(header[0x16]);
	unsigned int image_size = *(int*)&(header[0x22]);
	if (image_size == 0) image_size = width * height * 3;
	if (data_pos == 0) data_pos = 54;

	std::vector<unsigned char> data (image_size);
	fin.seekg(data_pos);
	fin.read(reinterpret_cast<char*>(&data[0]), image_size);
	if (! fin.good()) {
		std::cout << "Failed to read bmp data from file " << file_path << '\n';
		fin.close();
		return Optional<BMP>::none();	
	}
	fin.close();

	BMP res;
	res.width = width;
	res.height = height;
	res.data = std::move(data);
	return Optional<BMP>::wrap(std::move(res));
}

// reads contents of file to a string
Optional<std::string> readFile(std::string file_path) {
	std::ifstream in;
	in.open(file_path);
	if (!in.is_open()) {
		std::cout << "Failed to open file " << file_path << '\n';
		return Optional<std::string>::none();
	}
	std::stringstream sstr;
	sstr << in.rdbuf();
	in.close();
	std::string res = sstr.str();
	return Optional<std::string>::wrap(std::move(res));
}
