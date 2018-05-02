#ifndef __UTIL_FILE_IO_H_
#define __UTIL_FILE_IO_H_

#include <optional>
#include <string>

/// @file=file_io.h
/// Functions for file I/O.

/// reads contents of file to a string.
/// returns std::nullopt if opening the file or reading it failed.
std::optional<std::string> readFile(const std::string& file_path);
/// Propts the user to select a file, and returns its path.
/// returns std::nullopt if the user closed the prompt without selecting anything
std::optional<std::string> promptFile();
#endif  // __UTIL_FILE_IO_H_
