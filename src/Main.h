#ifndef MAIN_H_
#define MAIN_H_

#include "Client.h"
#include "NetworkingConstants.h"
#include "DrawingUtil.h"

#include "RobotIO.h"

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

namespace trickfire {

class Main {
public:
	static void Start();

private:
	static double forwards, rotation;

	static void OnClientMessageReceived(Packet& packet);

	static void SfmlWindowThread();

	static void DrawTrickFireHeader(Font& font, RenderWindow& window);
};
}

#endif
