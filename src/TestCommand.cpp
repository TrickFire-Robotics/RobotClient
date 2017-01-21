#include "TestCommand.h"

namespace trickfire {
	void TestCommand::Update() {
		std::cout << "TC1: " << GetRunningTime() << " s" << std::endl;
	}

	bool TestCommand::IsFinished() {
		return false;
	}
}
