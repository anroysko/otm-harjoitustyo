#include <iostream>	// std::cout
#include <string>	// std::string
#include <fstream>	// std::ifstream, std::ofstream
#include <sstream>	// std::stringstream
#include <vector>	// std::vector

#include <GL/glew.h>	// glew 2
#include <GLFW/glfw3.h>	// glfw 3

// Inits opengl and stuff
// returns null pointer if initialization fails
GLFWwindow* initGraphics() {
	// Initialize glfw
	if (!glfwInit()) {
		std::cout << "Failed to initialize glfw\n";
		return 0;
	}

	// GLFW window hints
	// Amount of samples for multisampling ( https://stackoverflow.com/questions/42848322/what-does-my-choice-of-glfw-samples-actually-do )
	glfwWindowHint(GLFW_SAMPLES, 4);
	// Give glfw opengl version to use (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Be forward compatible (remove deprecated functionality) ( http://www.glfw.org/docs/latest/window_guide.html )
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// Using opengl core profile ( https://www.khronos.org/opengl/wiki/OpenGL_Context#OpenGL_3.2_and_Profiles )
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Create "Windowed full screen" window
	// http://www.glfw.org/docs/latest/window_guide.html#window_windowed_full_screen
	// Retrieve screen width and height
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	if (monitor == 0) {
		std::cout << "Failed to retrieve monitor\n";
		return 0;
	}
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if (mode == 0) {
		std::cout << "Failed to retrieve monitor video mode\n";
		return 0;
	}
	int screen_width = mode->width;
	int screen_height = mode->height;
	// http://www.glfw.org/docs/latest/group__window.html#ga5c336fddf2cbb5b92f65f10fb6043344
	// Parameters: width, height, title, monitor, share
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "temporary testing", 0, 0);
	if (window == 0) {
		std::cout << "Failed to create window\n";
		return 0;
	}
	// Set current opengl context to our current window
	glfwMakeContextCurrent(window);
	
	// Initialize glew
	// Glew is a opengl loading library ( https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library )
	// You need to load opengl functions to use them. Doing it is laborous and platform dependent. Glew does this for you.
	// ( https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions )
	// Opengl function pointers are loaded to the current opengl context, so we do this after initializing our opengl context.
	glewExperimental = true; // Ensures that all extensions are considered ( http://glew.sourceforge.net/basic.html )
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW\n";
		return 0;
	}

	// Successful initialization
	return window;
}

// reads contents of file to a string
std::string readFile(std::string file_path) {
	std::ifstream in;
	in.open(file_path);
	std::stringstream sstr;
	sstr << in.rdbuf();
	in.close();
	return sstr.str();
}

void printInfoLog(std::vector<char> info_log, int length) {
	std::cout << "Info log:\n";
	for (int i = 0; i < length; ++i) std::cout << info_log[i];
	std::cout << '\n';
}

GLint compileShader(std::string& shader_path, GLenum shader_type) {
	GLint shader_id = glCreateShader(shader_type);

	std::string shader_src = readFile(shader_path);
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
		return -1;
	} else {
		return shader_id;
	}
}

// Input: id for vertex and fragment shaders
// Returns linked shader program id. If linking fails, returns -1
GLint linkProgram(GLint vertex_shader_id, GLint fragment_shader_id) {
	GLint program_id = glCreateProgram();
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
		glDeleteProgram(program_id);
		return -1;
	} else {
		return program_id;
	}
}

// Compiles a shader program from the given vertex and fragment shaders
// Returns id of the shader program. If compilation fails, returns -1
GLint makeProgram(std::string& vertex_shader_path, std::string& fragment_shader_path) {
	GLint vertex_shader_id = compileShader(vertex_shader_path, GL_VERTEX_SHADER);
	if (vertex_shader_id == -1) return -1;

	GLint fragment_shader_id = compileShader(fragment_shader_path, GL_FRAGMENT_SHADER);
	if (fragment_shader_id == -1) return -1;

	GLint program_id = linkProgram(vertex_shader_id, fragment_shader_id);
	
	// Delete shaders that don't matter anymore
	glDetachShader(vertex_shader_id, program_id);
	glDetachShader(fragment_shader_id, program_id);
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	
	if (program_id == -1) {
		std::cout << "From shaders with paths:\n";
		std::cout << "  " << vertex_shader_path << "\n";
		std::cout << "  " << fragment_shader_path << "\n";
		return -1;
	}
	return program_id;
}

int main() {
	GLFWwindow* window = initGraphics();
	if (window == 0) {
		std::cout << "Initializing graphics failed\n";
		return 1;
	}

	// Create a vertex array object
	// Vertex array object ( https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object )
	// A vertex array object stores the format of the vertex data, and the buffer objects with the vertex data arrays
	GLuint vao_id; // vertex array object id
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);

	// Load shaders
	std::string vertex_shader_path = "vertex_shader.glsl";
	std::string fragment_shader_path = "fragment_shader.glsl";
	GLint program_id = makeProgram(vertex_shader_path, fragment_shader_path);
	if (program_id == -1) {
		std::cout << "Failed to load shaders\n";
		return 2;
	}

	// Some data for testing
	std::vector<GLfloat> vertex_buffer_data = {
		-0.5f,	-0.5f,	0.0f,
		0.5f,	-0.5f,	0.0f,
		0.0f,	0.5f,	0.0f
	};
	GLuint vertex_buffer_id;
	glGenBuffers(1, &vertex_buffer_id);
	
	// Draw the testing data
	// Change active buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	// glBufferData ( https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml )
	// Parameters: target, size (in bytes), data, usage (expected usage pattern of the data store)
	// STREAM : The data store contents will be modified once and used at most a few times.
	// STATIC : The data store contents will be modified once and used many times.
	// DYNAMIC: The data store contents will be modified repeatedly and used many times.
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(GLfloat), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	while(true) {
		glClear( GL_COLOR_BUFFER_BIT );
		glUseProgram(program_id);
		// glVertexAttribPointer ( https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml )
		// Parameters: index (in vao), size (number of components), type (of each component),
		// 	normalized (should data be normalized when accessed), stride (byte offset between values in array), pointer (offset to first value)
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
		// glDrawArrays( https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml )
		// Parameters: type, first (vertex) to render, amount (of vertices) to render
		glDrawArrays(GL_TRIANGLES, 0, 9);
		
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
	}
	glfwTerminate();
	return 0;
}
