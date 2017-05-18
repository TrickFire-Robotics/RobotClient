#include "DisplayVariables.h"

namespace trickfire {
sf::Mutex DisplayVariables::mutex_all;
float DisplayVariables::left;
float DisplayVariables::right;

float DisplayVariables::GetLeft() {
	sf::Lock lock(DisplayVariables::mutex_all);
	return DisplayVariables::left;
}

float DisplayVariables::GetRight() {
	sf::Lock lock(DisplayVariables::mutex_all);
	return DisplayVariables::right;
}

void DisplayVariables::SetLeft(float l) {
	sf::Lock lock(DisplayVariables::mutex_all);
	DisplayVariables::left = l;
}

void DisplayVariables::SetRight(float r) {
	sf::Lock lock(DisplayVariables::mutex_all);
	DisplayVariables::right = r;
}
}
