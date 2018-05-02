#include <fstream>		     // std::ifstream
#include <optional>		     // std::optional
#include <sstream>		     // std::stringstream
#include <utility>		     // std::move
#include <vector>		     // std::vector
#include "./../graphics/graphics.h"  // DrawData
#include "./../graphics/keystate.h"  // Move constants
#include "./../util/error.h"	 // makeError()

#include "game.h" // Implement functions for
/// @file=game.cpp

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
	this->name = "default name";
	this->width = width;
	this->height = height;
	this->current_score = current_score;
	this->needed_score = needed_score;
	this->time_limit = 0;
	this->current_time = 0;
	this->state = state;
}

// Parses a level from the file_path
// TODO: error if level format is incorrect
std::optional<Level> Level::parse(const std::string& file_path) {
	// Example level:
	/*
		lots of rocks		// Level name
		8 10			// Width, height
		7  7  7  7  7  7  7  7 	// Blocks
		7  1  1  1  1  1  5  7 
		7  3  2  3  2  3  2  7 
		7  2  3  2  3  2  3  7 
		7  3  2  3  2  3  2  7 
		7  2  3  2  3  2  3  7 
		7  1  1  1  1  1  1  7 
		7  1  1  7  7  1  1  7 
		7  8  1  1  1  1  1  7 
		7  7  7  7  7  7  7  7
		10 50			// Emeralds needed, Time Limit
	*/
	std::ifstream fin;
	fin.open(file_path);
	if (!fin.is_open()) {
		makeError() << "failed to open " << file_path << ", failed to parse level\n";
		return std::nullopt;
	}

	Level res;
	std::getline(fin, res.name);
	fin >> res.width >> res.height;
	res.state.resize(res.width * res.height);
	for (int y = 0; y < res.height; ++y) {
		for (int x = 0; x < res.width; ++x) {
			fin >> res.state[x + y * res.width];
		}
	}
	fin >> res.needed_score;
	fin >> res.time_limit;
	if (fin.fail()) {
		makeError() << "failed to read " << file_path << ", possibly it doesn't follow the format for levels\n";
		return std::nullopt;
	}
	res.current_time = 0;
	res.current_score = 0;
	return std::optional<Level>{std::move(res)};
}

std::string Level::getOverlayString() {
	std::stringstream ss;
	ss << "LEVEL: " << name << "    TIME: " << current_time << " / " << time_limit << "    EMERALDS: " << current_score << " / " << needed_score;
	return ss.str();
}

// TODO: redo
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
bool Level::playerWon() {
	for (auto it : state) {
		if (it == GOAL) return false;
	}
	return true;
}
