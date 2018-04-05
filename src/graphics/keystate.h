#ifndef __GRAPHICS_KEYSTATE_H_
#define __GRAPHICS_KEYSTATE_H_

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

class KeyState {
	private:
		bool control_down;
		std::vector<int> state;
		int curr_move;
		int next_move;
	public:
		KeyState();
		void updateKeyState(GLFWwindow* window);
		int getMove();
		bool esc_pressed;
};

#endif // __GRAPHICS_KEYSTATE_H_
