#include "./../graphics/graphics.h"	// DrawData
#include "./../graphics/keystate.h"	// Move constants
#include <vector>			// std::vector

#include "game.h"

// TODO: redo

// ATLAS INDICES
const int EMPTY = 0;
const int SAND = 1;
const int ROCK = 2;
const int EMERALD = 3;
const int GOAL = 5;
// Open goal: 6
const int WALL = 7;
const int PLAYER = 8;

const int FALLING_BIT = 16;

bool Level::can_move_up(int i, std::vector<bool>& moved) {
	return (!moved[i]) && (state[i] == PLAYER) && (pdy == -1) && ((state[i-width] == EMPTY) || (state[i-width] == SAND) || (state[i-width] == EMERALD) || (state[i-width] == GOAL && current_score >= needed_score));
}

bool Level::can_move_left(int i, std::vector<bool>& moved) {
	if (moved[i]) return false;
	if (state[i] == PLAYER) {
		return (pdx == -1) && (state[i-1] == EMPTY || state[i-1] == SAND || state[i-1] == EMERALD || (state[i-1] == GOAL && current_score >= needed_score));
	}
	if (state[i] == ROCK) {
		return (pdx == -1) && (state[i-1] == EMPTY) && (state[i+1] == PLAYER) && (! moved[i+1]);
	}
	return false;
}

bool Level::can_move_right(int i, std::vector<bool>& moved) {
	if (moved[i]) return false;
	if (state[i] == PLAYER) {
		return (pdx == 1) && (state[i+1] == EMPTY || state[i+1] == SAND || state[i+1] == EMERALD || (state[i+1] == GOAL && current_score >= needed_score));
	}
	if (state[i] == ROCK) {
		return (pdx == 1) && (state[i+1] == EMPTY) && (state[i-1] == PLAYER) && (! moved[i-1]);
	}
	return false;
}

bool Level::can_move_down(int i, std::vector<bool>& moved) {
	return (!moved[i]) && (state[i] == PLAYER) && (pdy == 1) && ((state[i+width] == EMPTY) || (state[i+width] == SAND) || (state[i+width] == EMERALD) || (state[i+width] == GOAL && current_score >= needed_score));
}

bool Level::can_fall(int i, std::vector<bool>& moved) {
	if (moved[i]) return false;
	if ((state[i] | FALLING_BIT) == (ROCK | FALLING_BIT)) {
		if (state[i + width] == EMPTY) return true;
		if (state[i] & FALLING_BIT) {
			if (state[i+width] == PLAYER) return true;
			if (state[i+width] == EMERALD) return true;
		}
		return false;
	} else if ((state[i] | FALLING_BIT) == (EMERALD | FALLING_BIT)) {
		if (state[i + width] == EMPTY) return true;
		if (state[i] & FALLING_BIT) {
			if (state[i+width] == PLAYER) return true;
		}
		return false;
	}
	return false;
}

bool Level::can_slide_left(int i, std::vector<bool>& moved) {
	return (!moved[i]) && (state[i] == ROCK || state[i] == EMERALD) && (state[i+width] == ROCK || state[i+width] == EMERALD || state[i+width] == GOAL) && (state[i-1] == EMPTY) && (state[i-1+width] == EMPTY);
}

bool Level::can_slide_right(int i, std::vector<bool>& moved) {
	return (!moved[i]) && (state[i] == ROCK || state[i] == EMERALD) && (state[i+width] == ROCK || state[i+width] == EMERALD || state[i+width] == GOAL) && (state[i+1] == EMPTY) && (state[i+1+width] == EMPTY);
}

Level::Level(int width, int height, int current_score, int needed_score, std::vector<int>& state) {
	this->width = width;
	this->height = height;
	this->current_score = current_score;
	this->needed_score = needed_score;
	this->state = state;
}

DrawData Level::update(int move) {
	pdx = 0;
	pdy = 0;
	if (move == MOVE_LEFT) {
		pdx = -1;
	} else if (move == MOVE_RIGHT) {
		pdx = 1;
	} else if (move == MOVE_DOWN) {
		pdy = 1;
	} else if (move == MOVE_UP) {
		pdy = -1;
	}
	// std::cout << pdx << ' ' << pdy << '\n';
	DrawData data;
	data.width = width;
	data.height = height;
	data.cam_x = 0;
	data.cam_y = 0;
	data.cam_dx = 0;
	data.cam_dy = 0;

	// DO STUFF

	std::vector<int> explosions; // Vector containing coordinates that will explode at "explode" step
	std::vector<bool> moved (width * height, false);
	std::vector<int> stack;

	// Step 0. Preprocessing
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			int i = (x + y * width);
			// Set things that have nothing below them to be falling
			if (can_fall(i, moved)) state[i] |= FALLING_BIT;
		}
	}
	// Step 1. Moving sideways
	stack.resize(width * height);
	for (int i = 0; i < stack.size(); ++i) stack[i] = i;
	while(! stack.empty()) {
		int i = stack.back();
		stack.pop_back();
		int x = i % width;
		int y = i / width;
		// See if it can move
		int dx = 0;
		if (can_move_left(i, moved)) dx = -1;
		if (can_move_right(i, moved)) dx = 1;
		if (dx == 0) continue;
		// Create Sprite
		Sprite sprite (x, y, dx, 0, state[i] & 15);
		data.sprites.push_back(sprite);
		// Update level
		int ti = i + dx;
		if (state[ti] == EMERALD && state[i] == PLAYER) {
			++current_score;
		}
		moved[i] = false;
		moved[ti] = true;
		state[ti] = state[i];
		state[i] = EMPTY;
		// Update stack
		stack.push_back(i-dx);
	}
	// Step 2. Moving up
	stack.resize(width * height);
	for (int i = 0; i < stack.size(); ++i) stack[i] = i;
	while(! stack.empty()) {
		int i = stack.back();
		stack.pop_back();
		int x = i % width;
		int y = i / width;
		// See if it can move
		int dy = 0;
		if (can_move_up(i, moved)) dy = -1;
		if (dy == 0) continue;
		// Create Sprite
		Sprite sprite (x, y, 0, dy, state[i] & 15);
		data.sprites.push_back(sprite);
		// Update level
		int ti = i + dy * width;
		if (state[ti] == EMERALD && state[i] == PLAYER) {
			++current_score;
		}
		moved[i] = false;
		moved[ti] = true;
		state[ti] = state[i];
		state[i] = EMPTY;
		// Update stack
		stack.push_back(i-dy*width);
	}
	// Step 3. Falling/Moving down
	stack.resize(width * height);
	for (int i = 0; i < stack.size(); ++i) stack[i] = i;
	while(! stack.empty()) {
		int i = stack.back();
		stack.pop_back();
		int x = i % width;
		int y = i / width;
		// See if it can move
		int dy = 0;
		if (can_fall(i, moved)) {
			dy = 1;
			state[i] |= FALLING_BIT;
		}
		if (can_move_down(i, moved)) dy = 1;
		if (dy == 0) {
			if ((!moved[i]) && (state[i] & FALLING_BIT)) {
				state[i] &= ~FALLING_BIT;
				moved[i] = true;
				Sprite sprite (x, y, 0, 0, state[i] & 15);
				data.sprites.push_back(sprite);
			}
			continue;
		}
		// Create Sprite
		Sprite sprite (x, y, 0, dy, state[i] & 15);
		data.sprites.push_back(sprite);
		// Update level
		int ti = i + dy * width;
		if (state[ti] == EMERALD && state[i] == PLAYER) {
			++current_score;
		}
		moved[i] = false;
		moved[ti] = true;
		state[ti] = state[i];
		state[i] = EMPTY;
		// Update stack
		stack.push_back(i-dy*width);
	}
	// Step 4. Things rolling to the side
	stack.resize(width * height);
	for (int i = 0; i < stack.size(); ++i) stack[i] = i;
	while(! stack.empty()) {
		int i = stack.back();
		stack.pop_back();
		int x = i % width;
		int y = i / width;
		// See if it can move
		int dx = 0;
		if (can_slide_left(i, moved)) dx = -1;
		if (can_slide_right(i, moved)) dx = 1;
		if (dx == 0) continue;
		// Create Sprite
		Sprite sprite (x, y, dx, 0, state[i] & 15);
		data.sprites.push_back(sprite);
		// Update level
		state[i] |= FALLING_BIT;
		int ti = i + dx;
		if (state[ti] == EMERALD && state[i] == PLAYER) {
			++current_score;
		}
		moved[i] = false;
		moved[ti] = true;
		state[ti] = state[i];
		state[i] = EMPTY;
		// Update stack
		stack.push_back(i-1-width);
		stack.push_back(i+1-width);
	}
	
	// Step 5. Draw objects that didn't move
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			int i = x + y * width;
			if (moved[i] == true) continue;
			if (state[i] == EMPTY) continue;
			if (state[i] == GOAL && current_score >= needed_score) {
				Sprite sprite(x, y, 0, 0, 6);
				data.sprites.push_back(sprite);
			} else {
				Sprite sprite(x, y, 0, 0, state[i] & 15);
				data.sprites.push_back(sprite);
			}
		}
	}
	
	// END DO STUFF
	return data;
}

// TMP thing for testing
// TODO: remove
int add(int a, int b) {
	return a + b;
}
