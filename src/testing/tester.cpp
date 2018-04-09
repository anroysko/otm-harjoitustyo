#include "./../game/game.h"
#include "./../graphics/graphics.h"
#include <iostream>
#include <utility>

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
	if (add(3, 5) == 3+5) ++correct;
	if (add(3, -3) == 3-3) ++correct;
	std::cout << indent(1 + inds) << correct << "/" << total << " of tests correct\n";
	return {correct, total};
}

std::pair<int, int> testMultiply(int inds) {
	std::cout << indent(inds) << "Testing multiply(int a, int b)\n";
	int correct = 0;
	int total = 2;
	if (multiply(3, 5) == 3*5) ++correct;
	if (multiply(3, -3) == 3*-3) ++correct;
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

	std::cout << correct << "/" << total << " of tests correct\n";
	
	std::cout << strStars(18) << " END TESTING " << strStars(18) << '\n';
	std::cout << '\n';
}
