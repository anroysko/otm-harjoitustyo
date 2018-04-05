#ifndef __GRAPHICS_SHADER_H_
#define __GRAPHICS_SHADER_H_

#include <string>
#include <GL/glew.h>
#include "util.h"

Optional<GLuint> makeProgram(std::string& vertex_shader_path, std::string& fragment_shader_path);

#endif // __GRAPHICS_SHADER_H_
