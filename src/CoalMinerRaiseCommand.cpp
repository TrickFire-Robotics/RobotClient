#include "CoalMinerRaiseCommand.h"

namespace trickfire {
void CoalMinerRaiseCommand::OnStart() {

}

void CoalMinerRaiseCommand::OnFinish() {
	RobotIO::SetMotor(MINER_MOVE_LOWER, 0.0);
	RobotIO::SetMotor(MINER_MOVE_UPPER, 0.0);
}

void CoalMinerRaiseCommand::Update() {
	RobotIO::SetMotor(MINER_MOVE_LOWER, -1.0);
	RobotIO::SetMotor(MINER_MOVE_UPPER, -1.0);
}

bool CoalMinerRaiseCommand::IsFinished() {
	return false; // TODO: Implement
}
}
