#include "error.h"   // Implement functions for
#include <iostream>  // std::cerr, std::ostream

/// @file = error.cpp
/// Implements error.h

std::ostream& makeError(int line, const char* func, const char* file) {
	std::cerr << file << ":" << func << ":" << line << ": error: ";
	return std::cerr;
}

std::ostream& makeWarning(int line, const char* func, const char* file) {
	std::cerr << file << ":" << func << ":" << line << ": warning: ";
	return std::cerr;
}

std::ostream& makeNote(int line, const char* func, const char* file) {
	std::cerr << file << ":" << func << ":" << line << ": note: ";
	return std::cerr;
}
