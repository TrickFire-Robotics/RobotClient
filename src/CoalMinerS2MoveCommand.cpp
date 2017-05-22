#include "CoalMinerS2MoveCommand.h"

namespace trickfire {
CoalMinerS2MoveCommand::CoalMinerS2MoveCommand(double l, double r) : Command(), left(l), right(r) {

}

void CoalMinerS2MoveCommand::OnStart() {

}

void CoalMinerS2MoveCommand::OnFinish() {
	RobotIO::SetMotor(MINER_MOVE_UPPER_LEFT, 0.0);
	RobotIO::SetMotor(MINER_MOVE_UPPER_RIGHT, 0.0);
}

void CoalMinerS2MoveCommand::Update() {
	RobotIO::SetMotor(MINER_MOVE_UPPER_LEFT, left);
	RobotIO::SetMotor(MINER_MOVE_UPPER_RIGHT, right);
}

bool CoalMinerS2MoveCommand::IsFinished() {
	return false; // TODO: Implement
}
}
