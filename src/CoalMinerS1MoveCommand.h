#ifndef COALMINERS1MOVECOMMAND_H_
#define COALMINERS1MOVECOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class CoalMinerS1MoveCommand: public Command {
public:
	CoalMinerS1MoveCommand(double l = 1.0, double r = 1.0);
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Coal Miner S1 Move Command";
	}
	double left, right;
};
}

#endif
