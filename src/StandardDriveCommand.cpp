#include "StandardDriveCommand.h"

namespace trickfire {
void StandardDriveCommand::OnStart() {

}

void StandardDriveCommand::Update() {
	RobotIO::SimpleTank(left, right);
}

bool StandardDriveCommand::IsFinished() {
	return false;
}
}
