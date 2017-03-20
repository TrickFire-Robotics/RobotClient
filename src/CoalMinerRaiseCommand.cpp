#include "CoalMinerRaiseCommand.h"

namespace trickfire {
void CoalMinerRaiseCommand::OnStart() {

}

void CoalMinerRaiseCommand::OnFinish() {
	RobotIO::SetMotor(MINER_RAISE_LOWER, 0.0);
}

void CoalMinerRaiseCommand::Update() {
	RobotIO::SetMotor(MINER_RAISE_LOWER, 1.0);
}

bool CoalMinerRaiseCommand::IsFinished() {
	return true; // TODO: Implement
}
}
