#ifndef __GAME_GAME_H_
#define __GAME_GAME_H_

#include <vector>
#include "./../graphics/graphics.h"

class Level {
private:
	int width;
	int height;
	int current_score;
	int needed_score;
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
	Level(int width, int height, int current_score, int needed_score, std::vector<int> &state);
	DrawData update(int move);
};

int add(int a, int b);

#endif  // __GAME_GAME_H_
