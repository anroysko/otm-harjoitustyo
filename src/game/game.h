#ifndef __GAME_GAME_H_
#define __GAME_GAME_H_

#include <optional>
#include <string>
#include <vector>
#include "./../graphics/graphics.h"

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

	int pdx = 0;
	int pdy = 0;

	bool playerCanMoveTo(int i);
	bool canMoveUp(int i, std::vector<bool> &moved);
	bool canMoveLeft(int i, std::vector<bool> &moved);
	bool canMoveRight(int i, std::vector<bool> &moved);
	bool canMoveDown(int i, std::vector<bool> &moved);
	bool canFall(int i, std::vector<bool> &moved);
	bool canSlideLeft(int i, std::vector<bool> &moved);
	bool canSlideRight(int i, std::vector<bool> &moved);

public:
	int current_score;
	int needed_score;
	int current_time;
	int time_limit;
	std::string getOverlayString();

	Level() = default;
	Level(int width, int height, int current_score, int needed_score, std::vector<int> &state);
	static std::optional<Level> parse(const std::string& level_path);

	DrawData update(int move);
	bool playerWon();
};

int add(int a, int b);

#endif  // __GAME_GAME_H_
