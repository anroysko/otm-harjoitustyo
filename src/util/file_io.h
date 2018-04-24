#ifndef __UTIL_FILE_IO_H_
#define __UTIL_FILE_IO_H_

#include <string>
#include <optional>


// reads contents of file to a string
std::optional<std::string> readFile(std::string file_path);

#endif // __UTIL_FILE_IO_H_
