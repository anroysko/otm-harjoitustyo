#ifndef __UTIL_ERROR_H_
#define __UTIL_ERROR_H_

#include <iostream>

inline std::ostream& makeError(int line = __builtin_LINE(), const char* func = __builtin_FUNCTION(), const char* file = __builtin_FILE()) {
	std::cerr << file << ":" << func << ":" << line << ": error: ";
	return std::cerr;
}
inline std::ostream& makeWarning(int line = __builtin_LINE(), const char* func = __builtin_FUNCTION(), const char* file = __builtin_FILE()) {
	std::cerr << file << ":" << func << ":" << line << ": warning: ";
	return std::cerr;
}
inline std::ostream& makeNote(int line = __builtin_LINE(), const char* func = __builtin_FUNCTION(), const char* file = __builtin_FILE()) {
	std::cerr << file << ":" << func << ":" << line << ": note: ";
	return std::cerr;
}

#endif  // __UTIL_ERROR_H_
