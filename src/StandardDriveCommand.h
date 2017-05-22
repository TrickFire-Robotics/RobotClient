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
		this->left = forward;
		this->right = rot;
		mut_val.unlock();
	}

	inline void SetForward(double forward) {
		mut_val.lock();
		this->left = forward;
		mut_val.unlock();
	}

	inline void SetRot(double rot) {
		mut_val.lock();
		this->right = rot;
		mut_val.unlock();
	}

	inline std::string GetCommandName() override {
		return "Standard Drive Command";
	}
private:
	double left, right;
	sf::Mutex mut_val;
};
}

#endif
