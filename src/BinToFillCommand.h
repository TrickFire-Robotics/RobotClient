#ifndef BINTOFILLCOMMAND_H_
#define BINTOFILLCOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class BinToFillCommand: public Command {
public:
	BinToFillCommand(double time = 0);
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Bin To Fill Position Command";
	}
private:
	double time;
};
}

#endif
