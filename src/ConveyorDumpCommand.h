#ifndef CONVEYORDUMPCOMMAND_H_
#define CONVEYORDUMPCOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class ConveyorDumpCommand: public Command {
public:
	ConveyorDumpCommand(double s = 1.0);
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Conveyor Dump Command";
	}
	double speed;
};
}

#endif
