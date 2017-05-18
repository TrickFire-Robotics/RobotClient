#include "CoalMinerS1LowerCommand.h"

namespace trickfire {
CoalMinerS1LowerCommand::CoalMinerS1LowerCommand() : Command(), mod(0.8) {

}

void CoalMinerS1LowerCommand::OnStart() {
	
}

void CoalMinerS1LowerCommand::OnFinish() {
	RobotIO::SetMotor(MINER_MOVE_LOWER_LEFT, 0.0);
	RobotIO::SetMotor(MINER_MOVE_LOWER_RIGHT, 0.0);
}

void CoalMinerS1LowerCommand::Update() {
	RobotIO::SetMotor(MINER_MOVE_LOWER_LEFT, mod);
	RobotIO::SetMotor(MINER_MOVE_LOWER_RIGHT, 1.0);
}

bool CoalMinerS1LowerCommand::IsFinished() {
	return false; // TODO: Implement
}
}
