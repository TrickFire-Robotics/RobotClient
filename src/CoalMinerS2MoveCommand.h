#ifndef COALMINERS2MOVECOMMAND_H_
#define COALMINERS2MOVECOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class CoalMinerS2MoveCommand: public Command {
public:
	CoalMinerS2MoveCommand(double l = 1.0, double r = 1.0);
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Coal Miner S2 Move Command";
	}
	double left, right;
};
}

#endif
