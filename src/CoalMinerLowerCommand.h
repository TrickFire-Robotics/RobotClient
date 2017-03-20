#ifndef COALMINERLOWERCOMMAND_H_
#define COALMINERLOWERCOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class CoalMinerLowerCommand: public Command {
public:
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Coal Miner Lower Command";
	}
};
}

#endif
