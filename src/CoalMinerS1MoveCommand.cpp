#include "CoalMinerS1MoveCommand.h"

namespace trickfire {
CoalMinerS1MoveCommand::CoalMinerS1MoveCommand(double l, double r) : Command(), left(l), right(r) {

}

void CoalMinerS1MoveCommand::OnStart() {
	
}

void CoalMinerS1MoveCommand::OnFinish() {
	RobotIO::SetMotor(MINER_MOVE_LOWER_LEFT, 0.0);
	RobotIO::SetMotor(MINER_MOVE_LOWER_RIGHT, 0.0);
}

void CoalMinerS1MoveCommand::Update() {
	RobotIO::SetMotor(MINER_MOVE_LOWER_LEFT, left);
	RobotIO::SetMotor(MINER_MOVE_LOWER_RIGHT, right);
}

bool CoalMinerS1MoveCommand::IsFinished() {
	return false; // TODO: Implement
}
}
