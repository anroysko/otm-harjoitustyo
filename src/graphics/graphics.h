#ifndef __GRAPHICS_GRAPHICS_H_
#define __GRAPHICS_GRAPHICS_H_

#include <GL/glew.h>     // Opengl functions
#include <GLFW/glfw3.h>  // Opengl helper

#include <memory>      // std::unique_ptr
#include <vector>      // std::vector
#include "keystate.h"  // KeyState

/// @file=graphics.h
/// Sprite, DrawData and GraphicsState -classes.

/// Represents a single sprite.
struct Sprite {
	/// x-coordinate.
	int x;
	/// y-coordinate.
	int y;
	/// x-speed.
	int dx;
	/// y-speed.
	int dy;
	/// texture index.
	int tex;

	/// Constructor.
	Sprite(int x, int y, int dx, int dy, int tex);
};

Sprite makeSprite(int x, int y, int dx, int dy, int tex);

/// Struct representing everything that needs to be drawn.
struct DrawData {
	/// How wide the area to draw is.
	int width;
	/// How tall the area to draw is.
	int height;
	/// Camera x-coordinate.
	int cam_x;
	/// Camera y-coordinate.
	int cam_y;
	/// Camera's horizontal speed.
	int cam_dx;
	/// Camera's vertical speed.
	int cam_dy;

	/// Sprites to be drawn.
	std::vector<Sprite> sprites;
};

/// Creates sprites from the given text.
/// All sprites have the given y-coordinate.
/// First sprite has the given x-coordinate, and subjequent ones have it increased by one each time.
std::vector<Sprite> textToSprites(std::string text, int first_y);

/// Object containing all openGl objects.
/// Singleton, so you can only have one at once. Use the factory function.
/// Has functions for drawing and taking input.
class GraphicsState {
private:
	/// Pointer to window
	GLFWwindow* window;

	/// Screen width
	int screen_width;
	/// Screen height
	int screen_height;
	/// Vertex Array Object id
	GLuint vertex_array_object_id;

	/// Level shader program id.
	GLuint level_program_id;
	/// Level vertex buffer for coordinates.
	GLuint level_vertex_buffer_id;
	/// Level vertex buffer for texture coordinates.
	GLuint level_uv_buffer_id;
	/// Level vertex buffer for speed.
	GLuint level_dxy_buffer_id;
	/// Level Uniform index for map scale.
	GLuint level_map_scale_id;
	/// Level Uniform index for movement scale.
	/// Multiplier for dx, dy.
	/// Should be 1 / moves_per_second.
	GLuint level_move_scale_id;
	/// Level Uniform index for player position.
	/// Camera is centered at the player.
	GLuint level_player_pos_id;
	/// Level Uniform index for the texture sampler.
	/// Samples from atlas.bmp
	GLuint level_texture_sampler_id;

	/// Overlay shader program id.
	GLuint overlay_program_id;
	/// Overlay vertex buffer for coordinates.
	GLuint overlay_vertex_buffer_id;
	/// Overlay vertex buffer for texture coordinates.
	GLuint overlay_uv_buffer_id;
	/// Overlay vertex buffer for vertex color.
	GLuint overlay_color_buffer_id;
	/// Overlay Uniform index for map scale.
	GLuint overlay_map_scale_id;
	/// Overlay Uniform index for camera's vertical speed.
	GLuint overlay_camera_dy_id;
	/// Overlay uniform index for the texture sampler.
	/// Samples from font.bmp
	GLuint overlay_texture_sampler_id;

	/// How many tiles wide atlas is.
	const int ATLAS_GRID_WIDTH = 8;
	/// How many tiles tall atlas is.
	const int ATLAS_GRID_HEIGHT = 4;
	/// How wide a single tile in atlas is.
	int atlas_tile_width;
	/// How tall a single tile in atlas is.
	int atlas_tile_height;
	/// ID for atlas texture
	GLuint atlas_id;

	/// How many tiles wide font is.
	const int FONT_GRID_WIDTH = 16;
	/// How many tiles tall font is.
	const int FONT_GRID_HEIGHT = 4;
	/// How wide a single tile in font is.
	int font_symbol_width;
	/// How tall a single tile in font is.
	int font_symbol_height;
	/// ID for font texture
	GLuint font_id;

	/// Current keyboard state
	KeyState key_state;

	/// Initializes openGL.
	bool initOpengl();
	/// Initializes shaders.
	bool initShaders();
	/// Initializes buffers.
	bool initBuffers();
	/// Initializes textures.
	bool initTextures();
	/// Calls all initialization functions in correct order.
	bool init();

	/// Private constructor.
	/// Only the factory method is allowed to create instances of this class.
	GraphicsState() = default;

public:
	/// Factory method for graphicsState.
	static std::unique_ptr<GraphicsState> create();
	/// Destructor for graphicsState.
	/// Quits openGL and frees resources.
	~GraphicsState();

	/// Sets what should be drawn on the level-layer.
	void setLevelDraw(DrawData& level_data);
	/// Sets what should be drawn on the overlay-layer.
	void setOverlayDraw(DrawData& overlay_data);
	/// Draws the level-layer.
	/// dt: how much time has elapsed since last setting of data to draw.
	/// timestep: How much time has to elapse before a move is complete
	void drawLevel(double dt, double timestep, DrawData& level_data);
	/// Draws the overlay-layer.
	/// dt: how much time has elapsed since last setting of data to draw.
	/// timestep: How much time has to elapse before a move is complete
	void drawOverlay(double dt, double timestep, DrawData& overlay_data);
	/// Draws both overlay and level.
	/// dt: how much time has elapsed since last setting of data to draw.
	/// timestep: How much time has to elapse before a move is complete
	void draw(double dt, double timestep, DrawData& level_data, DrawData& overlay_data);

	/// Get move player has made.
	int getMove();
	/// Whether the graphics context should quit.
	/// For example, if the player presses escape, or tries to close the window.
	bool shouldQuit();

	/// Copying is forbidden.
	GraphicsState& operator=(const GraphicsState& state) = delete;
	/// Copying is forbidden.
	GraphicsState(const GraphicsState& state) = delete;
	/// Moving is forbidden.
	GraphicsState(GraphicsState&& state) = delete;
	/// Moving is forbidden.
	GraphicsState& operator=(GraphicsState&& state) = delete;
};

#endif  // __GRAPHICS_GRAPHICS_H_
