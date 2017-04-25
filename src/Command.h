#ifndef COMMAND_H_
#define COMMAND_H_

#include <vector>
#include <SFML/System.hpp>
#include <iostream>
#include <chrono>
#include <unistd.h>

#include "Logger.h"

namespace trickfire {

class Command {
public:
	Command();
	virtual ~Command();

	void Start();
	bool IsRunning() { return running; }
	void Stop();
	float GetRunningTime();

	static inline void KillAll() { killAll = true; }

	virtual std::string GetCommandName() = 0;
protected:
	virtual void OnStart() = 0;
	virtual void Update() = 0;
	virtual bool IsFinished() = 0;
	virtual void OnFinish();
	float GetDeltaTime();
private:
	long long _startTime;
	long long _prevTime;
	float deltaTime;
	sf::Thread cmdThread;
	sf::Mutex runningMutex;
	bool running;

	static bool killAll;

	static void ThreadMethod(Command * command);
};
}

#endif
