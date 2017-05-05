#include "CoalMinerS2RaiseCommand.h"

namespace trickfire {
void CoalMinerS2RaiseCommand::OnStart() {

}

void CoalMinerS2RaiseCommand::OnFinish() {
	RobotIO::SetMotor(MINER_MOVE_UPPER_LEFT, 0.0);
	RobotIO::SetMotor(MINER_MOVE_UPPER_RIGHT, 0.0);
}

void CoalMinerS2RaiseCommand::Update() {
	RobotIO::SetMotor(MINER_MOVE_UPPER_LEFT, -1.0);
	RobotIO::SetMotor(MINER_MOVE_UPPER_RIGHT, -1.0);
}

bool CoalMinerS2RaiseCommand::IsFinished() {
	return false; // TODO: Implement
}
}
