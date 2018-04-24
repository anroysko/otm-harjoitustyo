#include <GL/glew.h>  // Opengl functions
#include <optional>   // std::optional
#include <sstream>    // std::stringstream
#include <string>     // std::string
#include <vector>     // std::vector

#include "./../util/error.h"    // makeError
#include "./../util/file_io.h"  // readFile
#include "shader.h"

std::string getInfoLog(std::vector<char> info_log, int length) {
	std::stringstream res;
	res << "Info log:\n";
	for (int i = 0; i < length; ++i) res << info_log[i];
	return res.str();
}

std::optional<GLuint> compileShader(std::string& shader_path, GLenum shader_type) {
	GLuint shader_id = glCreateShader(shader_type);

	std::optional<std::string> tmp = readFile(shader_path);
	if (!tmp) {
		makeError() << "Failed compilation of shader with path \"" << shader_path << "\"\n";
		return std::nullopt;
	}
	std::string shader_src = std::move(tmp.value());

	const char* src_cstr = shader_src.c_str();
	glShaderSource(shader_id, 1, &src_cstr, 0);
	glCompileShader(shader_id);

	// DEBUG
	GLint result = GL_FALSE;
	int info_log_length;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 1) {
		std::vector<char> error_message(info_log_length + 1);
		glGetShaderInfoLog(shader_id, info_log_length, 0, &error_message[0]);
		std::string log = getInfoLog(error_message, info_log_length);

		if (result == GL_FALSE) {
			makeError() << "Failed compilation of shader with path \"" << shader_path << "\"\n"
				    << log << "\n";
		} else {
			makeNote() << "Info log nonempty for shader with path \"" << shader_path << "\"\n"
				   << log << "\n";
		}
	}

	if (result == GL_FALSE) {
		glDeleteShader(shader_id);
		return std::nullopt;
	} else {
		return std::optional<GLuint>{std::move(shader_id)};
	}
}

// Input: id for vertex and fragment shaders
// Returns linked shader program id. If linking fails, returns -1
std::optional<GLuint> linkProgram(GLuint vertex_shader_id, GLuint fragment_shader_id) {
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
		std::vector<char> error_message(info_log_length + 1);
		glGetProgramInfoLog(program_id, info_log_length, 0, &error_message[0]);
		std::string log = getInfoLog(error_message, info_log_length);

		if (result == GL_FALSE) {
			makeError() << "Linking program failed\n"
				    << log << "\n";
		} else {
			makeNote() << "Linking info log nonempty\n"
				   << log << "\n";
		}
	}

	if (result == GL_FALSE) {
		glDetachShader(program_id, vertex_shader_id);
		glDetachShader(program_id, fragment_shader_id);
		glDeleteProgram(program_id);
		return std::nullopt;
	} else {
		return std::optional<GLuint>{std::move(program_id)};
	}
}

// Compiles a shader program from the given vertex and fragment shaders
// Returns id of the shader program. If compilation fails, returns -1
std::optional<GLuint> makeProgram(std::string& vertex_shader_path, std::string& fragment_shader_path) {
	std::optional<GLuint> tmp = std::nullopt;
	tmp = compileShader(vertex_shader_path, GL_VERTEX_SHADER);
	if (!tmp) return std::nullopt;
	GLuint vertex_shader_id = std::move(tmp.value());

	tmp = compileShader(fragment_shader_path, GL_FRAGMENT_SHADER);
	if (!tmp) {
		glDeleteShader(vertex_shader_id);
		return std::nullopt;
	}
	GLuint fragment_shader_id = std::move(tmp.value());
	;

	tmp = linkProgram(vertex_shader_id, fragment_shader_id);
	if (!tmp) {
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
		makeError() << "Failed to link shader from shaders with paths:\n"
			    << "  " << vertex_shader_path << "\n"
			    << "  " << fragment_shader_path << "\n";
		return std::nullopt;
	}
	GLuint program_id = std::move(tmp.value());

	// Detach and delete shaders that don't matter anymore
	glDetachShader(program_id, fragment_shader_id);
	glDetachShader(program_id, vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	glDeleteShader(vertex_shader_id);

	return std::optional<GLuint>{std::move(program_id)};
}
