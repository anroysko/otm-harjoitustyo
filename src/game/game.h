#ifndef __GAME_GAME_H_
#define __GAME_GAME_H_

#include <vector>
#include <string>
#include "./../graphics/graphics.h"
#include <optional>

class Level {
private:
	std::string name;
	int width;
	int height;
	int current_score;
	int needed_score;
	int current_time;
	int time_limit;
	std::vector<int> state;  // Block type for this block

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
	Level() = default;
	Level(int width, int height, int current_score, int needed_score, std::vector<int> &state);
	static std::optional<Level> parse(std::string level_path);
	
	DrawData update(int move);
	bool playerWon();
};

int add(int a, int b);

#endif  // __GAME_GAME_H_
