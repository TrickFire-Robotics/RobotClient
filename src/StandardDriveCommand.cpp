#include "StandardDriveCommand.h"

namespace trickfire {
void StandardDriveCommand::Update() {
	RobotIO::SimpleArcade(forward, rot);
}

bool StandardDriveCommand::IsFinished() {
	return false;
}
}
