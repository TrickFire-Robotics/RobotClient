#include "Command.h"

#define CURRENT_TIME std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
#define TIME_DIVIDER 1000000

namespace trickfire {
std::vector<Command *> Command::runningCommands;
sf::Mutex Command::mutex_runningCommands;

Command::Command() :
		_startTime(0.0f), _prevTime(0.0f), deltaTime(0.0f), cmdThread(
				&Command::ThreadMethod, this), running(false) {

}

Command::~Command() {

}

void Command::Start() {
	mutex_runningCommands.lock();
	runningCommands.push_back(this);
	mutex_runningCommands.unlock();
	_startTime = CURRENT_TIME;
	_prevTime = CURRENT_TIME;
	running = true;
	cmdThread.launch();
}

void Command::Stop() {
	//std::cout << "Stopping" << std::endl;
	running = false;
	OnFinish();

	CleanUpRunningCommList();
}

void Command::OnFinish() {

}

void Command::ThreadMethod(Command * command) {
	while (command->running && !command->IsFinished()) {
		long timediff = CURRENT_TIME- command->_prevTime;
		command->deltaTime = (float) timediff / TIME_DIVIDER;
		command->_prevTime = CURRENT_TIME;

		command->Update();
	}
	if (command->running) { // To prevent a doublestop (heyyy, drumming joke)
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

float Command::GetRunningTime() {
	if (!running)
		return 0.0f;
	return ((float) (CURRENT_TIME - _startTime)) / TIME_DIVIDER;
}

float Command::GetDeltaTime() {
	if (!running)
		return 0.0f;
	return deltaTime;
}
}
