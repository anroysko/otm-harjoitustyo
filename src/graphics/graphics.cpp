#include <GL/glew.h>     // Opengl functions
#include <GLFW/glfw3.h>  // Helping with opengl
#include <assert.h>      // assert
#include <iostream>      // std::ostream
#include <memory>	// std::unique_ptr
#include <optional>      // std::optional
#include <string>	// std::string
#include <vector>	// std::vector

#include "./../util/bmp.h"    // Optional, loadBMP
#include "./../util/error.h"  // makeError
#include "graphics.h"
#include "shader.h"  // makeProgram

/// @file=graphics.cpp
/// Implements graphics.h

Sprite::Sprite(int x, int y, int dx, int dy, int tex) {
	this->x = x;
	this->y = y;
	this->dx = dx;
	this->dy = dy;
	this->tex = tex;
}

Sprite makeSprite(int x, int y, int dx, int dy, int tex) {
	Sprite res(x, y, dx, dy, tex);
	return res;
}

// Creates sprites from text
std::vector<Sprite> textToSprites(std::string text, int first_y) {
	std::vector<Sprite> res;
	int dx = 1;
	int first_x = -dx * (1 + text.size()) / 2;
	for (int i = 0; i < text.size(); ++i) {
		char c = text[i];
		int ind = 0;
		if (c == ' ') continue;
		if ('A' <= c && c <= 'Z') {
			ind = 0 + (c - 'A');
		}
		if ('0' <= c && c <= '9') {
			ind = 26 + (c - '0');
		}
		if (c == ':') {
			ind = 36;
		}
		if (c == '/') {
			ind = 37;
		}
		Sprite sp(first_x + dx * i, first_y, 0, 0, ind);
		res.push_back(sp);
	}
	return res;
}

/// Opengl error callback function.
/// Outputs information about the encountered error, warning or note into the standard error stream.
void errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if (type == GL_DEBUG_TYPE_ERROR) {
		makeError();
	} else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
		makeNote();
	} else {
		makeWarning();
	}

	std::string error_type = "NO ERROR TYPE MATCH";
	if (type == GL_DEBUG_TYPE_ERROR) error_type = "GL_DEBUG_TYPE_ERROR";
	if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR) error_type = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
	if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR) error_type = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
	if (type == GL_DEBUG_TYPE_PORTABILITY) error_type = "GL_DEBUG_TYPE_PORTABILITY";
	if (type == GL_DEBUG_TYPE_PERFORMANCE) error_type = "GL_DEBUG_TYPE_PERFORMANCE";
	if (type == GL_DEBUG_TYPE_MARKER) error_type = "GL_DEBUG_TYPE_MARKER";
	if (type == GL_DEBUG_TYPE_PUSH_GROUP) error_type = "GL_DEBUG_TYPE_PUSH_GROUP";
	if (type == GL_DEBUG_TYPE_POP_GROUP) error_type = "GL_DEBUG_TYPE_POP_GROUP";
	if (type == GL_DEBUG_TYPE_OTHER) error_type = "GL_DEBUG_TYPE_OTHER";

	std::string severity_level = "NO SEVERITY MATCH";
	if (severity == GL_DEBUG_SEVERITY_HIGH) severity_level = "GL_DEBUG_SEVERITY_HIGH";
	if (severity == GL_DEBUG_SEVERITY_MEDIUM) severity_level = "GL_DEBUG_SEVERITY_MEDIUM";
	if (severity == GL_DEBUG_SEVERITY_LOW) severity_level = "GL_DEBUG_SEVERITY_LOW";
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) severity_level = "GL_DEBUG_SEVERITY_NOTIFICATION";

	std::cerr << "**GL CALLBACK** "
		  << " type: " << error_type << "(" << type << "), severity: " << severity_level << "(" << severity << "), message: " << message << '\n';
}

// Initialize openGL
bool GraphicsState::initOpengl() {
	// Initialize glfw
	if (!glfwInit()) {
		makeError() << "Failed to initialize glfw\n";
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
		makeError() << "Failed to retrieve monitor\n";
		return false;
	}
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if (mode == nullptr) {
		makeError() << "Failed to retrieve monitor video mode\n";
		return false;
	}
	screen_width = mode->width;
	screen_height = mode->height;
	// http://www.glfw.org/docs/latest/group__window.html#ga5c336fddf2cbb5b92f65f10fb6043344
	// Parameters: width, height, title, monitor, share
	window = glfwCreateWindow(screen_width, screen_height, "temporary testing", 0, 0);
	// GLFWwindow* window = glfwCreateWindow(512, 512, "temporary testing", 0, 0);
	if (window == nullptr) {
		makeError() << "Failed to create window\n";
		return false;
	}
	// Set current opengl context to our current window
	glfwMakeContextCurrent(window);

	// Initialize glew
	// Glew is a opengl loading library ( https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library )
	// You need to load opengl functions to use them. Doing it is laborous and platform dependent. Glew does this for you.
	// ( https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions )
	// Opengl function pointers are loaded to the current opengl context, so we do this after initializing our opengl context.
	glewExperimental = true;  // Ensures that all extensions are considered ( http://glew.sourceforge.net/basic.html )
	if (glewInit() != GLEW_OK) {
		makeError() << "Failed to initialize GLEW\n";
		return false;
	}

	GLint tmp_error = glGetError();
	while (tmp_error != GL_NO_ERROR) {
		makeError() << "GL error initializing opengl:  " << tmp_error << '\n';
		tmp_error = glGetError();
	}

	// Enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback((GLDEBUGPROC)errorCallback, 0);

	// Successful initialization
	return true;
}

// Initialize shaders
bool GraphicsState::initShaders() {
	std::string level_vertex_shader_path = "assets/level_vertex_shader.glsl";
	std::string level_fragment_shader_path = "assets/level_fragment_shader.glsl";
	std::optional<GLuint> tmp = makeProgram(level_vertex_shader_path, level_fragment_shader_path);
	if (!tmp) return false;
	level_program_id = std::move(tmp.value());

	std::string overlay_vertex_shader_path = "assets/overlay_vertex_shader.glsl";
	std::string overlay_fragment_shader_path = "assets/overlay_fragment_shader.glsl";
	tmp = makeProgram(overlay_vertex_shader_path, overlay_fragment_shader_path);
	if (!tmp) return false;
	overlay_program_id = std::move(tmp.value());

	return true;
}

// Initialize buffers
bool GraphicsState::initBuffers() {
	glGenVertexArrays(1, &vertex_array_object_id);
	glBindVertexArray(vertex_array_object_id);

	// Level values
	glUseProgram(level_program_id);
	// Create a vertex array object
	// Vertex array object ( https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object )
	// A vertex array object stores the format of the vertex data, and the buffer objects with the vertex data arrays
	// Create buffer objects
	glGenBuffers(1, &level_vertex_buffer_id);
	glGenBuffers(1, &level_uv_buffer_id);
	glGenBuffers(1, &level_dxy_buffer_id);
	// Get uniforms's indexes
	level_map_scale_id = glGetUniformLocation(level_program_id, "map_scale");
	level_move_scale_id = glGetUniformLocation(level_program_id, "move_scale");
	level_player_pos_id = glGetUniformLocation(level_program_id, "player_pos");
	level_texture_sampler_id = glGetUniformLocation(level_program_id, "texture_sampler");

	// Overlay values
	glUseProgram(overlay_program_id);
	glGenBuffers(1, &overlay_vertex_buffer_id);
	glGenBuffers(1, &overlay_uv_buffer_id);
	glGenBuffers(1, &overlay_color_buffer_id);
	overlay_camera_dy_id = glGetUniformLocation(overlay_program_id, "camera_dy");
	overlay_texture_sampler_id = glGetUniformLocation(overlay_program_id, "texture_sampler");
	overlay_map_scale_id = glGetUniformLocation(overlay_program_id, "map_scale");
	return true;  // Nothing in this function can fail
}

// Initialize textures
bool GraphicsState::initTextures() {
	std::string atlas_path = "assets/atlas.bmp";
	std::optional<BMP> tmp = loadBMP(atlas_path);
	if (!tmp) return false;
	BMP atlas_bmp = std::move(tmp.value());

	glGenTextures(1, &atlas_id);
	glBindTexture(GL_TEXTURE_2D, atlas_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, atlas_bmp.width, atlas_bmp.height, 0, GL_BGR, GL_UNSIGNED_BYTE, &atlas_bmp.data[0]);
	atlas_tile_width = atlas_bmp.width / ATLAS_GRID_WIDTH;
	atlas_tile_height = atlas_bmp.height / ATLAS_GRID_HEIGHT;
	// simple filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	std::string font_path = "assets/font.bmp";
	tmp = loadBMP(font_path);
	if (!tmp) return false;
	BMP font_bmp = std::move(tmp.value());

	glGenTextures(1, &font_id);
	glBindTexture(GL_TEXTURE_2D, font_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, font_bmp.width, font_bmp.height, 0, GL_BGR, GL_UNSIGNED_BYTE, &font_bmp.data[0]);
	font_symbol_width = font_bmp.width / FONT_GRID_WIDTH;
	font_symbol_height = font_bmp.height / FONT_GRID_HEIGHT;
	// simple filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	return true;
}

// Initialize the graphics state
bool GraphicsState::init() {
	// Order is important!
	if (!initOpengl()) {
		makeError() << "Failed to initialize opengl\n";
		return false;
	}
	if (!initShaders()) {
		makeError() << "Failed to initialize shaders\n";
		return false;
	}
	if (!initBuffers()) {
		makeError() << "Failed to initialize buffers\n";
		return false;
	}
	if (!initTextures()) {
		makeError() << "Failed to initialize textures\n";
		return false;
	}

	return true;
}

GraphicsState::~GraphicsState() {
	// TODO: actually safely quit
	glfwTerminate();
}

// Factory method.
std::unique_ptr<GraphicsState> GraphicsState::create() {
	std::unique_ptr<GraphicsState> state(new GraphicsState);
	if (!state->init()) state = nullptr;
	return state;
}

// Set what to draw in the level-layer
void GraphicsState::setLevelDraw(DrawData& data) {
	int sprite_cou = data.sprites.size();
	std::vector<GLfloat> vertex_buffer_data(2 * 6 * sprite_cou);
	std::vector<GLfloat> uv_buffer_data(2 * 6 * sprite_cou);
	std::vector<GLfloat> dxy_buffer_data(2 * 6 * sprite_cou);

	// x- and y-coordinates of the endpoints of the two triangles making a square
	std::vector<int> help = {
		0, 0,
		0, 1,
		1, 0,
		1, 1,
		0, 1,
		1, 0};

	for (int i = 0; i < sprite_cou; ++i) {
		Sprite sp = data.sprites[i];

		std::vector<GLfloat> vertex_x = {(GLfloat)sp.x, (GLfloat)sp.x + 1};
		// Invert y-axis
		std::vector<GLfloat> vertex_y = {(GLfloat)(data.height - sp.y), (GLfloat)(data.height - (sp.y + 1))};
		int th = (ATLAS_GRID_HEIGHT - 1) - (sp.tex / ATLAS_GRID_WIDTH);
		int tw = sp.tex % ATLAS_GRID_WIDTH;
		std::vector<GLfloat> uv_u = {(GLfloat)tw / ATLAS_GRID_WIDTH, (GLfloat)(tw + 1) / ATLAS_GRID_WIDTH};
		// Invert y-axis
		std::vector<GLfloat> uv_v = {(GLfloat)(th + 1) / ATLAS_GRID_HEIGHT, (GLfloat)th / ATLAS_GRID_HEIGHT};

		int base = 12 * i;
		for (int j = 0; j < 6; ++j) {
			// Fill vertex data
			vertex_buffer_data[base + 2 * j + 0] = vertex_x[help[2 * j + 0]];
			uv_buffer_data[base + 2 * j + 0] = uv_u[help[2 * j + 0]];
			dxy_buffer_data[base + 2 * j + 0] = sp.dx;

			vertex_buffer_data[base + 2 * j + 1] = vertex_y[help[2 * j + 1]];
			uv_buffer_data[base + 2 * j + 1] = uv_v[help[2 * j + 1]];
			dxy_buffer_data[base + 2 * j + 1] = -sp.dy;
		}
	}
	// Buffer data
	glBindBuffer(GL_ARRAY_BUFFER, level_vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(GLfloat), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, level_uv_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, uv_buffer_data.size() * sizeof(GLfloat), &uv_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, level_dxy_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, dxy_buffer_data.size() * sizeof(GLfloat), &dxy_buffer_data[0], GL_STATIC_DRAW);
}

// Set what to draw on the overlay level
void GraphicsState::setOverlayDraw(DrawData& data) {
	int sprite_cou = data.sprites.size();
	std::vector<GLfloat> vertex_buffer_data(2 * 6 * sprite_cou);
	std::vector<GLfloat> uv_buffer_data(2 * 6 * sprite_cou);
	std::vector<GLfloat> color_buffer_data(3 * 6 * sprite_cou);

	// x- and y-coordinates of the endpoints of the two triangles making a square
	std::vector<int> help = {
		0, 0,
		0, 1,
		1, 0,
		1, 1,
		0, 1,
		1, 0};

	for (int i = 0; i < sprite_cou; ++i) {
		Sprite sp = data.sprites[i];

		std::vector<GLfloat> vertex_x = {(GLfloat)sp.x, (GLfloat)sp.x + 1};
		// Invert y-axis
		std::vector<GLfloat> vertex_y = {(GLfloat)(sp.y), (GLfloat)(sp.y - 1)};
		int th = (FONT_GRID_HEIGHT - 1) - (sp.tex / FONT_GRID_WIDTH);
		int tw = sp.tex % FONT_GRID_WIDTH;
		std::vector<GLfloat> uv_u = {(GLfloat)tw / FONT_GRID_WIDTH, (GLfloat)(tw + 1) / FONT_GRID_WIDTH};
		// Invert y-axis
		std::vector<GLfloat> uv_v = {(GLfloat)(th + 1) / FONT_GRID_HEIGHT, (GLfloat)th / FONT_GRID_HEIGHT};

		int base = 12 * i;
		for (int j = 0; j < 6; ++j) {
			// Fill vertex data
			vertex_buffer_data[base + 2 * j + 0] = vertex_x[help[2 * j + 0]];
			uv_buffer_data[base + 2 * j + 0] = uv_u[help[2 * j + 0]];

			vertex_buffer_data[base + 2 * j + 1] = vertex_y[help[2 * j + 1]];
			uv_buffer_data[base + 2 * j + 1] = uv_v[help[2 * j + 1]];
		}
	}
	for (int j = 0; j < sprite_cou * 18; ++j) color_buffer_data[j] = 0.5;
	// Buffer data
	glBindBuffer(GL_ARRAY_BUFFER, overlay_vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(GLfloat), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, overlay_uv_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, uv_buffer_data.size() * sizeof(GLfloat), &uv_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, overlay_color_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, color_buffer_data.size() * sizeof(GLfloat), &color_buffer_data[0], GL_STATIC_DRAW);
}

// Draw the level
void GraphicsState::drawLevel(double dt, double time_per_step, DrawData& level_data) {
	glUseProgram(level_program_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlas_id);
	glUniform1i(level_texture_sampler_id, 0);

	glBindBuffer(GL_ARRAY_BUFFER, level_vertex_buffer_id);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, level_uv_buffer_id);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, level_dxy_buffer_id);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// TODO: calculate player coordinates here
	double tiles_per_width = (double)screen_width / (atlas_tile_width);
	double tiles_per_height = (double)screen_height / (atlas_tile_height);
	double camera_x = (double)(level_data.width) / 2;
	double camera_y = (double)(level_data.height) / 2;
	/*
	double camera_x = 0.5 + level_data.cam_x + level_data.cam_dx * dt / time_per_step;
	double camera_y = 0.5 + level_data.height - level_data.cam_y - level_data.cam_dy * dt / time_per_step;
	double left_tiles = camera_x / tiles_per_width;
	double right_tiles = (level_data.width + 1 - camera_x) / tiles_per_width;
	double up_tiles = camera_y / tiles_per_width
	*/
	glUniform1f(level_move_scale_id, dt / time_per_step);				 // How much of a step has elapsed
	glUniform2f(level_map_scale_id, 2.0 / tiles_per_width, 2.0 / tiles_per_height);  // How wide and tall blocks should be (here, 64)
	glUniform2f(level_player_pos_id, camera_x, camera_y);				 // Player coordinates

	glDrawArrays(GL_TRIANGLES, 0, level_data.sprites.size() * 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

// Draw the overlay
void GraphicsState::drawOverlay(double dt, double time_per_step, DrawData& overlay_data) {
	glUseProgram(overlay_program_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font_id);
	glUniform1i(overlay_texture_sampler_id, 0);

	glBindBuffer(GL_ARRAY_BUFFER, overlay_vertex_buffer_id);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, overlay_uv_buffer_id);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, overlay_color_buffer_id);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glUniform1f(overlay_camera_dy_id, 0.0);
	glUniform2f(overlay_map_scale_id, 2.0 * font_symbol_width / screen_width, 2.0 * font_symbol_height / screen_height);

	glDrawArrays(GL_TRIANGLES, 0, overlay_data.sprites.size() * 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

// Draw
void GraphicsState::draw(double dt, double time_per_step, DrawData& level_data, DrawData& overlay_data) {
	// Update inputs
	key_state.updateKeyState(window);

	// Clear previous
	glClearColor(0.0, 0.0, 0.0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw level
	drawLevel(dt, time_per_step, level_data);
	// End draw level

	// Draw overlay
	drawOverlay(dt, time_per_step, overlay_data);
	// End draw overlay

	// Swap buffers to finalize drawing
	glfwSwapBuffers(window);
}

// Get keyboard input
int GraphicsState::getMove() {
	return key_state.getMove();
}

// Whether this graphics state should close
bool GraphicsState::shouldQuit() {
	if (key_state.esc_pressed) return true;
	if (glfwWindowShouldClose(window)) return true;
	return false;
}
