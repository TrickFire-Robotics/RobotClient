#include "ArucoDriveCommand.h"

namespace trickfire {

#define SIDE 120

void ArucoDriveCommand::OnStart() {

}

void ArucoDriveCommand::Update() {
	std::cout << "Aruco!" << std::endl;
	mutex_aruco.lock();
	if (centerX >= 0) {
		if (centerX - 340 > SIDE) {
			RobotIO::SimpleArcade(0.0, 0.25);
		} else if (centerX - 340 < -SIDE) {
			RobotIO::SimpleArcade(0.0, -0.25);
		} else {
			RobotIO::SimpleArcade(0.0, 0.0);
		}
	} else {
		RobotIO::SimpleArcade(0.0, 0.0);
	}
	mutex_aruco.unlock();
}

bool ArucoDriveCommand::IsFinished() {
	return false;
}

void ArucoDriveCommand::OnFinish() {
	RobotIO::SimpleArcade(0.0, 0.0);
	Main::ResumeStandardDrive();
}
}
