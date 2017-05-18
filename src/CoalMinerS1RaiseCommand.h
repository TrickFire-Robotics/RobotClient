#ifndef COALMINERS1RAISECOMMAND_H_
#define COALMINERS1RAISECOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class CoalMinerS1RaiseCommand: public Command {
public:
	CoalMinerS1RaiseCommand();
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Coal Miner S1 Raise Command";
	}
	double mod;
};
}

#endif
