#include "DisplayVariables.h"

namespace trickfire {
sf::Mutex DisplayVariables::mutex_all;
float DisplayVariables::drive;
float DisplayVariables::rot;

float DisplayVariables::GetDrive() {
	sf::Lock lock(DisplayVariables::mutex_all);
	return DisplayVariables::drive;
}

float DisplayVariables::GetRot() {
	sf::Lock lock(DisplayVariables::mutex_all);
	return DisplayVariables::rot;
}

void DisplayVariables::SetDrive(float drive) {
	sf::Lock lock(DisplayVariables::mutex_all);
	DisplayVariables::drive = drive;
}

void DisplayVariables::SetRot(float rot) {
	sf::Lock lock(DisplayVariables::mutex_all);
	DisplayVariables::rot = rot;
}
}
