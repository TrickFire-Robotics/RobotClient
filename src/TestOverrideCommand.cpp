#include "TestOverrideCommand.h"
#include "RobotIO.h"
#include "Driver.h"

namespace trickfire {

	void TestOverrideCommand::UpdatePeriodic() {
		RobotIO::SimpleArcade(0.0, 1.0);
	}

	string TestOverrideCommand::GetDisplayName() {
		return "Test Override Command";
	}

	bool TestOverrideCommand::IsFinished() {
		return GetRunningTime() >= 5.0f;
	}

	void TestOverrideCommand::OnFinish() {
		RobotIO::SimpleArcade(0.0, 0.0);
		Driver::OnNonDefaultDriveFinish();
	}
}
