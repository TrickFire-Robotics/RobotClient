#ifndef GYRORESETCOMMAND_H_
#define GYRORESETCOMMAND_H_

#include "Command.h"

namespace trickfire {

class GyroResetCommand: public Command {
public:
	GyroResetCommand();

	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Gyro Reset Command";
	}
	inline void SetZero(double zero) {
		this->zero = zero;
	}
	inline double GetZero() {
		return zero;
	}
	inline void SetTarget(double target) {
		this->target = target;
	}
	inline double GetTarget() {
		return target;
	}
private:
	double zero;
	double target;
	double yawDiff0To360(double offset);
};
}

#endif
