#include <iostream>	// std::cout
#include <string>	// std::string
#include <fstream>	// std::ifstream, std::ofstream
#include <sstream>	// std::stringstream
#include <vector>	// std::vector

#include <assert.h>	// tmp
#include <stdio.h>	// stuff

#include <GL/glew.h>	// glew 2
#include <GLFW/glfw3.h>	// glfw 3

// GAME LOGIC
// Struct that contains everything needed to draw the time step
// We make one of these every time step, so like three times a second
struct DrawData {
	int width;
	int height;
	int px;
	int py;
	int pdx;
	int pdy;
	std::vector<int> dx;
	std::vector<int> dy;
	std::vector<int> tex;
};

// END GAME LOGIC

// DRAW LOGIC
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

// Read a bmp
GLint loadBMP(std::string file_path) {
	std::vector<unsigned char> header (54);
	std::ifstream fin;
	fin.open(file_path, std::ios::in | std::ios::binary);
	if (! fin.is_open()) {
		std::cout << "Failed to open bmp file " << file_path << '\n';
		return -1;
	}
	fin.read(reinterpret_cast<char*>(&header[0]), 54);
	if (! fin.good()) {
		std::cout << "Failed to read bmp header from file " << file_path << '\n';
		fin.close();
		return -1;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		std::cout << "Not a bmp file: " << file_path << '\n';
		fin.close();
		return -1;
	}

	unsigned int data_pos   = *(int*)&(header[0x0A]);
	unsigned int width      = *(int*)&(header[0x12]);
	unsigned int height     = *(int*)&(header[0x16]);
	unsigned int image_size = *(int*)&(header[0x22]);
	if (image_size == 0) image_size = width * height * 3;
	if (data_pos == 0) data_pos = 54;

	std::vector<unsigned char> data (image_size);
	fin.seekg(data_pos);
	fin.read(reinterpret_cast<char*>(&data[0]), image_size);
	if (! fin.good()) {
		std::cout << "Failed to read bmp data from file " << file_path << '\n';
		fin.close();
		return -1;	
	}
	fin.close();

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, &data[0]);

	// simple filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// mipmap filtering
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// glGenerateMipmap(GL_TEXTURE_2D);
	return texture_id;
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

const int ATLAS_WIDTH = 4;
const int ATLAS_HEIGHT = 4;
const GLfloat WIDTH_MULT = 1.0 / ATLAS_WIDTH;
const GLfloat HEIGHT_MULT = 1.0 / ATLAS_HEIGHT;

void initDrawStep(GLuint vertex_buffer_id, GLuint uv_buffer_id, GLuint dxy_buffer_id, DrawData& data) {
	std::vector<GLfloat> vertex_buffer_data (2 * 6 * data.width * data.height);
	std::vector<GLfloat> uv_buffer_data (2 * 6 * data.width * data.height);
	std::vector<GLfloat> dxy_buffer_data (2 * 6 * data.width * data.height);

	std::vector<int> help = {
		0,0,
		0,1,
		1,0,
		1,1,
		0,1,
		1,0
	};
	for (int y = 0; y < data.height; ++y) {
		for (int x = 0; x < data.width; ++x) {
			std::vector<GLfloat> vertex_x = {(GLfloat)x, (GLfloat)x+1};
			std::vector<GLfloat> vertex_y = {(GLfloat)((data.height-1) - y), (GLfloat)(data.height-y)};

			int i = x + data.width * y;
			int th = (ATLAS_HEIGHT - 1) - (data.tex[i] / ATLAS_HEIGHT);
			int tw = data.tex[i] % ATLAS_HEIGHT;
			std::vector<GLfloat> uv_u = {tw*WIDTH_MULT, (tw+1)*WIDTH_MULT};
			std::vector<GLfloat> uv_v = {th*HEIGHT_MULT, (th+1)*HEIGHT_MULT};

			int base = 12 * i;
			for (int j = 0; j < 6; ++j) {
				vertex_buffer_data[	base + 2*j + 0] = vertex_x[help[2*j + 0]];
				uv_buffer_data[		base + 2*j + 0] = uv_u[help[2*j + 0]];
				dxy_buffer_data[	base + 2*j + 0] = data.dx[i];
				
				vertex_buffer_data[	base + 2*j + 1] = vertex_y[help[2*j + 1]];
				uv_buffer_data[		base + 2*j + 1] = uv_v[help[2*j + 1]];
				dxy_buffer_data[	base + 2*j + 1] = -data.dy[i];
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(GLfloat), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, uv_buffer_data.size() * sizeof(GLfloat), &uv_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, dxy_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, dxy_buffer_data.size() * sizeof(GLfloat), &dxy_buffer_data[0], GL_STATIC_DRAW);
}

// END DRAW LOGIC

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

	// Load texture
	std::string texture_path = "testing.bmp";
	GLint texture_id = loadBMP(texture_path);
	GLint texture_sampler_id = glGetUniformLocation(program_id, "texture_sampler");

	GLint map_scale_id = glGetUniformLocation(program_id, "map_scale");
	GLint move_scale_id = glGetUniformLocation(program_id, "move_scale");
	GLint player_pos_id = glGetUniformLocation(program_id, "player_pos");

	// Drawing things for testing
	/*
	struct DrawData {
		int width;
		int height;
		int px;
		int py;
		int pdx;
		int pdy;
		std::vector<int> dx;
		std::vector<int> dy;
		std::vector<int> tex;
	}
	*/
	DrawData data;
	data.width = 3;
	data.height = 3;
	data.px = 1;
	data.py = 1;
	data.pdx = 0;
	data.pdy = 0;
	data.dx = {0, 0, 0, 1, 0,-1, 0, 0, 0};
	data.dy = {0, 1, 0, 0,-1, 0, 0, 0, 1};
	data.tex ={0, 1, 2, 3, 4, 5, 6, 7, 8};

	GLuint vertex_buffer_id;
	glGenBuffers(1, &vertex_buffer_id);
	
	GLuint uv_buffer_id;
	glGenBuffers(1, &uv_buffer_id);

	GLuint dxy_buffer_id;
	glGenBuffers(1, &dxy_buffer_id);

	initDrawStep(vertex_buffer_id, uv_buffer_id, dxy_buffer_id, data);

	/*
	// Draw the testing data
	// Change active buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	// glBufferData ( https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml )
	// Parameters: target, size (in bytes), data, usage (expected usage pattern of the data store)
	// STREAM : The data store contents will be modified once and used at most a few times.
	// STATIC : The data store contents will be modified once and used many times.
	// DYNAMIC: The data store contents will be modified repeatedly and used many times.
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(GLfloat), &vertex_buffer_data[0], GL_STATIC_DRAW);
	*/
	glUseProgram(program_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(texture_sampler_id, 0);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	int tick = 0;
	while(true) {
		glClear( GL_COLOR_BUFFER_BIT );
		// glVertexAttribPointer ( https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml )
		// Parameters: index (in vao), size (number of components), type (of each component),
		// 	normalized (should data be normalized when accessed), stride (byte offset between values in array), pointer (offset to first value)
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, dxy_buffer_id);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	
		glUniform1f(move_scale_id, tick / 59.0);
		glUniform2f(map_scale_id, 1 / 10.0, 1 / 10.0);
		glUniform2f(player_pos_id, 1.5, 1.5);

		// glDrawArrays( https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml )
		// Parameters: type, first (vertex) to render, amount (of vertices) to render
		glDrawArrays(GL_TRIANGLES, 0, data.width * data.height * 6);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(window);
		++tick;
		if (tick > 120 + 59) tick = 0;
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
	}
	
	glfwTerminate();
	return 0;
}
