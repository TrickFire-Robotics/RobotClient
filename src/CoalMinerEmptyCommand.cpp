#include "CoalMinerEmptyCommand.h"

namespace trickfire {
	void CoalMinerEmptyCommand::OnStart() {

	}

	void CoalMinerEmptyCommand::OnFinish() {
		RobotIO::SetMotor(MINER_SPIN, 0.0);
	}

	void CoalMinerEmptyCommand::Update() {
		RobotIO::SetMotor(MINER_SPIN, -1.0);
	}

	bool CoalMinerEmptyCommand::IsFinished() {
		return false;
	}
}
