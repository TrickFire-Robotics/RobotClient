#include "StandardDriveCommand.h"

namespace trickfire {
void StandardDriveCommand::Update() {
	std::cout << "Updating drive! " << forward << ", " << rot << std::endl;
	RobotIO::SimpleArcade(forward, rot);
}

bool StandardDriveCommand::IsFinished() {
	return false;
}
}
