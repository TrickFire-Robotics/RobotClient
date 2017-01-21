#include "TestCommand2.h"

namespace trickfire {
	void TestCommand2::Update() {
		std::cout << "TC2: " << GetRunningTime() << " s" << std::endl;
	}

	bool TestCommand2::IsFinished() {
		return false;
	}
}
