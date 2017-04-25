#include "ConveyorDumpCommand.h"

namespace trickfire {
void ConveyorDumpCommand::OnStart() {

}

void ConveyorDumpCommand::OnFinish() {
	RobotIO::SetMotor(CONVEYOR, 0.0);
	std::cout << "Finishing CDC" << std::endl;
}

void ConveyorDumpCommand::Update() {
	if (this->IsRunning()) {
		RobotIO::SetMotor(CONVEYOR, 1.0);
	} else {
		RobotIO::SetMotor(CONVEYOR, 0.0);
	}
	std::cout << "Updating CDC" << std::endl;
}

bool ConveyorDumpCommand::IsFinished() {
	return false; // TODO: Implement
}
}
