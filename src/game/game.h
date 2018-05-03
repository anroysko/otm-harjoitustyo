#ifndef __GAME_GAME_H_
#define __GAME_GAME_H_

#include <optional>		     // std::optional
#include <string>		     // std::string
#include <vector>		     // std::vector
#include "./../graphics/graphics.h"  // DrawData

/// @file=game.h

class Level {
private:
	/// Name of the level
	std::string name;
	/// Width of the level
	int width;
	/// Height of the level
	int height;
	/// Two-dimensional array that stores which block is where
	std::vector<int> state;

	/// Player dx
	int pdx = 0;
	/// Player dy
	int pdy = 0;

	/// Can player move to this tile
	bool playerCanMoveTo(int i);
	/// Can object at tile i move up
	bool canMoveUp(int i, std::vector<bool> &moved);
	/// Can object at tile i move left
	bool canMoveLeft(int i, std::vector<bool> &moved);
	/// Can object at tile i move right
	bool canMoveRight(int i, std::vector<bool> &moved);
	/// Can object at tile i move down
	bool canMoveDown(int i, std::vector<bool> &moved);
	/// Can object at tile i fall
	bool canFall(int i, std::vector<bool> &moved);
	/// Can object at tile i slide left
	bool canSlideLeft(int i, std::vector<bool> &moved);
	/// Can object at tile i slide right
	bool canSlideRight(int i, std::vector<bool> &moved);

public:
	/// Amount of emeralds collected.
	int current_score;
	/// Amount of emeralds needed.
	int needed_score;
	/// Current time.
	int current_time;
	/// Time limit.
	int time_limit;
	/// Returns name of the level
	std::string getOverlayString();

	/// Initializes to default values
	Level() = default;
	/// State has block types in it.
	/// x + y * width = i
	Level(int width, int height, int current_score, int needed_score, std::vector<int> &state);
	/// Parses a level from the given path.
	/// Returns std::nullopt if parsing failed
	static std::optional<Level> parse(const std::string &level_path);

	/// Make a time step.
	/// Returns what to draw
	/// Takes as input a move const from keystate.h
	DrawData update(int move);
	/// Has the player won?
	bool playerWon();
};

#endif  // __GAME_GAME_H_
