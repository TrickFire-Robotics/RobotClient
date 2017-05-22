#include "ConveyorDumpCommand.h"

namespace trickfire {
ConveyorDumpCommand::ConveyorDumpCommand(double s) : Command(), speed(s) {

}

void ConveyorDumpCommand::OnStart() {

}

void ConveyorDumpCommand::OnFinish() {
	RobotIO::SetMotor(CONVEYOR, 0.0);
}

void ConveyorDumpCommand::Update() {
	if (this->IsRunning()) {
		RobotIO::SetMotor(CONVEYOR, -speed);
	} else {
		RobotIO::SetMotor(CONVEYOR, 0.0);
	}
}

bool ConveyorDumpCommand::IsFinished() {
	return false; // TODO: Implement
}
}
