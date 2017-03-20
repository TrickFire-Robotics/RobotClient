#include "CoalMinerDigCommand.h"

namespace trickfire {
	void CoalMinerDigCommand::OnStart() {

	}

	void CoalMinerDigCommand::OnFinish() {
		RobotIO::SetMotor(MINER_SPIN, 0.0);
	}

	void CoalMinerDigCommand::Update() {
		RobotIO::SetMotor(MINER_SPIN, 1.0);
	}

	bool CoalMinerDigCommand::IsFinished() {
		return true; // TODO: Implement
	}
}
