#ifndef DISPLAYVARIABLES_H_
#define DISPLAYVARIABLES_H_

#include <SFML/System.hpp>

namespace trickfire {

class DisplayVariables {
public:
	static float GetDrive();
	static float GetRot();

	static void SetDrive(float drive);
	static void SetRot(float rot);
private:
	static sf::Mutex mutex_all;

	static float drive;
	static float rot;
};
}

#endif
