#include "AutoDriveCommand1.h"

namespace trickfire {
	void AutoDriveCommand1::Update() {
		//std::cout << "Updating ADC1" << std::endl;
		RobotIO::SimpleArcade(0.25, 0.0);
	}

	bool AutoDriveCommand1::IsFinished() {
		return GetRunningTime() > 3.0;
	}

	void AutoDriveCommand1::OnFinish() {
		std::cout << "Finishing ADC1" << std::endl;
		RobotIO::SimpleArcade(0.0, 0.0);
		Main::ResumeStandardDrive();
	}
}
