#ifndef __GAME_GAME_H_
#define __GAME_GAME_H_

#include "./../graphics/graphics.h"
#include <vector>

class Level {
	private:
		int width;
		int height;
		int current_score;
		int needed_score;
		std::vector<int> state; // Block type for this block

		int pdx = 0;
		int pdy = 0;
		
		bool can_move_up(int i, std::vector<bool>& moved);
		bool can_move_left(int i, std::vector<bool>& moved);
		bool can_move_right(int i, std::vector<bool>& moved);
		bool can_move_down(int i, std::vector<bool>& moved);
		bool can_fall(int i, std::vector<bool>& moved);
		bool can_slide_left(int i, std::vector<bool>& moved);
		bool can_slide_right(int i, std::vector<bool>& moved);
	public:
		Level(int width, int height, int current_score, int needed_score, std::vector<int>& state);
		DrawData update(int move);
};

int add(int a, int b);

#endif // __GAME_GAME_H_
