#include "BinToDumpCommand.h"

namespace trickfire {
BinToDumpCommand::BinToDumpCommand(double time) : time(time) {

}

void BinToDumpCommand::OnStart() {

}

void BinToDumpCommand::OnFinish() {
	RobotIO::SetMotor(BIN_SLIDE, 0.0);
}

void BinToDumpCommand::Update() {
	RobotIO::SetMotor(BIN_SLIDE, 1.0);
}

bool BinToDumpCommand::IsFinished() {
	return (time > 0) ? this->GetRunningTime() > time : false;
}
}
