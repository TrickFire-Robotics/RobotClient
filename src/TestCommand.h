#ifndef TESTCOMMAND_H_
#define TESTCOMMAND_H_

#include "Command.h"

namespace trickfire {

class TestCommand: public Command {
protected:
	void Update() override;
	bool IsFinished() override;
};
}

#endif
