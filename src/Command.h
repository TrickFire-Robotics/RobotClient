#ifndef COMMAND_H_
#define COMMAND_H_

#include <vector>
#include <SFML/System.hpp>
#include <iostream>
#include <chrono>
#include <unistd.h>

namespace trickfire {

class Command {
public:
	Command();
	virtual ~Command();

	static std::vector<Command *> runningCommands;
	static sf::Mutex mutex_runningCommands;

	void Start();
	bool IsRunning() { return running; }
	void Stop();
	float GetRunningTime();
protected:
	virtual void Update() = 0;
	virtual bool IsFinished() = 0; // TODO: Better names to distinguish between "finished" and "running"
	static bool _IsDone(Command * comm) { return !comm->IsRunning(); }
	void OnFinish();
	float GetDeltaTime();
private:
	long _startTime;
	long _prevTime;
	float deltaTime;
	sf::Thread cmdThread;
	bool running;

	static void ThreadMethod(Command * command);

	static void CleanUpRunningCommList();
};
}

#endif
