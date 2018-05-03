#include <iostream>
#include <fstream>
#include <optional>
#include <utility>
#include "./game/game.h"
#include "./graphics/graphics.h"
#include "./graphics/keystate.h"
#include "./util/file_io.h"

/// @file main.cpp
/// Contains the main-function

/// main-function.
int main() {
	std::cout << "\n";
	std::cout << "***** generic emerald mining game! *****\n";
	std::cout << "\n";
	std::cout << "Please select level\n";

	std::optional<std::string> file_path = promptFile();
	if (!file_path) {
		std::cout << "No level/replay selected. Exiting...\n";
		return 0;
	}

	std::string level_path = *file_path;
	std::ifstream fin;
	fin.open(level_path);
	std::string file_type;
	fin >> file_type;

	bool is_replay = false;
	std::vector<int> moves;

	if (file_type == "REPLAY") {
		is_replay = true;
		fin >> level_path;
		int move_count;
		fin >> move_count;
		moves.resize(move_count);
		for (int i = 0; i < move_count; ++i) fin >> moves[i];
	}
	fin.close();
	
	std::optional<Level> tmp = Level::parse(level_path);
	if (!tmp) {
		std::cout << "Failed to parse level\n";
		return 1;
	}
	Level& level = *tmp;
	DrawData data = level.update(MOVE_NONE);

	// int x, y, dx, dy, tex;
	DrawData text_draw_data;
	text_draw_data.sprites = textToSprites(level.getOverlayString(), -30, -22);

	std::unique_ptr<GraphicsState> state = GraphicsState::create();
	if (state == nullptr) {
		std::cout << "Failed to create graphics state\n";
		return 1;
	}

	int ticks = 0;
	int ticks_per_move = 16;
	int mi = 0;
	bool time_limit_elapsed = false;
	state->setLevelDraw(data);
	state->setOverlayDraw(text_draw_data);
	while (true) {
		state->draw(ticks, ticks_per_move, data, text_draw_data);
		++ticks;
		if (ticks == ticks_per_move) {
			if (! is_replay) {
				moves.push_back(state->getMove());
			} else {
				if (mi == moves.size()) break;
			}
			int move = moves[mi];
			++mi;
			data = level.update(move);
			text_draw_data.sprites = textToSprites(level.getOverlayString(), -30, -22);
			state->setLevelDraw(data);
			state->setOverlayDraw(text_draw_data);
			++level.current_time;
			ticks = 0;
			if (level.current_time >= level.time_limit) time_limit_elapsed = true;
		}
		if (state->shouldQuit() || level.playerWon() || time_limit_elapsed) break;
	}
	state.release(); // Close the window
	if (level.playerWon()) {
		std::cout << "You won!\n";
	} else if (time_limit_elapsed) {
		std::cout << "You ran out of time!\n";
	} else {
		std::cout << "You closed the window!\n";
	}
	std::cout << "Time used: " << level.current_time << "/" << level.time_limit << "\n";
	std::cout << "Emeralds mined: " << level.current_score << "/" << level.needed_score << "\n";

	if (!is_replay && level.playerWon()) {
		std::cout << "Choose where to save replay\n";
		std::optional<std::string> replay_file = promptFile();
		if (replay_file) {
			std::ofstream fout;
			fout.open(*replay_file);
			fout << "REPLAY\n";
			fout << level_path << '\n';
			fout << moves.size() << '\n';
			for (int i = 0; i < moves.size(); ++i) fout << moves[i] << ' ';
			fout << '\n';
			fout.close();
			std::cout << "Created replay file at " << *replay_file << '\n';
		}
	}
}
