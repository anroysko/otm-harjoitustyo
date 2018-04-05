#include <string> 	// std::string
#include <iostream>	// std::cout
#include <fstream>	// std::ifstream
#include <sstream>	// std::stringstream
#include <vector> 	// std::vector
#include <GL/glew.h>	// Opengl functions
#include "util.h"	// Optional

#include "shader.h"
// #include <GLFW/glfw3.h> //

void printInfoLog(std::vector<char> info_log, int length) {
	std::cout << "Info log:\n";
	for (int i = 0; i < length; ++i) std::cout << info_log[i];
	std::cout << '\n';
}

Optional<GLuint> compileShader(std::string& shader_path, GLenum shader_type) {
	GLuint shader_id = glCreateShader(shader_type);

	Optional<std::string> tmp = readFile(shader_path);
	if (! tmp) {
		std::cout << "Failed compilation of shader with path \"" << shader_path << "\"\n";
		return Optional<GLuint>::none();
	}
	std::string shader_src = tmp.unwrap();

	const char* src_cstr = shader_src.c_str();
	glShaderSource(shader_id, 1, &src_cstr, 0);
	glCompileShader(shader_id);

	// DEBUG
	GLint result = GL_FALSE;
	int info_log_length;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 1) {
		if (result == GL_FALSE) {
			std::cout << "Failed compilation of shader with path \"" << shader_path << "\"\n";
		} else {
			std::cout << "Info log nonempty for shader with path \"" << shader_path << "\"\n";
		}
		std::vector<char> error_message (info_log_length + 1);
		glGetShaderInfoLog(shader_id, info_log_length, 0, &error_message[0]);
		printInfoLog(error_message, info_log_length);
	}

	if (result == GL_FALSE) {
		glDeleteShader(shader_id);
		return Optional<GLuint>::none();
	} else {
		return Optional<GLuint>::wrap(std::move(shader_id));
	}
}

// Input: id for vertex and fragment shaders
// Returns linked shader program id. If linking fails, returns -1
Optional<GLuint> linkProgram(GLuint vertex_shader_id, GLuint fragment_shader_id) {
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);
	
	// DEBUG
	GLint result = GL_FALSE;
	int info_log_length;
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 1) {
		if (result == GL_FALSE) {
			std::cout << "Linking program failed\n";
		} else {
			std::cout << "Program info log nonempty\n";
		}
		std::vector<char> error_message (info_log_length + 1);
		glGetProgramInfoLog(program_id, info_log_length, 0, &error_message[0]);
		printInfoLog(error_message, info_log_length);
	}

	if (result == GL_FALSE) {
		glDetachShader(program_id, vertex_shader_id);
		glDetachShader(program_id, fragment_shader_id);
		glDeleteProgram(program_id);
		return Optional<GLuint>::none();
	} else {
		return Optional<GLuint>::wrap(std::move(program_id));
	}
}

// Compiles a shader program from the given vertex and fragment shaders
// Returns id of the shader program. If compilation fails, returns -1
Optional<GLuint> makeProgram(std::string& vertex_shader_path, std::string& fragment_shader_path) {
	Optional<GLuint> tmp;
	
	tmp = compileShader(vertex_shader_path, GL_VERTEX_SHADER);
	if (! tmp) return Optional<GLuint>::none();
	GLuint vertex_shader_id = tmp.unwrap();

	tmp = compileShader(fragment_shader_path, GL_FRAGMENT_SHADER);
	if (! tmp) {
		glDeleteShader(vertex_shader_id);
		return Optional<GLuint>::none();
	}
	GLuint fragment_shader_id = tmp.unwrap();

	tmp = linkProgram(vertex_shader_id, fragment_shader_id);
	if (! tmp) {
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
		std::cout << "Failed to link shader from shaders with paths:\n";
		std::cout << "  " << vertex_shader_path << "\n";
		std::cout << "  " << fragment_shader_path << "\n";
		return Optional<GLuint>::none();
	}
	GLuint program_id = tmp.unwrap();
	
	// Detach and delete shaders that don't matter anymore
	glDetachShader(program_id, fragment_shader_id);
	glDetachShader(program_id, vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	glDeleteShader(vertex_shader_id);

	return Optional<GLuint>::wrap(std::move(program_id));
}
