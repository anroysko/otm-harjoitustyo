#ifndef __UTIL_FILE_IO_H_
#define __UTIL_FILE_IO_H_

#include <optional>
#include <string>

// reads contents of file to a string
std::optional<std::string> readFile(std::string file_path);
std::string promptFile();
#endif  // __UTIL_FILE_IO_H_
