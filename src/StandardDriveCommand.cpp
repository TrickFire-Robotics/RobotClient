#include "StandardDriveCommand.h"

namespace trickfire {
	StandardDriveCommand::StandardDriveCommand(double * forw, double * rot) : forwards{forw}, rotation{rot} {

	}

	void StandardDriveCommand::UpdatePeriodic() {
		RobotIO::SimpleArcade(*forwards, *rotation);
	}

	void StandardDriveCommand::OnFinish() {
		RobotIO::SimpleArcade(0.0, 0.0);
	}
}
