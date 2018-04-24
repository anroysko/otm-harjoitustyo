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
	GLuint vertex_array_object_id;		// VAO id

	GLuint level_program_id;		// Shader program id
	GLuint level_vertex_buffer_id;  	// Buffer for vertex coordinates
	GLuint level_uv_buffer_id;      	// Buffer for vertex uv (image) coordinates
	GLuint level_dxy_buffer_id;     	// Buffer for vertex dx, dy (velocity) values.
	GLuint level_map_scale_id;		// Uniform index for map scale
	GLuint level_move_scale_id;		// Uniform index for move scale (Multiplier for dx, dy)
	GLuint level_player_pos_id;		// Uniform index for player pos
	GLuint level_texture_sampler_id;	// Uniform index for texture sampler
	
	GLuint overlay_program_id;		// Shader program id
	GLuint overlay_vertex_buffer_id;  	// Buffer for vertex coordinates
	GLuint overlay_uv_buffer_id;		// Buffer for vertex uv (image) coordinates
	GLuint overlay_color_buffer_id;		// Buffer for vertex color values
	GLuint overlay_camera_dy_id;		// Uniform index for camera's dy
	GLuint overlay_texture_sampler_id;	// Uniform index for texture sampler

	const int ATLAS_GRID_WIDTH = 4;
	const int ATLAS_GRID_HEIGHT = 4;
	int atlas_tile_width;
	int atlas_tile_height;
	GLuint atlas_id;	// Atlas texture id

	const int FONT_GRID_WIDTH = 16;
	const int FONT_GRID_HEIGHT = 4;
	int font_symbol_width;
	int font_symbol_height;
	GLuint font_id;		// Font texture id

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

	void setLevelDraw(DrawData& level_data);
	void setOverlayDraw(DrawData& overlay_data);
	void drawLevel(double dt, double timestep, DrawData& level_data);
	void drawOverlay(double dt, double timestep, DrawData& overlay_data);
	void draw(double dt, double timestep, DrawData& level_data, DrawData& overlay_data);

	int getMove();
	bool shouldQuit();

	// Delete forbidden operations
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
