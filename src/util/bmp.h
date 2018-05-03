#ifndef __UTIL_BMP_H_
#define __UTIL_BMP_H_

#include <optional>  // std::optional
#include <vector>    // std::vector

/// @file = bmp.h
/// BMP -struct and related functions

/// Struct representing a bitmap-image.
struct BMP {
	/// Image width
	int width;
	/// Image height
	int height;
	/// Pixel data
	std::vector<unsigned char> data;
};

/// Loads a BMP from the given path.
/// Returns std::nullopt if loading the image fails.
std::optional<BMP> loadBMP(const std::string& file_path);

#endif  // __UTIL_BMP_H_
