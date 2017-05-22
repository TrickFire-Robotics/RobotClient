#ifndef BINTODUMPCOMMAND_H_
#define BINTODUMPCOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class BinToDumpCommand: public Command {
public:
	BinToDumpCommand(double time = 0);
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Bin To Dump Position Command";
	}
private:
	double time;
};
}

#endif
