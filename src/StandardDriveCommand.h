#ifndef STANDARDDRIVECOMMAND_H_
#define STANDARDDRIVECOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class StandardDriveCommand: public Command {
private:
	double * forwards;
	double * rotation;

protected:
	void UpdatePeriodic() override;

	void OnFinish() override;

public:
	StandardDriveCommand(double * forw, double * rot);

	inline bool IsFinished() override {
		return false;
	}

	inline string GetDisplayName() override {
		return "Standard Drive";
	}
};
}

#endif
