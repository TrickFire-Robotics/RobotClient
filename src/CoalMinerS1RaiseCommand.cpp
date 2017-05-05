#include "CoalMinerS1RaiseCommand.h"

namespace trickfire {
void CoalMinerS1RaiseCommand::OnStart() {

}

void CoalMinerS1RaiseCommand::OnFinish() {
	RobotIO::SetMotor(MINER_MOVE_LOWER_LEFT, 0.0);
	RobotIO::SetMotor(MINER_MOVE_LOWER_RIGHT, 0.0);
}

void CoalMinerS1RaiseCommand::Update() {
	RobotIO::SetMotor(MINER_MOVE_LOWER_LEFT, -1.0);
	RobotIO::SetMotor(MINER_MOVE_LOWER_RIGHT, -1.0);
}

bool CoalMinerS1RaiseCommand::IsFinished() {
	return false; // TODO: Implement
}
}
