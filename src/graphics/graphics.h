#ifndef __GRAPHICS_GRAPHICS_H_
#define __GRAPHICS_GRAPHICS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>  // std::unique_ptr
#include <vector>  // std::vector

#include "keystate.h"  // KeyState

struct Sprite {
	int x, y, dx, dy, tex;
	Sprite(int x, int y, int dx, int dy, int tex);
};

struct DrawData {
	int width, height;
	int cam_x, cam_y;
	int cam_dx, cam_dy;
	std::vector<Sprite> sprites;
};

class GraphicsState {
private:
	// Variables
	GLFWwindow* window;  // Pointer to window

	int screen_width;   // Window width
	int screen_height;  // Window height

	GLuint program_id;		// Shader program id
	GLuint vertex_array_object_id;  // VAO id

	GLuint vertex_buffer_id;  // Buffer for vertex coordinates
	GLuint uv_buffer_id;      // Buffer for vertex uv (image) coordinates
	GLuint dxy_buffer_id;     // Buffer for vertex dx, dy (velocity) values.

	GLuint map_scale_id;	// Uniform index for map scale
	GLuint move_scale_id;       // Uniform index for move scale (Multiplier for dx, dy)
	GLuint player_pos_id;       // Uniform index for player pos
	GLuint texture_sampler_id;  // Uniform index for texture sampler

	const int ATLAS_GRID_HEIGHT = 4;
	const int ATLAS_GRID_WIDTH = 4;
	int atlas_tile_width;
	int atlas_tile_height;
	GLuint atlas_id;  // Atlas texture id

	KeyState key_state;  // For keyboard input
	// End variables

	// Initialization functions. Private since factory method calls these for you
	bool initOpengl();
	bool initShaders();
	bool initBuffers();
	bool initTextures();
	bool init();

	// Private constructor. Use factory method
	GraphicsState() = default;

public:
	static std::unique_ptr<GraphicsState> create();
	~GraphicsState();

	void setDraw(DrawData& data);
	void draw(double dt, double timestep, DrawData& data);
	int getMove();
	bool shouldQuit();

	// Forbid copying
	GraphicsState& operator=(const GraphicsState& state) = delete;
	GraphicsState(const GraphicsState& state) = delete;
	// Forbid moving
	GraphicsState(GraphicsState&& state) = delete;
	GraphicsState& operator=(GraphicsState&& state) = delete;
};

// TODO: remove
// For temporary testing
// Returns a * b
int multiply(int a, int b);

#endif  // __GRAPHICS_GRAPHICS_H_
