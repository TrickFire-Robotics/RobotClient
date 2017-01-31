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

	void Start();
	bool IsRunning() { return running; }
	void Stop();
	float GetRunningTime();

	static inline void KillAll() { killAll = true; }
protected:
	virtual void OnStart() = 0;
	virtual void Update() = 0;
	virtual bool IsFinished() = 0; // TODO: Better names to distinguish between "finished" and "running"
	static bool _IsDone(Command * comm) { return !comm->IsRunning(); }
	virtual void OnFinish();
	float GetDeltaTime();
private:
	long _startTime;
	long _prevTime;
	float deltaTime;
	sf::Thread cmdThread;
	bool running;

	static bool killAll;

	static void ThreadMethod(Command * command);
};
}

#endif
