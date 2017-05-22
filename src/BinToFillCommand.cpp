#include "BinToFillCommand.h"

namespace trickfire {
BinToFillCommand::BinToFillCommand(double time) : time(time) {

}

void BinToFillCommand::OnStart() {

}

void BinToFillCommand::OnFinish() {
	RobotIO::SetMotor(BIN_SLIDE, 0.0);
}

void BinToFillCommand::Update() {
	RobotIO::SetMotor(BIN_SLIDE, -1.0);
}

bool BinToFillCommand::IsFinished() {
	return (time > 0) ? this->GetRunningTime() > time : false;
}
}
