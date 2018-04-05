#include "./game/game.h"
#include "./graphics/graphics.h"
#include "./graphics/keystate.h"
#include <iostream>

int main() {
	std::vector<int> test_level_data = {
		7,7,7,7,7,7,7,7,
                7,1,1,1,1,1,5,7,
                7,3,2,3,2,3,2,7,
                7,2,3,2,3,2,3,7,
                7,3,2,3,2,3,2,7,
                7,2,3,2,3,2,3,7,
                7,1,1,1,1,1,1,7,
                7,1,1,7,7,1,1,7,
                7,8,1,1,1,1,1,7,
                7,7,7,7,7,7,7,7
	};
	// width, height, current_score, needed_score
	Level test_level (8, 10, 0, 10, test_level_data);
	DrawData data = test_level.update(MOVE_NONE);

	std::unique_ptr<GraphicsState> state = GraphicsState::create();
	if (state == nullptr) {
		std::cout << "Failed to create graphics state\n";
		return 1;
	} else {
		std::cout << "Successfully created graphics state\n";
	}

	int ticks = 0;
	int ticks_per_move = 16;
	state->setDraw(data);
	while(true) {
		state->draw(ticks, ticks_per_move, data);
		++ticks;
		if (ticks == ticks_per_move) {
			int move = state->getMove();
			data = test_level.update(move);
			state->setDraw(data);
			ticks = 0;
		}
		if (state->shouldQuit()) break;
	}
}
