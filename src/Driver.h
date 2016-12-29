#ifndef DRIVER_H_
#define DRIVER_H_

#include "Client.h"
#include "Logger.h"
#include "DrawingUtil.h"
#include "NetworkingConstants.h"
#include "RobotIO.h"
#include "Command.h"
#include "TestOverrideCommand.h"
#include "StandardDriveCommand.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <pthread.h>

using namespace sf;
using namespace std;

namespace trickfire {

class Driver {
public:
	static void Start();
	static void OnNonDefaultDriveFinish();

private:
	static void * WindowThread(void * cl);
	static void ClientMessageCallback(Packet& packet);
	static void DrawTrickFireHeader(Font& font, RenderWindow& window);

	static double forwards;
	static double rotation;

	static StandardDriveCommand standardDrive;
	static Command * driveCommand;
};
}

#endif
