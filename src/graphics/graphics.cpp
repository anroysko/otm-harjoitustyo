#include "graphics.h"

#include <GL/glew.h>	// Opengl functions
#include <GLFW/glfw3.h> // Helping with opengl

#include "util.h"	// Optional, loadBMP
#include "shader.h"	// makeProgram

#include <iostream>	// std::cout
#include <vector>	// std::vector
#include <memory>	// std::unique_ptr
#include <string>	// std::string
#include <fstream>	// std::ifstream
#include <sstream>	// std::stringstream
#include <assert.h>

Sprite::Sprite(int x, int y, int dx, int dy, int tex) {
	this->x = x;
	this->y = y;
	this->dx = dx;
	this->dy = dy;
	this->tex = tex;
}

void errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::string callback_type = (type == GL_DEBUG_TYPE_ERROR ? "** GL_ERROR **" : "");
	std::cout << "GL CALLBACK: " << callback_type << " type = " << type << ", severity = " << severity << ", message = " << message << '\n';
}

bool GraphicsState::initOpengl() {
	// Initialize glfw
	if (!glfwInit()) {
		std::cout << "Failed to initialize glfw\n";
		return false;
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
	if (monitor == nullptr) {
		std::cout << "Failed to retrieve monitor\n";
		return false;
	}
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if (mode == nullptr) {
		std::cout << "Failed to retrieve monitor video mode\n";
		return false;
	}
	screen_width = mode->width;
	screen_height = mode->height;
	// http://www.glfw.org/docs/latest/group__window.html#ga5c336fddf2cbb5b92f65f10fb6043344
	// Parameters: width, height, title, monitor, share
	window = glfwCreateWindow(screen_width, screen_height, "temporary testing", 0, 0);
	// GLFWwindow* window = glfwCreateWindow(512, 512, "temporary testing", 0, 0);
	if (window == nullptr) {
		std::cout << "Failed to create window\n";
		return false;
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
		return false;
	}
	
	GLint tmp_error = glGetError();
	while(tmp_error != GL_NO_ERROR) {
		std::cout << "GL error initializing opengl:  " << tmp_error << '\n';
		tmp_error = glGetError();
	}

	// Enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback((GLDEBUGPROC) errorCallback, 0);

	// Successful initialization
	return true;
}

bool GraphicsState::initShaders() {
	std::string vertex_shader_path = "vertex_shader.glsl";
	std::string fragment_shader_path = "fragment_shader.glsl";
	Optional<GLuint> tmp = makeProgram(vertex_shader_path, fragment_shader_path);
	if (! tmp) return false;

	program_id = tmp.unwrap();

	glUseProgram(program_id);
	
	return true;
}

bool GraphicsState::initBuffers() {
	// Create a vertex array object
	// Vertex array object ( https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object )
	// A vertex array object stores the format of the vertex data, and the buffer objects with the vertex data arrays
	glGenVertexArrays(1, &vertex_array_object_id);
	glBindVertexArray(vertex_array_object_id);

	// Create buffer objects
	glGenBuffers(1, &vertex_buffer_id);
	glGenBuffers(1, &uv_buffer_id);
	glGenBuffers(1, &dxy_buffer_id);

	// Get uniforms's indexes
	map_scale_id = glGetUniformLocation(program_id, "map_scale");
	move_scale_id = glGetUniformLocation(program_id, "move_scale");
	player_pos_id = glGetUniformLocation(program_id, "player_pos");
	texture_sampler_id = glGetUniformLocation(program_id, "texture_sampler");

	return true;
}

bool GraphicsState::initTextures() {
	std::string atlas_path = "atlas.bmp";
	Optional<BMP> tmp = loadBMP(atlas_path);
	if (!tmp) return false;
	BMP atlas_bmp = tmp.unwrap();	

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, atlas_bmp.width, atlas_bmp.height, 0, GL_BGR, GL_UNSIGNED_BYTE, &atlas_bmp.data[0]);
	atlas_id = texture_id;
	atlas_tile_width = atlas_bmp.width / ATLAS_GRID_WIDTH;
	atlas_tile_height = atlas_bmp.height / ATLAS_GRID_HEIGHT;

	// simple filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Bind atlas texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlas_id);
	glUniform1i(texture_sampler_id, 0);
	
	return true;
}

bool GraphicsState::init() {
	// Order is important!	
	if (!initOpengl()) {
		std::cout << "Failed to initialize opengl\n";
		return false;
	}
	if (!initShaders()) {
		std::cout << "Failed to initialize shaders\n";
		return false;
	}
	if (!initBuffers()) {
		std::cout << "Failed to initialize buffers\n";
		return false;
	}
	if (!initTextures()) {
		std::cout << "Failed to initialize textures\n";
		return false;
	}
	std::cout << "program_id " << program_id << '\n';
	std::cout << "vertex_array_object_id " << vertex_array_object_id << '\n';
	std::cout << "vertex_buffer_id " << vertex_buffer_id << '\n';
	std::cout << "uv_buffer_id " << uv_buffer_id << '\n';
	std::cout << "dxy_buffer_id " << dxy_buffer_id << '\n';
	std::cout << "map_scale_id " << map_scale_id << '\n';
	std::cout << "move_scale_id " << move_scale_id << '\n';
	std::cout << "player_pos_id " << player_pos_id << '\n';
	std::cout << "texture_sampler_id " << texture_sampler_id << '\n';
	std::cout << "ATLAS_GRID_HEIGHT " << ATLAS_GRID_HEIGHT << '\n';
	std::cout << "ATLAS_GRID_WIDTH " << ATLAS_GRID_WIDTH << '\n';
	std::cout << "atlas_tile_width " << atlas_tile_width << '\n';
	std::cout << "atlas_tile_height " << atlas_tile_height << '\n';
	std::cout << "screen_width " << screen_width << '\n';
	std::cout << "screen_height " << screen_height << '\n';
	std::cout << "atlas_id " << atlas_id << '\n';
	return true;
}


GraphicsState::~GraphicsState() {
	// TODO: actually safely quit
	// Not really a high priority though :Dd
	glfwTerminate();
}

std::unique_ptr<GraphicsState> GraphicsState::create() {
	std::unique_ptr<GraphicsState> state (new GraphicsState);
	if (! state->init()) state = nullptr;
	return state;
}

void GraphicsState::setDraw(DrawData& data) {
	int sprite_cou = data.sprites.size();
	std::vector<GLfloat> vertex_buffer_data (2 * 6 * sprite_cou);
	std::vector<GLfloat> uv_buffer_data (2 * 6 * sprite_cou);
	std::vector<GLfloat> dxy_buffer_data (2 * 6 * sprite_cou);

	// x- and y-coordinates of the endpoints of the two triangles making a square
	std::vector<int> help = {
		0,0,
		0,1,
		1,0,
		1,1,
		0,1,
		1,0
	};

	for (int i = 0; i < sprite_cou; ++i) {
		Sprite sp = data.sprites[i];

		std::vector<GLfloat> vertex_x = {(GLfloat)sp.x, (GLfloat)sp.x + 1};
		// Invert y-axis
		std::vector<GLfloat> vertex_y = {(GLfloat)(data.height - sp.y), (GLfloat)(data.height-(sp.y + 1))};
		int th = (ATLAS_GRID_HEIGHT - 1) - (sp.tex / ATLAS_GRID_HEIGHT);
		int tw = sp.tex % ATLAS_GRID_HEIGHT;
		std::vector<GLfloat> uv_u = {(GLfloat)tw / ATLAS_GRID_WIDTH, (GLfloat)(tw+1) / ATLAS_GRID_WIDTH};
		// Invert y-axis
		std::vector<GLfloat> uv_v = {(GLfloat)(th+1) / ATLAS_GRID_HEIGHT, (GLfloat)th / ATLAS_GRID_HEIGHT};

		int base = 12 * i;
		for (int j = 0; j < 6; ++j) {
			// Fill vertex data
			vertex_buffer_data[     base + 2*j + 0] = vertex_x[help[2*j + 0]];
			uv_buffer_data[         base + 2*j + 0] = uv_u[help[2*j + 0]];
			dxy_buffer_data[        base + 2*j + 0] = sp.dx;

			vertex_buffer_data[     base + 2*j + 1] = vertex_y[help[2*j + 1]];
			uv_buffer_data[         base + 2*j + 1] = uv_v[help[2*j + 1]];
			dxy_buffer_data[        base + 2*j + 1] = -sp.dy;
		}
	}
	// Buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(GLfloat), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, uv_buffer_data.size() * sizeof(GLfloat), &uv_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, dxy_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, dxy_buffer_data.size() * sizeof(GLfloat), &dxy_buffer_data[0], GL_STATIC_DRAW);
}

void GraphicsState::draw(double dt, double time_per_step, DrawData& data) {
	key_state.updateKeyState(window);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);	
	glClear( GL_COLOR_BUFFER_BIT );

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, dxy_buffer_id);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// TODO: calculate player coordinates here
	glUniform1f(move_scale_id, dt / time_per_step); // How much of a step has elapsed
	glUniform2f(map_scale_id, 2.0 * 64.0 / screen_width, 2.0 * 64.0 / screen_height); // How wide and tall blocks should be
	glUniform2f(player_pos_id, 4.0, 5.0);	// Player coordinates

	glDrawArrays(GL_TRIANGLES, 0, data.sprites.size() * 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glfwSwapBuffers(window);
}

int GraphicsState::getMove() {
	return key_state.getMove();
}

bool GraphicsState::shouldQuit() {
	if (key_state.esc_pressed) return true;
	if (glfwWindowShouldClose(window)) return true;
	return false;
}

// TODO: remove
// For temporary testing
int multiply(int a, int b) {
	return a * b;
}
