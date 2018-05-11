#include "game.h"
#include <vector> // std::vector
#include <string> // std::string
#include <optional> // std::optional
#include <queue> // std::priority_queue
#include <utility> // std::pair
#include <memory> // std::move

// Texture ID's (in atlas)
const int TEXTURE_SAND = 0;
const int TEXTURE_WALL = 1;
const int TEXTURE_CRACKED_WALL = 2;
const int TEXTURE_ROUND_WALL = 3;
const int TEXTURE_CRACKED_ROUND_WALL = 4;
const int TEXTURE_ROCK = 5;
const int TEXTURE_EMERALD = 6;
const int TEXTURE_BOMB = 7;
const int TEXTURE_BAG = 8;
const int TEXTURE_DYNAMITE_0 = 9;
const int TEXTURE_DYNAMITE_1 = 10;
const int TEXTURE_DYNAMITE_2 = 11;
const int TEXTURE_DYNAMITE_3 = 12;
const int TEXTURE_DYNAMITE_4 = 13;
const int TEXTURE_DYNAMITE_5 = 14;
const int TEXTURE_DYNAMITE_6 = 15;
const int TEXTURE_DYNAMITE_7 = 16;
const int TEXTURE_EXPLOSION = 17;
const int TEXTURE_CLOSED_GOAL = 18;
const int TEXTURE_OPEN_GOAL = 19;
const int TEXTURE_PLAYER = 20;

// Block types
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

// Player move types
const int MOVE_NONE = 0;
const int MOVE_LEFT = 1;
const int MOVE_UP = 2;
const int MOVE_RIGHT = 3;
const int MOVE_DOWN = 4;
const int MOVE_SLIDE_BIT = 8;
const int MOVE_FALL_BIT = 16;
const int MOVE_GRAB_BIT = 32;
const int MOVE_PLACE_DYNAMITE_BIT = 64;

// Rules constants
const int EXPLOSION_DURATION = 3;
const int DYNAMITE_TIMER = 8;

/// Struct representing a block type.
/// Doesn't contain all of the necessary information about the block type, but handles most cases.
struct BlockType {
	/// Does gravity affect this block?
	bool can_fall;
	/// Can this block be pushed?
	bool pushable;
	/// Does this block slide sideways if there is space to fall there?
	/// Requires can_fall == true, since sliding sets falling bit to one.
	bool can_slide;
	/// Can this block be collected?
	bool collectible;
	/// Do objects that can slide, that are on top of this object, slide off?
	bool round;
	/// Can this object be destroyed (by explosions, crushing, ...)
	bool destroyable;
	/// Can this block be crushed?
	bool can_be_crushed;
	/// Base texture for this block.
	int base_texture;
};

/// Struct representing a block
struct Block {
	/// One of the block type constants.
	int type;
	/// Has this block moved?
	bool moved;
	/// Has this block been drawn?
	bool drawn;
	/// Is this block falling
	bool falling;
	/// For players, which direction is this block moving in
	int move_dir;
	/// For players, is this block going to place dynamite
	bool place_dynamite;
	/// For players, is this block actually moving or just reaching to the next tile.
	bool grab;
	/// Which direction the last object that rolled of this object rolled off to?
	bool last_dir;
	/// For players, the amount of dynamite. For active dynamite, the timer.
	int counter;
};

class Level {
	private:
		/// Width of play area.
		int width;
		/// Height of play area.
		int height;
		/// Score needed to finish.
		int needed_score;
		/// Current score.
		int current_score;
		/// Time limit.
		int time_limit;
		/// Current time.
		int current_time;
		/// Contains which block is at which tile and what properties it has.
		std::vector<Block> blocks;
		/// Queue that explosions are pushed to.
		/// Pairs are <place, size>.
		/// Size must be odd. Usually size is 1 or 3.
		std::vector<std::pair<int, int>> explosion_que;
		/// Boolean representing whether the player has lost.
		/// Player has won when this is not set to false, and there are no players remaining in the level.
		bool player_destroyed;
		/// Have the goals been opened yet?
		bool open_goals;

		int getInd(int x, int y);

		/// Default constructor is private, so that it can't be used outside the class.
		/// Objects should be created with the "load" factory function, or by copying existing objects.
		Level() = default;
	public:
		/// Loads the level from the given file.
		static std::optional<Level> load(const std::string& level_path);
		/// 
		DrawData update(int move);
		
		/// Returns 0 if the game is continuing, 1 if the player has won, 2 if the player has lost.
		int getState();

		/// Copy constructor.
		Level(const Level& oth);
		/// Copy assignment.
		Level& operator=(const Level& oth);
};

/// Returns pair<priority, move>, where priority is -1 if the block cannot make a move.
/// Priority is never less than step.
std::pair<int, int> Level::movePriority(int block, int step) {

}

/*
// Player move types
const int MOVE_NONE = 0;
const int MOVE_LEFT = 1;
const int MOVE_UP = 2;
const int MOVE_RIGHT = 3;
const int MOVE_DOWN = 4;
const int MOVE_SLIDE_BIT = 8;
const int MOVE_FALL_BIT = 16;
const int MOVE_GRAB_BIT = 32;
const int MOVE_PLACE_DYNAMITE_BIT = 64;
*/

void Level::goTo(int place, int target, DrawData* data) {

}



void Level::collect(int place, DrawData* data) {

}

/// Assumes the move is legal.
void Level::makeMove(int place, int move, DrawData* data) {
	int dir = move & 7;

	int ti; // Target index
	if (dir == MOVE_LEFT) ti = place - 1;
	if (dir == MOVE_UP) ti = place - width;
	if (dir == MOVE_RIGHT) ti = place + 1;
	if (dir == MOVE_DOWN) ti = place + width;

	if ((move & MOVE_PLACE_DYNAMITE_BIT) && (blocks[place].counter == 0)) {
		// There's no placing dynamite if we don't have any.
		move ^= MOVE_PLACE_DYNAMITE_BIT;
	}
	// Special case, since we're not actually moving.
	if (move & MOVE_GRAB_BIT) {
		collect(ti, data);
		// Player move, remember to center the camera!
		// Camera always follows bottom-rightmost player
		blocks[place].draw(0, 0, data);
	} else {
		// Handle special cases with crushing
		if (dir == MOVE_DOWN) {
			if (blocks[place].type == BLOCK_BOMB) {
				// Bomb stops falling
				if (blocks[ti].type != BLOCK_NONE) {
					blocks[place].draw(0, 0, data);
					explosion_que.push_back({place, 3});
					blocks[place].clear();
					return;
				}
			} else if (blocks[place].type == BLOCK_ROCK && blocks[ti] == BLOCK_BAG) {
				// Bag gets opened
				blocks[ti].type = BLOCK_EMERALD;
				blocks[place].draw(0, 0, data);
				blocks[place].moved = true;
				blocks[place].falling = false;
				return;
			} else if (blocks[ti].type == BLOCK_BOMB) {
				// Something falls on a bomb
				blocks[ti].draw(0, 0, data);
				explosion_que.push_back({ti, 3});
				blocks[ti].clear();
				blocks[place].moved = true;
				blocks[place].falling = false;
				return;
			}
		}
		// Regular move
		goTo(place, ti, data);
		if (move & MOVE_PLACE_DYNAMITE_BIT) {
			blocks[place].type = BLOCK_ACTIVE_DYNAMITE;
			blocks[place].counter = -1; // Dynamite timers start at -1 so that they'll be 0 at the first update.
		}
	}
}

DrawData Level::update(int move) {
	// Initialize draw data.
	DrawData data;
	data.width = width;
	data.height = height;
	data.camera_x = -1;
	data.camera_y = -1;
	data.camera_dx = -1;
	data.camera_dy = -1;

	// Step 0: Initialization.
	for (int i = 0; i < width * heigth; ++i) {
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
		if (block_types[blocks[i].type].can_fall && is_empty(i + width)) {
			blocks[i].is_falling = true;
		}
	}
	// Movement steps
	// Step 1: Moving up
	// Step 2: Moving left and right
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
	while(! que.empty()) {
		int i = que.top();
		que.pop();
		std::pair<int, int mp = movePriority(i, step);
		if (mp.first != -1) {
			make_move(i, mp.second, &data);
			// Update adjancent objects.
			for (int dx = -1; dx <= 1; ++dx) {
				for (int dy = -1; dy <= 1; ++dy) {
					int j = i + dx + dy * width;
					int pr = movePriority(j, step).first;
					if (pr != -1) que.push({-pr, i});
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
			if (blocks[i].counter == DYNAMITE_TIMER) {
				// Notably, do not draw the block.	
				// There is no texture for a bomb at 8'th tick.
				blocks[i].clear();
				explosion_que.push_back({i, 3});
			}
		}
		// Update explosion timer
		if (blocks[i].type == BLOCK_EXPLOSION) {
			++blocks[i].counter;
			if (blocks[i].counter >= EXPLOSION_DURATION) blocks[i].clear();
		}
		// Draw still blocks
		if ((blocks[i].type != BLOCK_NONE) && (!blocks[i].drawn)) {
			data.sprites.push_back(blocks[i].draw(0, 0));
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
					} else if (blocks[j].type == BLOCK_BOMB || blocks[j].type == BLOCK_DYNAMITE || blocks[j].type == BLOCK_ACTIVE_DYNAMITE) {
						next_que.push_back({j, 3});
					}
					blocks[j].clear();
				}
			}
		}
	}
	explosion_que = std::move(next_que);
	return data;
}


































