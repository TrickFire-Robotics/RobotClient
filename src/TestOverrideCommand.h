#ifndef TESTOVERRIDECOMMAND_H_
#define TESTOVERRIDECOMMAND_H_

#include "Command.h"

namespace trickfire {

class TestOverrideCommand: public Command {
protected:
	virtual void UpdatePeriodic() override;
	virtual string GetDisplayName() override;
	virtual bool IsFinished() override;
	virtual void OnFinish() override;
};
}

#endif
