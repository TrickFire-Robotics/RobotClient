#ifndef COALMINERS2LOWERCOMMAND_H_
#define COALMINERS2LOWERCOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class CoalMinerS2LowerCommand: public Command {
public:
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Coal Miner S2 Lower Command";
	}
};
}

#endif
