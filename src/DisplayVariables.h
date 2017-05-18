#ifndef DISPLAYVARIABLES_H_
#define DISPLAYVARIABLES_H_

#include <SFML/System.hpp>

namespace trickfire {

class DisplayVariables {
public:
	static float GetLeft();
	static float GetRight();

	static void SetLeft(float drive);
	static void SetRight(float rot);
private:
	static sf::Mutex mutex_all;

	static float left;
	static float right;
};
}

#endif
