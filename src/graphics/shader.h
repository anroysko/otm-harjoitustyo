#ifndef __GRAPHICS_SHADER_H_
#define __GRAPHICS_SHADER_H_

#include <GL/glew.h>
#include <string>
#include "util.h"

Optional<GLuint> makeProgram(std::string& vertex_shader_path, std::string& fragment_shader_path);

#endif  // __GRAPHICS_SHADER_H_
