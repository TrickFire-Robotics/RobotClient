#ifndef COALMINERS2RAISECOMMAND_H_
#define COALMINERS2RAISECOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class CoalMinerS2RaiseCommand: public Command {
public:
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Coal Miner S2 Raise Command";
	}
};
}

#endif
