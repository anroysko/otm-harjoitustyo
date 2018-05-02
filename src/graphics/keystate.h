#ifndef __GRAPHICS_KEYSTATE_H_
#define __GRAPHICS_KEYSTATE_H_

#include <GL/glew.h>    // Opengl functions
#include <GLFW/glfw3.h> // Opengl helper
#include <vector>       // std::vector

/// @file=keystate.h
/// Keyboard I/O, struct Keystate .

// Different moves
const int MOVE_NONE = 0;
const int MOVE_LEFT = 1;
const int MOVE_RIGHT = 2;
const int MOVE_DOWN = 3;
const int MOVE_UP = 4;
const int MOVE_CONTROL_LEFT = 5;
const int MOVE_CONTROL_RIGHT = 6;
const int MOVE_CONTROL_DOWN = 7;
const int MOVE_CONTROL_UP = 8;

/// Class representing the current state of the keyboard.
/// Stores moves the player has made.
class KeyState {
private:
	/// Is control being held down.
	bool control_down;
	/// State of recognized keyboard keys.
	std::vector<int> state;
	/// Current move, that will be returned by getMove() .
	int curr_move;
	/// next move after curr_move.
	int next_move;

public:
	/// Constructor.
	KeyState();
	/// Updates state of keys, curr_move and next_move .
	void updateKeyState(GLFWwindow* window);
	/// Returns the top move in the move queue.
	int getMove();
	/// Has escape been pressed.
	bool esc_pressed;
};

#endif  // __GRAPHICS_KEYSTATE_H_
