#ifndef BINTODUMPCOMMAND_H_
#define BINTODUMPCOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class BinToDumpCommand: public Command {
public:
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Bin 2 Dump Position Command";
	}
};
}

#endif
