#ifndef COALMINERRAISECOMMAND_H_
#define COALMINERRAISECOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class CoalMinerRaiseCommand: public Command {
public:
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Coal Miner Raise Command";
	}
};
}

#endif
