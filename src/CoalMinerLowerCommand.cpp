#include "CoalMinerLowerCommand.h"

namespace trickfire {
void CoalMinerLowerCommand::OnStart() {

}

void CoalMinerLowerCommand::OnFinish() {
	RobotIO::SetMotor(MINER_MOVE_LOWER, 0.0);
	RobotIO::SetMotor(MINER_MOVE_UPPER, 0.0);
}

void CoalMinerLowerCommand::Update() {
	RobotIO::SetMotor(MINER_MOVE_LOWER, 1.0);
	RobotIO::SetMotor(MINER_MOVE_UPPER, 1.0);
}

bool CoalMinerLowerCommand::IsFinished() {
	return false; // TODO: Implement
}
}
