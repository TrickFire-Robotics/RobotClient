#include "ConveyorDumpCommand.h"

namespace trickfire {
void ConveyorDumpCommand::OnStart() {

}

void ConveyorDumpCommand::OnFinish() {
	RobotIO::SetMotor(CONVEYOR, 0.0);
}

void ConveyorDumpCommand::Update() {
	RobotIO::SetMotor(CONVEYOR, 1.0);
}

bool ConveyorDumpCommand::IsFinished() {
	return true; // TODO: Implement
}
}
