#include "GyroResetCommand.h"
#include "RobotIO.h"
#include "Main.h"

namespace trickfire {

GyroResetCommand::GyroResetCommand() : zero(0), target(0) {

}

void GyroResetCommand::OnStart() {

}

void GyroResetCommand::OnFinish() {
	RobotIO::SimpleArcade(0.0, 0.0);
	Main::ResumeStandardDrive();
}

double GyroResetCommand::yawDiff0To360(double offset) {
	return fmod(RobotIO::GetGyroYaw() - zero - offset + 360.0, 360.0);
}

void GyroResetCommand::Update() {
	double diff = yawDiff0To360(target) - 180;

	std::cout << diff << std::endl;

	double speed;

	if (180 - abs(diff) > 30.0) {
		speed = 0.75;
	}
	else {
		speed = 0.35;
	}

	if (diff > 0) {
		RobotIO::SimpleArcade(0.0, speed);
	} else {
		RobotIO::SimpleArcade(0.0, -speed);
	}
}

bool GyroResetCommand::IsFinished() {
	return yawDiff0To360(target) < 2.5;
}
}
