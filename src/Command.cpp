#include "Command.h"
#include <unistd.h>

namespace trickfire {
vector<Command*> Command::runningCommands;

Command::Command() :
		runningTime(0.0f), deltaTime(0.0f), _startTime(0.0f), _prevTime(0.0f), running(false), sfmlThread(&Command::CommandThreadLoop, this) {

}

void Command::CommandThreadLoop(Command * command) {
	while (!command->IsFinished() && command->running) {
		long timediff = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()).count()
				- command->_prevTime;
		command->_prevTime = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()).count();
		command->deltaTime = ((float) timediff) / 1000;
		command->runningTime = ((float) (duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()).count()
				- command->_startTime)) / 1000;
		command->UpdatePeriodic();
	}

	command->Stop();
}

void Command::StopAllCommands() {
	while (runningCommands.size() == 0) {
		runningCommands[0]->Stop();
	}
}

void Command::Start() {
	runningCommands.push_back(this);
	sfmlThread.launch();
	_startTime = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()).count();
	_prevTime = _startTime;
	runningTime = 0.0f;
	deltaTime = 0.0f;
	running = true;
	OnInitialize();
}

void Command::Stop() {
	running = false;
	OnFinish();

	CleanUpList();
}

void Command::OnInitialize() {

}

void Command::OnFinish() {

}

bool Command::IsFinished() {
	return false;
}

void Command::UpdatePeriodic() {
	cout << GetDisplayName() << ": " << runningTime << "s" << endl;
	usleep(100000);
}

string Command::GetDisplayName() {
	return "[Generic Command]";
}

void Command::CleanUpList() {
	runningCommands.erase(
			std::remove_if(runningCommands.begin(), runningCommands.end(),
					Command::IsDone));
}
}
