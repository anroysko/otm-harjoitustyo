#ifndef __GRAPHICS_SHADER_H_
#define __GRAPHICS_SHADER_H_

/// @file=shader.h
/// Functions for compiling shaders and shader programs.

#include <GL/glew.h>  // Opengl functions
#include <optional>   // std::optional
#include <string>     // std::string

/// Creates a shader program, compiling shaders from the given paths.
/// Returns std::nullopt if compiling the shader failed
std::optional<GLuint> makeProgram(const std::string& vertex_shader_path, const std::string& fragment_shader_path);

#endif  // __GRAPHICS_SHADER_H_
