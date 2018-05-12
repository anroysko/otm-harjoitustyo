#include "game.h"
#include <fstream>		     // std::ifstream
#include <iostream>		     // TODO: remove
#include <memory>		     // std::move
#include <optional>		     // std::optional
#include <queue>		     // std::priority_queue
#include <sstream>		     // std::stringstream
#include <string>		     // std::string
#include <utility>		     // std::pair
#include <vector>		     // std::vector
#include "./../graphics/graphics.h"  // DrawData, Sprite, makeSprite()
#include "./../util/constants.h"     // Constants

/// @file game.cpp

// Block and blocktype
BlockType::BlockType(bool can_fall, bool collectible, bool round, bool destroyable, int hardness, int crush_strength, int base_texture) {
	this->can_fall = can_fall;
	this->collectible = collectible;
	this->round = round;
	this->destroyable = destroyable;
	this->hardness = hardness;
	this->crush_strength = crush_strength;
	this->base_texture = base_texture;
}

BlockType makeBlockType(bool can_fall, bool collectible, bool round, bool destroyable, int hardness, int crush_strength, int base_texture) {
	BlockType type(can_fall, collectible, round, destroyable, hardness, crush_strength, base_texture);
	return type;
}

Block::Block(int type) {
	this->type = type;
	moved = false;
	drawn = false;
	falling = false;
	move_dir = MOVE_NONE;
	place_dynamite = false;
	grab = false;
	slide_dir = DIR_LEFT;
	counter = 0;
}

/*
const int BLOCK_NONE = 0;
const int BLOCK_SAND = 1;
const int BLOCK_WALL = 2;
const int BLOCK_CRACKED_WALL = 3;
const int BLOCK_ROUND_WALL = 4;
const int BLOCK_CRACKED_ROUND_WALL = 5;
const int BLOCK_ROCK = 6;
const int BLOCK_EMERALD = 7;
const int BLOCK_BOMB = 8;
const int BLOCK_BAG = 9;
const int BLOCK_DYNAMITE = 10;
const int BLOCK_ACTIVE_DYNAMITE = 11;
const int BLOCK_EXPLOSION = 12;
const int BLOCK_CLOSED_GOAL = 13;
const int BLOCK_OPEN_GOAL = 14;
const int BLOCK_PLAYER = 15;
*/
// BlockType makeBlockType(bool can_fall, bool collectible, bool round, bool destroyable, int hardness, int crush_strength, int base_texture) {
/// Vector containing the info on the block types.
std::vector<BlockType> block_types = {
	makeBlockType(false, true, false, true, 0, 0, TEXTURE_NOT_SET),
	makeBlockType(false, true, false, true, 3, 0, TEXTURE_SAND),
	makeBlockType(false, false, false, false, 3, 0, TEXTURE_WALL),
	makeBlockType(false, false, false, true, 3, 0, TEXTURE_CRACKED_WALL),
	makeBlockType(false, false, true, false, 3, 0, TEXTURE_ROUND_WALL),
	makeBlockType(false, false, true, true, 3, 0, TEXTURE_CRACKED_ROUND_WALL),
	makeBlockType(true, false, true, true, 3, 2, TEXTURE_ROCK),
	makeBlockType(true, true, true, true, 2, 1, TEXTURE_EMERALD),
	makeBlockType(true, false, true, true, 3, 0, TEXTURE_BOMB),
	makeBlockType(true, false, true, true, 3, 1, TEXTURE_BAG),
	makeBlockType(false, true, false, true, 3, 0, TEXTURE_DYNAMITE_0),
	makeBlockType(false, false, false, true, 3, 0, TEXTURE_DYNAMITE_0),
	makeBlockType(false, false, false, true, 3, 0, TEXTURE_EXPLOSION),
	makeBlockType(false, false, true, true, 3, 0, TEXTURE_CLOSED_GOAL),
	makeBlockType(false, true, true, true, 3, 0, TEXTURE_OPEN_GOAL),
	makeBlockType(false, false, false, true, 1, 0, TEXTURE_PLAYER)};

// Level's functions
Level::Level() {
	name = "UNNAMED LEVEL";
	difficulty = "TRIVIAL";
	width = 0;
	height = 0;
	needed_score = 0;
	current_score = 0;
	time_limit = 0;
	current_time = 0;
	player_destroyed = false;
	open_goals = false;
}

std::optional<Level> Level::load(const std::string& level_path) {
	std::ifstream fin;
	fin.open(level_path);
	if (!fin.is_open()) return std::nullopt;
	std::string file_type;
	std::getline(fin, file_type);
	if (file_type != "LEVEL") return std::nullopt;

	Level level;
	std::getline(fin, level.name);
	std::getline(fin, level.difficulty);

	fin >> level.width >> level.height >> level.needed_score >> level.time_limit;
	if (!fin) return std::nullopt;

	level.blocks.resize(level.width * level.height);
	for (int y = 0; y < level.height; ++y) {
		for (int x = 0; x < level.width; ++x) {
			int type;
			fin >> type;
			Block block(type);
			level.blocks[x + y * level.width] = block;
		}
	}

	if (!fin) return std::nullopt;
	return std::optional<Level>{level};
}

// Functions for updating the state of the level.
void Level::clearBlock(int place) {
	Block empty_block(BLOCK_NONE);
	blocks[place] = empty_block;
}

void Level::drawBlock(int place, int dx, int dy, DrawData* data) {
	if (blocks[place].drawn) return;
	if (blocks[place].type == BLOCK_NONE) return;
	blocks[place].drawn = true;
	int x = place % width;
	int y = place / width;
	int tex = block_types[blocks[place].type].base_texture;
	if (blocks[place].type == BLOCK_ACTIVE_DYNAMITE) tex += blocks[place].counter;
	data->sprites.push_back(makeSprite(x, y, dx, dy, tex));
}

bool Level::canSlide(int place, bool dir) {
	int dx = (dir == DIR_LEFT ? -1 : 1);
	return (block_types[blocks[place + width].type].round) && (blocks[place + dx].type == BLOCK_NONE) && (blocks[place + dx + width].type == BLOCK_NONE);
}
bool Level::canPush(int place, bool dir) {
	if (!block_types[blocks[place].type].can_fall) return false;
	if (blocks[place].falling) return false;
	if (block_types[blocks[place].type].collectible) return false;
	int dx = (dir == DIR_LEFT ? -1 : 1);
	if (blocks[place + dx].type != BLOCK_NONE) return false;
	if (blocks[place - dx].type != BLOCK_PLAYER) return false;
	if (blocks[place - dx].moved) return false;
	if (blocks[place - dx].move_dir != (dir == DIR_LEFT ? MOVE_LEFT : MOVE_RIGHT)) return false;
	if (blocks[place - dx].grab) return false;
	return true;
}

bool Level::canCollect(int place) {
	return (block_types[blocks[place].type].collectible == true) && (blocks[place].falling == false);
}

void Level::collect(int place, int player_place, DrawData* data) {
	// drawBlock(place, 0, 0, data);
	if (blocks[place].type == BLOCK_OPEN_GOAL) {
		blocks[place].type = BLOCK_CLOSED_GOAL;
		return;
	} else if (blocks[place].type == BLOCK_DYNAMITE) {
		++blocks[player_place].counter;
	} else if (blocks[place].type == BLOCK_EMERALD) {
		++current_score;
	}
	clearBlock(place);
}

void Level::goTo(int place, int target, DrawData* data) {
	int x = place % width;
	int y = place / width;
	int tx = target % width;
	int ty = target / width;
	int dx = tx - x;
	int dy = ty - y;
	blocks[place].moved = true;
	if (blocks[place].type == BLOCK_PLAYER) {
		// Camera follows bottom-right-most player.
		if (data->cam_y < y || (data->cam_y == y && data->cam_x < x)) {
			data->cam_x = x;
			data->cam_y = y;
			data->cam_dx = dx;
			data->cam_dy = dy;
		}
		collect(target, place, data);
		drawBlock(place, dx, dy, data);
		if (blocks[target].type == BLOCK_CLOSED_GOAL) {
			// Player exited! Hooray!
			clearBlock(place);
		} else {
			blocks[target] = blocks[place];
			clearBlock(place);
		}
	} else {
		if (blocks[target].type == BLOCK_PLAYER) {
			player_destroyed = true;
		}
		drawBlock(place, dx, dy, data);
		blocks[target] = blocks[place];
		clearBlock(place);
	}
}

/// Returns pair<priority, move>, where priority is -1 if the block cannot make a move.
/// Priority is never less than step.
// Step 1: Moving up
// Step 2: Moving left and right
// Step 3: Moving down, falling down
// Step 4: Sliding in the preferred direction.
// Step 5: Sliding in the opposite direction.
// Step 6: Grabbing from any direction.
std::pair<int, int> Level::movePriority(int place, int step) {
	if (blocks[place].moved == true) return {-1, -1};
	int dir = blocks[place].move_dir;
	if (blocks[place].type == BLOCK_PLAYER) {
		int ti = MOVE_NONE;
		if (dir == MOVE_LEFT) ti = place - 1;
		if (dir == MOVE_UP) ti = place - width;
		if (dir == MOVE_RIGHT) ti = place + 1;
		if (dir == MOVE_DOWN) ti = place + width;
		if (ti == MOVE_NONE) return {-1, -1};
		int dyn_bit = (((blocks[place].place_dynamite) && (blocks[place].counter > 0)) ? MOVE_PLACE_DYNAMITE_BIT : 0);
		if (blocks[place].grab) {
			// Grab-move.
			if (step <= 6) {
				if (canCollect(ti) && (blocks[ti].type != BLOCK_OPEN_GOAL)) {
					return {6, dir | MOVE_GRAB_BIT};
				}
			}
		} else {
			// Regular move.
			if (canCollect(ti)) {
				if ((dir == MOVE_UP) && (step <= 1)) {
					return {1, dir | dyn_bit};
				} else if ((dir == MOVE_LEFT || dir == MOVE_RIGHT) && (step <= 2)) {
					return {2, dir | dyn_bit};
				} else if ((dir == MOVE_DOWN) && (step <= 3)) {
					return {3, dir | dyn_bit};
				}
			}
		}
	} else if (block_types[blocks[place].type].can_fall == true) {
		// 2. Being pushed
		if (step <= 2) {
			if (canPush(place, DIR_LEFT)) return {2, MOVE_LEFT};
			if (canPush(place, DIR_RIGHT)) return {2, MOVE_RIGHT};
		}
		// 3. Falling
		if (blocks[place + width].type == BLOCK_NONE || blocks[place].falling) {
			if (step <= 3) {
				return {3, MOVE_DOWN | MOVE_FALL_BIT};
			}
		}
		// 4. Sliding in favored direction
		// 5. Sliding in unfavored direction
		if (!blocks[place].falling) {
			bool can_left = canSlide(place, DIR_LEFT);
			bool can_right = canSlide(place, DIR_RIGHT);
			if (blocks[place + width].slide_dir == DIR_LEFT) {
				if (step <= 4 && can_left) {
					return {4, MOVE_LEFT | MOVE_SLIDE_BIT};
				} else if (step <= 5 && can_right) {
					return {5, MOVE_RIGHT | MOVE_SLIDE_BIT};
				}
			} else {
				if (step <= 4 && can_right) {
					return {4, MOVE_RIGHT | MOVE_SLIDE_BIT};
				} else if (step <= 5 && can_left) {
					return {5, MOVE_LEFT | MOVE_SLIDE_BIT};
				}
			}
		}
	}
	return {-1, -1};
}

/// Assumes the move is legal.
void Level::makeMove(int place, int move, DrawData* data) {
	int dir = move & 7;

	int ti;  // Target index
	if (dir == MOVE_LEFT) ti = place - 1;
	if (dir == MOVE_UP) ti = place - width;
	if (dir == MOVE_RIGHT) ti = place + 1;
	if (dir == MOVE_DOWN) ti = place + width;

	if (move & MOVE_PLACE_DYNAMITE_BIT) {
		--blocks[place].counter;
	}
	// Special case, since we're not actually moving.
	if (move & MOVE_GRAB_BIT) {
		int x = place % width;
		int y = place / width;
		if (data->cam_y < y || (data->cam_y == y && data->cam_x < x)) {
			data->cam_x = x;
			data->cam_y = y;
			data->cam_dx = 0;
			data->cam_dy = 0;
		}
		collect(ti, place, data);
		blocks[place].moved = true;
		drawBlock(place, 0, 0, data);
	} else {
		// Handle special cases with crushing
		if (move & MOVE_FALL_BIT) {
			blocks[place].falling = true;
			// Special falling effects
			if (blocks[place].type == BLOCK_BOMB) {
				// Bomb stops falling
				if (blocks[place + width].type != BLOCK_NONE) {
					clearBlock(place);
					explosion_que.push_back({place, 3});
					return;
				}
			} else if (blocks[place].type == BLOCK_ROCK && blocks[ti].type == BLOCK_BAG) {
				// Bag gets opened
				blocks[ti].type = BLOCK_EMERALD;
				drawBlock(ti, 0, 0, data);

				blocks[place].moved = true;
				blocks[place].falling = false;
				return;
			} else if (blocks[ti].type == BLOCK_BOMB) {
				// Something falls on a bomb
				clearBlock(ti);
				explosion_que.push_back({ti, 3});

				blocks[place].moved = true;
				blocks[place].falling = false;
				return;
			}
			if (block_types[blocks[ti].type].hardness > block_types[blocks[place].type].crush_strength) {
				blocks[place].moved = true;
				blocks[place].falling = false;
				return;
			}
		} else if (move & MOVE_SLIDE_BIT) {
			// Flip the direction objects slide to from the top of this object.
			if ((blocks[place + width].slide_dir == DIR_LEFT) == (dir == MOVE_LEFT)) {
				blocks[place + width].slide_dir ^= 1;
			}
		}
		// Regular move
		goTo(place, ti, data);
		if (move & MOVE_PLACE_DYNAMITE_BIT) {
			blocks[place].type = BLOCK_ACTIVE_DYNAMITE;
			blocks[place].counter = -1;  // Dynamite timers start at -1 so that they'll be 0 at the first update.
		}
	}
}

DrawData Level::update(int move, int default_cam_x, int default_cam_y) {
	// Initialize draw data.
	DrawData data;
	data.width = width;
	data.height = height;
	data.cam_x = -1;
	data.cam_y = -1;
	data.cam_dx = 0;
	data.cam_dy = 0;

	// Step 0: Initialization.
	for (int i = 0; i < width * height; ++i) {
		// Reset "moved" and "drawn" statistics
		blocks[i].moved = false;
		blocks[i].drawn = false;
		// Set all players to be making the given move
		if (blocks[i].type == BLOCK_PLAYER) {
			blocks[i].move_dir = move & 7;
			blocks[i].grab = move & MOVE_GRAB_BIT;
			blocks[i].place_dynamite = move & MOVE_PLACE_DYNAMITE_BIT;
		}
		// Set all gravity-affected objects to fall if there's nothing below them.
		if (block_types[blocks[i].type].can_fall && blocks[i + width].type == BLOCK_NONE) {
			blocks[i].falling = true;
		}
	}
	// Movement steps
	// Step 1: Moving up
	// Step 2: Moving left and right, pushing left and right
	// Step 3: Moving down, falling down
	// Step 4: Sliding in the preferred direction.
	// Step 5: Sliding in the opposite direction.
	// Step 6: Grabbing from any direction.

	// Pair<priority, place>
	// std::priority_queue has largest first, so we have a multiplier of -1 in the priorities.
	int step = 1;
	std::priority_queue<std::pair<int, int>> que;
	// Initialize the queue
	for (int i = 0; i < width * height; ++i) {
		int pr = movePriority(i, step).first;
		if (pr != -1) que.push({-pr, i});
	}
	// Handle the events
	while (!que.empty()) {
		int i = que.top().second;
		que.pop();

		std::pair<int, int> mp = movePriority(i, step);
		if (mp.first != -1) {
			step = mp.first;
			makeMove(i, mp.second, &data);
			// Update adjancent objects.
			for (int dx = -1; dx <= 1; ++dx) {
				for (int dy = -1; dy <= 1; ++dy) {
					int j = i + dx + dy * width;
					int pr = movePriority(j, step).first;
					if (pr != -1) {
						que.push({-pr, j});
					}
				}
			}
		}
	}
	// Cleanup steps
	// Step 7: Update timers, transform goals, draw still objects, handle time running out.
	// Transform goals, if needed.
	if ((current_score >= needed_score) && (!open_goals)) {
		open_goals = true;
		for (int i = 0; i < width * height; ++i) {
			if (blocks[i].type == BLOCK_CLOSED_GOAL) {
				blocks[i].type = BLOCK_OPEN_GOAL;
			}
		}
	}
	// Update timers, draw still objects
	for (int i = 0; i < width * height; ++i) {
		// Update dynamite timer
		if (blocks[i].type == BLOCK_ACTIVE_DYNAMITE) {
			++blocks[i].counter;
			if (blocks[i].counter == RULES_DYNAMITE_TIMER) {
				// Notably, do not draw the block.
				// There is no texture for a bomb at 8'th tick.
				clearBlock(i);
				explosion_que.push_back({i, 3});
			}
		}
		// Update explosion timer
		if (blocks[i].type == BLOCK_EXPLOSION) {
			++blocks[i].counter;
			if (blocks[i].counter >= RULES_EXPLOSION_DURATION) {
				drawBlock(i, 0, 0, &data);
				clearBlock(i);
			}
		}
	}
	// Handle time running out
	// When time runs out, each player explodes.
	++current_time;
	if (current_time == time_limit) {
		for (int i = 0; i < width * height; ++i) {
			if (blocks[i].type == BLOCK_PLAYER) {
				explosion_que.push_back({i, 1});
			}
		}
	}
	// Step 8: Handle explosions
	std::vector<std::pair<int, int>> next_que;
	for (auto pr : explosion_que) {
		int i = pr.first;
		int x = i % width;
		int y = i / width;
		int w = pr.second / 3;
		for (int dx = -w; dx <= w; ++dx) {
			for (int dy = -w; dy <= w; ++dy) {
				int j = (x + dx) + (y + dy) * width;
				if (block_types[blocks[j].type].destroyable) {
					if (blocks[j].type == BLOCK_PLAYER) {
						player_destroyed = true;
					} else if (blocks[j].type == BLOCK_BOMB || blocks[j].type == BLOCK_DYNAMITE || blocks[j].type == BLOCK_ACTIVE_DYNAMITE) {
						next_que.push_back({j, 3});
					}
					clearBlock(j);
					blocks[j].type = BLOCK_EXPLOSION;
				}
			}
		}
	}
	explosion_que = std::move(next_que);
	// Step 9: Draw remaining blocks
	for (int i = 0; i < width * height; ++i) {
		// Draw still blocks
		if (blocks[i].type != BLOCK_NONE) drawBlock(i, 0, 0, &data);
		if ((blocks[i].type == BLOCK_PLAYER) && (!blocks[i].moved)) {
			int x = i % width;
			int y = i / width;
			if (data.cam_y < y || (data.cam_y == y && data.cam_x < x)) {
				data.cam_x = x;
				data.cam_y = y;
			}
		}
	}
	if (data.cam_x == -1 && data.cam_y == -1) {
		data.cam_x = default_cam_x;
		data.cam_y = default_cam_y;
	}
	return data;
}

std::string Level::getOverlayString() {
	std::stringstream ss;
	ss << name << "          "
	   << "EMERALDS: " << current_score << "/" << needed_score << "          "
	   << "TIME: " << current_time << "/" << time_limit << "          " << difficulty;
	return ss.str();
}

int Level::getState() {
	if (player_destroyed) return 2;  // LOSS
	for (int i = 0; i < width * height; ++i) {
		if (blocks[i].type == BLOCK_PLAYER) return 0;  // ONGOING
	}
	return 1;  // WIN
}

void Level::printState() {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			std::cout << blocks[x + y * width].type << " ";
			if (blocks[x + y * width].type < 10) std::cout << " ";
		}
		std::cout << '\n';
	}
}
