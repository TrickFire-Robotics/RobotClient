#ifndef AUTODRIVECOMMAND1_H_
#define AUTODRIVECOMMAND1_H_

#include "Command.h"
#include "RobotIO.h"
#include "Main.h"

namespace trickfire {

class AutoDriveCommand1: public Command {
public:
	void OnStart() override;
	void Update() override;
	bool IsFinished() override;
	void OnFinish() override;
};
}

#endif
