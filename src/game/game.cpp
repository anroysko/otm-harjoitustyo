#include <vector>		     // std::vector
#include "./../graphics/graphics.h"  // DrawData
#include "./../graphics/keystate.h"  // Move constants

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

bool Level::playerCanMoveTo(int i) {
	if (state[i] == EMPTY) return true;
	if (state[i] == SAND) return true;
	if (state[i] == EMERALD) return true;
	if (state[i] == GOAL && current_score >= needed_score) return true;
	return false;
}

bool Level::canMoveUp(int i, std::vector<bool> &moved) {
	if (moved[i] || state[i] != PLAYER || pdy != -1) return false;
	return playerCanMoveTo(i - width);
}

bool Level::canMoveLeft(int i, std::vector<bool> &moved) {
	if (moved[i]) return false;
	if (state[i] == PLAYER) {
		return (pdx == -1) && playerCanMoveTo(i - 1);
	}
	if (state[i] == ROCK) {
		bool player_pushing = (pdx == -1) && (state[i + 1] == PLAYER) && (!moved[i + 1]);
		return player_pushing && (state[i - 1] == EMPTY);
	}
	return false;
}
bool Level::canMoveRight(int i, std::vector<bool> &moved) {
	if (moved[i]) return false;
	if (state[i] == PLAYER) {
		return (pdx == 1) && playerCanMoveTo(i + 1);
	}
	if (state[i] == ROCK) {
		bool player_pushing = (pdx == 1) && (state[i - 1] == PLAYER) && (!moved[i - 1]);
		return player_pushing && (state[i + 1] == EMPTY);
	}
	return false;
}

bool Level::canMoveDown(int i, std::vector<bool> &moved) {
	if (moved[i] || state[i] != PLAYER || pdy != 1) return false;
	return playerCanMoveTo(i + width);
}

bool Level::canFall(int i, std::vector<bool> &moved) {
	if (moved[i]) return false;
	if ((state[i] | FALLING_BIT) == (ROCK | FALLING_BIT)) {
		if (state[i + width] == EMPTY) return true;
		if (state[i] & FALLING_BIT) {
			if (state[i + width] == PLAYER) return true;
			if (state[i + width] == EMERALD) return true;
		}
		return false;
	} else if ((state[i] | FALLING_BIT) == (EMERALD | FALLING_BIT)) {
		if (state[i + width] == EMPTY) return true;
		if (state[i] & FALLING_BIT) {
			if (state[i + width] == PLAYER) return true;
		}
		return false;
	}
	return false;
}

bool Level::canSlideLeft(int i, std::vector<bool> &moved) {
	if (moved[i]) return false;
	if (state[i] != ROCK && state[i] != EMERALD) return false;
	bool round_surface = (state[i + width] == ROCK || state[i + width] == EMERALD || state[i + width] == GOAL);
	return round_surface && (state[i - 1] == EMPTY) && (state[i - 1 + width] == EMPTY);
}

bool Level::canSlideRight(int i, std::vector<bool> &moved) {
	if (moved[i]) return false;
	if (state[i] != ROCK && state[i] != EMERALD) return false;
	bool round_surface = (state[i + width] == ROCK || state[i + width] == EMERALD || state[i + width] == GOAL);
	return round_surface && (state[i + 1] == EMPTY) && (state[i + 1 + width] == EMPTY);
}

Level::Level(int width, int height, int current_score, int needed_score, std::vector<int> &state) {
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

	std::vector<int> explosions;  // Vector containing coordinates that will explode at "explode" step
	std::vector<bool> moved(width * height, false);
	std::vector<int> stack;

	// Step 0. Preprocessing
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			int i = (x + y * width);
			// Set things that have nothing below them to be falling
			if (canFall(i, moved)) state[i] |= FALLING_BIT;
		}
	}
	// Step 1. Moving sideways
	stack.resize(width * height);
	for (int i = 0; i < stack.size(); ++i) stack[i] = i;
	while (!stack.empty()) {
		int i = stack.back();
		stack.pop_back();
		int x = i % width;
		int y = i / width;
		// See if it can move
		int dx = 0;
		if (canMoveLeft(i, moved)) dx = -1;
		if (canMoveRight(i, moved)) dx = 1;
		if (dx == 0) continue;
		// Create Sprite
		Sprite sprite(x, y, dx, 0, state[i] & 15);
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
		stack.push_back(i - dx);
	}
	// Step 2. Moving up
	stack.resize(width * height);
	for (int i = 0; i < stack.size(); ++i) stack[i] = i;
	while (!stack.empty()) {
		int i = stack.back();
		stack.pop_back();
		int x = i % width;
		int y = i / width;
		// See if it can move
		int dy = 0;
		if (canMoveUp(i, moved)) dy = -1;
		if (dy == 0) continue;
		// Create Sprite
		Sprite sprite(x, y, 0, dy, state[i] & 15);
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
		stack.push_back(i - dy * width);
	}
	// Step 3. Falling/Moving down
	stack.resize(width * height);
	for (int i = 0; i < stack.size(); ++i) stack[i] = i;
	while (!stack.empty()) {
		int i = stack.back();
		stack.pop_back();
		int x = i % width;
		int y = i / width;
		// See if it can move
		int dy = 0;
		if (canFall(i, moved)) {
			dy = 1;
			state[i] |= FALLING_BIT;
		}
		if (canMoveDown(i, moved)) dy = 1;
		if (dy == 0) {
			if ((!moved[i]) && (state[i] & FALLING_BIT)) {
				state[i] &= ~FALLING_BIT;
				moved[i] = true;
				Sprite sprite(x, y, 0, 0, state[i] & 15);
				data.sprites.push_back(sprite);
			}
			continue;
		}
		// Create Sprite
		Sprite sprite(x, y, 0, dy, state[i] & 15);
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
		stack.push_back(i - dy * width);
	}
	// Step 4. Things rolling to the side
	stack.resize(width * height);
	for (int i = 0; i < stack.size(); ++i) stack[i] = i;
	while (!stack.empty()) {
		int i = stack.back();
		stack.pop_back();
		int x = i % width;
		int y = i / width;
		// See if it can move
		int dx = 0;
		if (canSlideLeft(i, moved)) dx = -1;
		if (canSlideRight(i, moved)) dx = 1;
		if (dx == 0) continue;
		// Create Sprite
		Sprite sprite(x, y, dx, 0, state[i] & 15);
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
		stack.push_back(i - 1 - width);
		stack.push_back(i + 1 - width);
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
