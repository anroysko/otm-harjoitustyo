#ifndef __UTIL_ERROR_H_
#define __UTIL_ERROR_H_

#include <iostream> // std::ostream

/// @file = error.h
/// Functions for printing errors, warnings and notes

/// Print line, function and file of the call to standard error, then return standard error.
/// Parameters are automatically generated with g++ builtins.
std::ostream& makeError(int line = __builtin_LINE(), const char* func = __builtin_FUNCTION(), const char* file = __builtin_FILE());

/// Print line, function and file of the call to standard error, then return standard error.
/// Parameters are automatically generated with g++ builtins.
std::ostream& makeWarning(int line = __builtin_LINE(), const char* func = __builtin_FUNCTION(), const char* file = __builtin_FILE());

/// Print line, function and file of the call to standard error, then return standard error.
/// Parameters are automatically generated with g++ builtins.
std::ostream& makeNote(int line = __builtin_LINE(), const char* func = __builtin_FUNCTION(), const char* file = __builtin_FILE());

#endif  // __UTIL_ERROR_H_
