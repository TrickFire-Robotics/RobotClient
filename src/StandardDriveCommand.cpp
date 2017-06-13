#include "StandardDriveCommand.h"

namespace trickfire {
void StandardDriveCommand::OnStart() {

}

void StandardDriveCommand::Update() {
	RobotIO::SimpleTank(left, right, fl, rl, fr, rr);
}

bool StandardDriveCommand::IsFinished() {
	return false;
}
}
