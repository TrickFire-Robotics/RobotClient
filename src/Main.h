#ifndef MAIN_H_
#define MAIN_H_

#include "Client.h"
#include "NetworkingConstants.h"
#include "DrawingUtil.h"
#include "Logger.h"

#include "RobotIO.h"
#include "Command.h"
#include "DisplayVariables.h"

#include "StandardDriveCommand.h"
#include "AutoDriveCommand1.h"
#include "CameraSendCommand.h"
#include "KinectSendCommand.h"

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "ProjectConstants.h"
#if OPENCV
#include <opencv.hpp>
#endif
#if KINECT
#include <libfreenect.hpp>
#endif

using namespace std;

namespace trickfire {

class Main {
public:
	static void Start();

	static void ResumeStandardDrive();

private:
	static void OnClientMessageReceived(Packet& packet);

	static void SfmlWindowThread();

	static void DrawTrickFireHeader(Font& font, RenderWindow& window);
};
}

#endif
