#include "BinToFillCommand.h"

namespace trickfire {
void BinToFillCommand::OnStart() {

}

void BinToFillCommand::OnFinish() {
	RobotIO::SetMotor(BIN_SLIDE, 0.0);
}

void BinToFillCommand::Update() {
	RobotIO::SetMotor(BIN_SLIDE, -1.0);
}

bool BinToFillCommand::IsFinished() {
	return false; // TODO: Implement
}
}
