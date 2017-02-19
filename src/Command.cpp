#include "Command.h"

#define CURRENT_TIME std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
#define TIME_DIVIDER 1000000

namespace trickfire {
bool Command::killAll;

Command::Command() :
		_startTime(0.0f), _prevTime(0.0f), deltaTime(0.0f), cmdThread(
				&Command::ThreadMethod, this), running(false) {
	cmdThread.launch();
}

Command::~Command() {

}

void Command::Start() {
	if (!running) {
		Logger::Log(Logger::LEVEL_INFO_FINE, "Starting command");
		_startTime = CURRENT_TIME;
		_prevTime = CURRENT_TIME;
		running = true;
	} else {
		Logger::Log(Logger::LEVEL_INFO_FINE, "Starting command (was already running)");
	}

	OnStart();
}

void Command::Stop() {
	Logger::Log(Logger::LEVEL_INFO_FINE, "Stopping command");
	if (running) {
		running = false;
		OnFinish();
	}
	_startTime = 0.0f;
	_prevTime = 0.0f;
	deltaTime = 0.0f;
}

void Command::OnFinish() {

}

void Command::ThreadMethod(Command * command) {
	while (!Command::killAll) {
		bool needsToStop = false;
		while (!Command::killAll && command->running && !command->IsFinished()) {
			needsToStop = true;
			long timediff = CURRENT_TIME- command->_prevTime;
			command->deltaTime = (float) timediff / TIME_DIVIDER;
			command->_prevTime = CURRENT_TIME;

			command->Update();
		}
		if (needsToStop && command->running) { // To prevent a doublestop (heyyy, drumming joke)
			command->Stop();
			needsToStop = false;
		}
	}
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
