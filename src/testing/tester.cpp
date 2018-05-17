#include <iostream>		     // std::cout
#include <utility>		     // std::pair
#include "./../game/game.h"	  // Level
#include "./../graphics/graphics.h"  // DrawData

std::string strStars(int count) {
	std::string res(count, '*');
	return res;
}
std::string indent(int count) {
	std::string res(count, '\t');
	return res;
}

// tests that the game works as expected
// The levels cover special cases very well, so just testing like this is hopefully fine?
std::pair<int, int> testGame(const std::vector<std::string>& levels, const std::vector<std::vector<int>>& moves) {
	int total = levels.size();
	int curr = 0;
	for (int i = 0; i < total; ++i) {
		std::optional<Level> tmp = Level::load(levels[i]);
		if (!tmp) {
			std::cout << indent(2) << "Testing " << levels[i] << " Failed, parsing level could not be done.\n";
			continue;
		}
		bool success = true;
		Level& lvl = *tmp;
		lvl.update(MOVE_NONE, 0, 0);
		for (int mv : moves[i]) lvl.update(mv, 0, 0);
		if (lvl.getState() != 1) {
			success = false;
		}
		curr += success;
		if (success == false) {
			std::cout << indent(2) << "Testing " << levels[i] << " Failed, moves didn't lead to victory\n";
		}
	}
	return {curr, total};
}

int main() {
	std::cout << '\n';
	std::cout << strStars(18) << "   TESTING   " << strStars(18) << '\n';

	std::pair<int, int> tmp;
	int correct = 0;
	int total = 0;

	std::cout << "Testing game\n";
	std::vector<std::string> level_names = {"assets/levels/easy/dangerous_bridge.txt", "assets/levels/hard/dangerous_bridge_2.txt", "assets/levels/medium/double_trouble.txt", "assets/levels/tutorial/tutorial_1.txt"};
	std::vector<std::vector<int>> moves = {
		{0, 0, 3, 3, 3, 3, 1, 0, 2, 0, 0, 0, 66, 2, 1, 1, 4, 3, 3, 2, 0, 0, 0, 0, 4, 4, 4, 3, 3, 1, 1, 2, 2, 2, 2, 2, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 1, 0, 0, 34, 66, 0, 0, 0, 0, 0, 2, 2, 1, 4, 3, 0, 0, 4, 4, 3, 3, 1, 1, 2, 2, 2, 2, 0, 0, 0, 0},
		{0, 1, 0, 0, 2, 3, 3, 4, 2, 2, 1, 3, 35, 4, 0, 3, 3, 1, 4, 3, 1, 1, 4, 2, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 1, 0, 3, 2, 2, 2, 3, 1, 4, 4, 4, 1, 0, 67, 2, 2, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 1, 4, 4, 3, 3, 1, 0, 3, 1, 1, 2, 2, 3, 3, 4, 3, 3, 4, 2, 1, 3, 3, 3, 4, 3, 3, 3, 1, 2, 3, 2, 2, 1, 1, 0, 33, 0, 3, 3, 2, 1, 2, 1, 3, 4, 3, 3, 0, 1, 0, 0, 67, 68, 3, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 3, 3, 3, 2, 2, 3, 3, 0, 0, 0}};

	tmp = testGame(level_names, moves);
	std::cout << indent(1) << tmp.first << "/" << tmp.second << " of tests correct\n";
	correct += tmp.first;
	total += tmp.second;

	std::cout << correct << "/" << total << " of tests correct\n";

	std::cout << strStars(18) << " END TESTING " << strStars(18) << '\n';
	std::cout << '\n';
}
