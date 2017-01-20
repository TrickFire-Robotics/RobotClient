#include "Command.h"

namespace trickfire {
std::vector<Command *> Command::runningCommands;
sf::Mutex Command::mutex_runningCommands;

Command::Command() :
		cmdThread(&Command::ThreadMethod, this), running(false) {

}

Command::~Command() {

}

void Command::Start() {
	mutex_runningCommands.lock();
	runningCommands.push_back(this);
	mutex_runningCommands.unlock();
	running = true;
	cmdThread.launch();
}

void Command::Stop() {
	std::cout << "Stopping" << std::endl;
	running = false;
	OnFinish();

	CleanUpRunningCommList();
}

void Command::OnFinish() {

}

void Command::ThreadMethod(Command * command) {
	while (command->running && !command->IsFinished()) {
		command->Update();
	}
	if (command->running) { // To prevent a doublestop (heyyy, drumming joke)
				<< command->IsFinished() << std::endl;
		command->Stop();
	}

}

void Command::CleanUpRunningCommList() {
	mutex_runningCommands.lock();
	if (runningCommands.size() > 0) {
		runningCommands.erase(
				std::remove_if(runningCommands.begin(), runningCommands.end(),
						Command::_IsDone));
	}
	std::cout << "Cleaned up list, running commands count is now: "
			<< runningCommands.size() << std::endl;
	mutex_runningCommands.unlock();
}
}
