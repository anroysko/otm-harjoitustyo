#include <iostream>
#include <utility>
#include "./../game/game.h"
#include "./../graphics/graphics.h"

std::pair<int, int> testGame() {
	std::vector<int> test_level_data = {
		7, 7, 7, 7, 7, 7, 7, 7,
		7, 1, 1, 1, 1, 1, 5, 7,
		7, 3, 2, 3, 2, 3, 2, 7,
		7, 2, 3, 2, 3, 2, 3, 7,
		7, 3, 2, 3, 2, 3, 2, 7,
		7, 2, 3, 2, 3, 2, 3, 7,
		7, 1, 1, 1, 1, 1, 1, 7,
		7, 1, 1, 7, 7, 1, 1, 7,
		7, 8, 1, 1, 1, 1, 1, 7,
		7, 7, 7, 7, 7, 7, 7, 7};
	std::vector<int> moves = {
		0, 2, 2, 2, 2, 2, 4, 1,
		4, 2, 4, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1,
		2, 2, 4, 4, 4, 1, 4, 1,
		1, 1, 1, 3, 2, 3, 2, 3,
		2, 3, 2, 3, 1, 1, 1, 4,
		3, 1, 3, 2, 0, 0, 0, 0,
		1, 4, 4, 4, 4, 4, 4, 2,
		2, 2, 2, 2, 0};
	// width, height, current_score, needed_score
	Level test_level(8, 10, 0, 10, test_level_data);
	DrawData data = test_level.update(MOVE_NONE);

	std::unique_ptr<GraphicsState> state = GraphicsState::create();
	if (state == nullptr) {
		std::cout << "Failed to create graphics state\n";
		return {0, 1};
	}

	int ticks = 0;
	int ticks_per_move = 1;
	int current_step = 0;
	state->setDraw(data);
	while (true) {
		state->draw(ticks, ticks_per_move, data);
		++ticks;
		if (ticks == ticks_per_move) {
			int move = moves[current_step];
			++current_step;
			data = test_level.update(move);
			state->setDraw(data);
			ticks = 0;
		}
		if (state->shouldQuit() || (current_step == moves.size())) break;
	}
	if (test_level.playerWon()) return {1, 1};
	return {0, 1};
}

std::string strStars(int count) {
	std::string res(count, '*');
	return res;
}
std::string indent(int count) {
	std::string res(count, '\t');
	return res;
}

std::pair<int, int> testAddition(int inds) {
	std::cout << indent(inds) << "Testing add(int a, int b)\n";
	int correct = 0;
	int total = 2;
	if (add(3, 5) == 3 + 5) ++correct;
	if (add(3, -3) == 3 - 3) ++correct;
	std::cout << indent(1 + inds) << correct << "/" << total << " of tests correct\n";
	return {correct, total};
}

std::pair<int, int> testMultiply(int inds) {
	std::cout << indent(inds) << "Testing multiply(int a, int b)\n";
	int correct = 0;
	int total = 2;
	if (multiply(3, 5) == 3 * 5) ++correct;
	if (multiply(3, -3) == 3 * -3) ++correct;
	std::cout << indent(1 + inds) << correct << "/" << total << " of tests correct\n";
	return {correct, total};
}

int main() {
	std::cout << '\n';
	std::cout << strStars(20) << " TESTING " << strStars(20) << '\n';

	std::pair<int, int> tmp;
	int correct = 0;
	int total = 0;

	std::cout << "Testing add and multiply\n";
	tmp = testAddition(1);
	correct += tmp.first;
	total += tmp.second;

	tmp = testMultiply(1);
	correct += tmp.first;
	total += tmp.second;

	std::cout << "Testing game\n";
	tmp = testGame();
	std::cout << indent(1) << tmp.first << "/" << tmp.second << " of tests correct\n";
	correct += tmp.first;
	total += tmp.second;

	std::cout << correct << "/" << total << " of tests correct\n";

	std::cout << strStars(18) << " END TESTING " << strStars(18) << '\n';
	std::cout << '\n';
}
