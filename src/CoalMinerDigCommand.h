#ifndef COALMINERDIGCOMMAND_H_
#define COALMINERDIGCOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class CoalMinerDigCommand: public Command {
public:
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Coal Miner Dig Command";
	}
};
}

#endif
