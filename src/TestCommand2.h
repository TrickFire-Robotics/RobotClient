#ifndef TESTCOMMAND2_H_
#define TESTCOMMAND2_H_

#include "Command.h"

namespace trickfire {

class TestCommand2: public Command {
protected:
	void Update() override;
	bool IsFinished() override;
};
}

#endif
