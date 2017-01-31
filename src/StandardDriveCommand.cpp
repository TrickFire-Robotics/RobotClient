#include "StandardDriveCommand.h"

namespace trickfire {
void StandardDriveCommand::OnStart() {

}

void StandardDriveCommand::Update() {
	RobotIO::SimpleArcade(forward, rot);
}

bool StandardDriveCommand::IsFinished() {
	return false;
}
}
