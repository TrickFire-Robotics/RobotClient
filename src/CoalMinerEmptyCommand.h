#ifndef COALMINEREMPTYCOMMAND_H_
#define COALMINEREMPTYCOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class CoalMinerEmptyCommand: public Command {
public:
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Coal Miner Empty Command";
	}
};
}

#endif
