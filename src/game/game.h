#ifndef __GAME_GAME_H_
#define __GAME_GAME_H_

#include <vector> // std::vector
#include <string> // std::string
#include <optional> // std::optional
#include <utility> // std::pair
#include <memory> // std::move
#include "./../graphics/graphics.h" // DrawData, Sprite
#include "./../util/constants.h" // Constants

/// Struct representing a block type.
/// Doesn't contain all of the necessary information about the block type, but handles most cases.
struct BlockType {
	/// Does gravity affect this block?
	/// Also determines if it can be pushed, if it can slide, and if it can crush.
	bool can_fall;
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
	
	/// Constructs a block of default type.
	BlockType();
	/// Constructs a block type with the given parameters.
	BlockType(bool can_fall, bool collectible, bool round, bool destroyable, bool can_be_crushed, int base_texture);
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
	bool slide_dir;
	/// For players, the amount of dynamite. For active dynamite, the timer.
	int counter;
	/// Constructs a empty block.
	Block() = default;
	/// Constructs a block of the given type.
	Block(int type);
};

struct Level {
	private:
		/// Name of the level.
		std::string name;
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

		/// Helper functions
		/// Clears the block at the given location.
		void clearBlock(int place);
		/// Draws the given block into the draw data, unless its drawn -variable is set to true.
		/// This way no block gets drawn multiple times.
		void drawBlock(int place, int dx, int dy, DrawData* data);
		/// Can this block slide in the given direction?
		bool canSlide(int place, bool dir);
		/// Can the player collect the object at the given location?
		bool canCollect(int place);
		/// Make the player collect the object at the given location.
		void collect(int place, int player_place, DrawData* data);
		/// Make the object go to the given tile.
		void goTo(int place, int source, DrawData* data);
		/// Returns <priority, move>, where priority is -1 if no moves with priority >= step exist.
		std::pair<int, int> movePriority(int place, int step);
		/// Makes a move.
		void makeMove(int place, int move, DrawData* data);

		/// Objects should be created with the "load" factory function, or by copying existing objects.
		Level();

	public:
		/// Loads the level from the given file.
		static std::optional<Level> load(const std::string& level_path);
		/// Updates the level, when all players are making the given move.
		DrawData update(int move);
		/// String used in the overlay.
		std::string getOverlayString();
		/// Returns 0 if the game is continuing, 1 if the player has won, 2 if the player has lost.
		int getState();
		/// Prints the state of the level.
		void printState();
};

#endif // __GAME_GAME_H_
