#include <iostream>
#include "./game/game.h"
#include "./graphics/graphics.h"
#include "./graphics/keystate.h"
#include "./util/file_io.h"
#include <optional>
#include <utility>

int main() {
	std::cout << "\n";
	std::cout << "***** generic emerald mining game! *****\n";
	std::cout << "\n";
	std::cout << "Please select level\n";

	std::string level_path = promptFile();	
	std::optional<Level> tmp = Level::parse(level_path);
	if (! tmp) {
		std::cout << "Failed to parse level\n";
		return 1;
	}
	Level test_level = std::move(tmp.value());
	
	/*
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
	// width, height, current_score, needed_score
	Level test_level(8, 10, 0, 10, test_level_data);
	*/	

	std::vector<int> moves;
	DrawData data = test_level.update(MOVE_NONE);

	// int x, y, dx, dy, tex;
	DrawData text_draw_data;
	Sprite letter_a_sprite (0, 0, 0, 0, 0);
	text_draw_data.sprites.push_back(letter_a_sprite);

	std::unique_ptr<GraphicsState> state = GraphicsState::create();
	if (state == nullptr) {
		std::cout << "Failed to create graphics state\n";
		return 1;
	}

	int ticks = 0;
	int ticks_per_move = 16;
	bool time_limit_elapsed = false;
	state->setLevelDraw(data);
	state->setOverlayDraw(text_draw_data);
	while (true) {
		state->draw(ticks, ticks_per_move, data, text_draw_data);
		++ticks;
		if (ticks == ticks_per_move) {
			int move = state->getMove();
			moves.push_back(move);
			data = test_level.update(move);
			state->setLevelDraw(data);
			++test_level.current_time;
			ticks = 0;
			if (test_level.current_time >= test_level.time_limit) time_limit_elapsed = true;
		}
		if (state->shouldQuit() || test_level.playerWon() || time_limit_elapsed) break;
	}
	if (test_level.playerWon()) {
		std::cout << "You won!\n";
	} else if (time_limit_elapsed) {
		std::cout << "You ran out of time!\n";
	} else {
		std::cout << "You closed the window!\n";
	}
	std::cout << "Time used: " << test_level.current_time << "/" << test_level.time_limit << "\n";
	std::cout << "Emeralds mined: " << test_level.current_score << "/" << test_level.needed_score << "\n";

	if (moves.size() > 0) {
		std::cout << "Replay vector: ";
		std::cout << "{";
		for (int i = 0; i < (int)moves.size() - 1; ++i) std::cout << moves[i] << ", ";
		std::cout << moves.back() << "}\n";
	}
}
