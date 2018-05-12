#ifndef __GRAPHICS_KEYSTATE_H_
#define __GRAPHICS_KEYSTATE_H_

#include <GL/glew.h>		  // Opengl functions
#include <GLFW/glfw3.h>		  // Opengl helper
#include <vector>		  // std::vector
#include "./../util/constants.h"  // move consts.

/// @file=keystate.h
/// Keyboard I/O, struct Keystate .

/// Class representing the current state of the keyboard.
/// Stores moves the player has made.
class KeyState {
private:
	/// Is control being held down.
	bool control_down;
	/// Is shift being held down.
	bool shift_down;
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
