#include "./../game/game.h"
#include "./../graphics/graphics.h"
#include <iostream>

int main() {
	int correct = 0;
	int total = 2;
	if (add(3, 5) == 3+5) ++correct;
	if (multiply(3, 5) == 15) ++correct;
	std::cout << correct << "/" << total << " of tests correct\n";
}
