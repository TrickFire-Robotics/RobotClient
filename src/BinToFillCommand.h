#ifndef BINTOFILLCOMMAND_H_
#define BINTOFILLCOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class BinToFillCommand: public Command {
public:
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Bin 2 Fill Position Command";
	}
};
}

#endif
