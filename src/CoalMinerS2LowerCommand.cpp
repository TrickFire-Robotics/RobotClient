#include "CoalMinerS2LowerCommand.h"

namespace trickfire {
void CoalMinerS2LowerCommand::OnStart() {

}

void CoalMinerS2LowerCommand::OnFinish() {
	RobotIO::SetMotor(MINER_MOVE_UPPER_LEFT, 0.0);
	RobotIO::SetMotor(MINER_MOVE_UPPER_RIGHT, 0.0);
}

void CoalMinerS2LowerCommand::Update() {
	RobotIO::SetMotor(MINER_MOVE_UPPER_LEFT, 1.0);
	RobotIO::SetMotor(MINER_MOVE_UPPER_RIGHT, 1.0);
}

bool CoalMinerS2LowerCommand::IsFinished() {
	return false; // TODO: Implement
}
}
