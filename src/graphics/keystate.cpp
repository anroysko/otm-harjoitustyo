#include <GL/glew.h>		  // Opengl functions
#include <GLFW/glfw3.h>		  // Opengl helper
#include <vector>		  // std::vector
#include "./../util/constants.h"  // Move constants

#include "keystate.h"

/// @file=keystate.cpp
/// Implements keystate.h .

const int KEYSTATE_RELEASED = 0;
const int KEYSTATE_PRESSED = 1;
const int KEYSTATE_DOWN = 2;
/// Keys on the keyboard that should be kept track of
const std::vector<int> KEYS_TO_RECORD = {GLFW_KEY_LEFT, GLFW_KEY_UP, GLFW_KEY_RIGHT, GLFW_KEY_DOWN};

KeyState::KeyState() {
	control_down = false;
	shift_down = false;
	esc_pressed = false;
	state.resize(KEYS_TO_RECORD.size());
	for (int i = 0; i < state.size(); ++i) state[i] = KEYSTATE_RELEASED;
	curr_move = 0;
	next_move = 0;
}

void KeyState::updateKeyState(GLFWwindow* window) {
	glfwPollEvents();
	control_down = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);
	shift_down = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) esc_pressed = true;
	for (int i = 0; i < 4; ++i) {
		bool down = (glfwGetKey(window, KEYS_TO_RECORD[i]) == GLFW_PRESS);
		if (down) {
			if (state[i] == KEYSTATE_RELEASED) {
				// Key pressed down
				state[i] = KEYSTATE_PRESSED;

				int move = (MOVE_LEFT + i) | (control_down ? MOVE_GRAB_BIT : 0) | (shift_down ? MOVE_PLACE_DYNAMITE_BIT : 0);
				if (curr_move == MOVE_NONE) {
					curr_move = move;
				} else {
					next_move = move;
				}
			}
		} else {
			state[i] = KEYSTATE_RELEASED;
		}
	}
}

int KeyState::getMove() {
	// Check for keys down
	for (int i = 0; i < 4; ++i) {
		if (state[i] == KEYSTATE_DOWN) {
			int move = (MOVE_LEFT + i) | (control_down ? MOVE_GRAB_BIT : 0) | (shift_down ? MOVE_PLACE_DYNAMITE_BIT : 0);
			if (curr_move == MOVE_NONE) curr_move = move;
			// No buffering moves from keys being held down
		}
	}
	for (int i = 0; i < 4; ++i) {
		if (state[i] == KEYSTATE_PRESSED) state[i] = KEYSTATE_DOWN;
	}

	int res = curr_move;
	curr_move = next_move;
	next_move = MOVE_NONE;
	return res;
}
