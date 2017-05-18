#include "CoalMinerS1RaiseCommand.h"

namespace trickfire {
CoalMinerS1RaiseCommand::CoalMinerS1RaiseCommand() : Command(), mod(0.8) {

}

void CoalMinerS1RaiseCommand::OnStart() {
	
}

void CoalMinerS1RaiseCommand::OnFinish() {
	RobotIO::SetMotor(MINER_MOVE_LOWER_LEFT, 0.0);
	RobotIO::SetMotor(MINER_MOVE_LOWER_RIGHT, 0.0);
}

void CoalMinerS1RaiseCommand::Update() {
	RobotIO::SetMotor(MINER_MOVE_LOWER_LEFT, -mod);
	RobotIO::SetMotor(MINER_MOVE_LOWER_RIGHT, -1.0);
}

bool CoalMinerS1RaiseCommand::IsFinished() {
	return false; // TODO: Implement
}
}
