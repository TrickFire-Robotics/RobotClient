#include "CoalMinerLowerCommand.h"

namespace trickfire {
void CoalMinerLowerCommand::OnStart() {

}

void CoalMinerLowerCommand::OnFinish() {
	RobotIO::SetMotor(MINER_RAISE_LOWER, 0.0);
}

void CoalMinerLowerCommand::Update() {
	RobotIO::SetMotor(MINER_RAISE_LOWER, 1.0);
}

bool CoalMinerLowerCommand::IsFinished() {
	return true; // TODO: Implement
}
}
