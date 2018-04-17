#ifndef __GRAPHICS_UTIL_H_
#define __GRAPHICS_UTIL_H_

#include <string>   // std::string
#include <utility>  // std::move
#include <vector>   // std::vector

template <class T>
class Optional {
private:
	T object;
	bool exists;
	Optional() = default;

public:
	static Optional wrap(T&& obj) {
		Optional res;
		res.exists = true;
		res.object = obj;
		return res;
	}
	static Optional none() {
		Optional res;
		res.exists = false;
		return res;
	}
	T&& unwrap() {
		// assert(exists == true);
		exists = false;
		return std::move(object);
	}
	explicit operator bool() const {
		return exists;
	}
};

struct BMP {
	int width;
	int height;
	std::vector<unsigned char> data;
};

Optional<BMP> loadBMP(std::string file_path);
Optional<std::string> readFile(std::string file_path);

#endif  // __GRAPHICS_UTIL_H_
