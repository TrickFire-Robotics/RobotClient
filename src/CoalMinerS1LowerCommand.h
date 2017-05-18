#ifndef COALMINERS1LOWERCOMMAND_H_
#define COALMINERS1LOWERCOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class CoalMinerS1LowerCommand: public Command {
public:
	CoalMinerS1LowerCommand();
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Coal Miner S1 Lower Command";
	}
	double mod;
};
}

#endif
