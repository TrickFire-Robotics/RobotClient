#ifndef STANDARDDRIVECOMMAND_H_
#define STANDARDDRIVECOMMAND_H_

#include "Command.h"
#include "RobotIO.h"

namespace trickfire {

class StandardDriveCommand: public Command {
public:
	void OnStart() override;
	void Update() override;
	bool IsFinished() override;

	inline void SetVals(double forward, double rot) {
		mut_val.lock();
		this->forward = forward;
		this->rot = rot;
		mut_val.unlock();
	}

	inline void SetForward(double forward) {
		mut_val.lock();
		this->forward = forward;
		mut_val.unlock();
	}

	inline void SetRot(double rot) {
		mut_val.lock();
		this->rot = rot;
		mut_val.unlock();
	}
private:
	double forward, rot;
	sf::Mutex mut_val;
};
}

#endif
