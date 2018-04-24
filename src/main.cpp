#include <iostream>
#include "./game/game.h"
#include "./graphics/graphics.h"
#include "./graphics/keystate.h"

int main() {
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
	std::vector<int> moves;
	// width, height, current_score, needed_score
	Level test_level(8, 10, 0, 10, test_level_data);
	DrawData data = test_level.update(MOVE_NONE);
	DrawData empty_data;

	std::unique_ptr<GraphicsState> state = GraphicsState::create();
	if (state == nullptr) {
		std::cout << "Failed to create graphics state\n";
		return 1;
	}

	int ticks = 0;
	int ticks_per_move = 16;
	state->setLevelDraw(data);
	state->setOverlayDraw(empty_data);
	while (true) {
		state->draw(ticks, ticks_per_move, data, empty_data);
		++ticks;
		if (ticks == ticks_per_move) {
			int move = state->getMove();
			moves.push_back(move);
			data = test_level.update(move);
			state->setLevelDraw(data);
			ticks = 0;
		}
		if (state->shouldQuit() || test_level.playerWon()) break;
	}
	std::cout << "Replay vector: ";
	std::cout << "{";
	for (int i = 0; i < (int)moves.size() - 1; ++i) std::cout << moves[i] << ", ";
	std::cout << moves.back() << "}\n";
}
