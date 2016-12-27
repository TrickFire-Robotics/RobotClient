#ifndef COMMAND_H_
#define COMMAND_H_

#include <pthread.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

namespace trickfire {

class Command {
public:
	static void StopAllCommands();

	void Start();
	void Stop();
	virtual bool IsFinished();
	virtual string GetDisplayName();

	inline float GetRunningTime() { return runningTime; }

	static vector<Command> runningCommands;
protected:
	virtual void UpdatePeriodic();
	virtual void OnInitialize();
	virtual void OnFinish();
private:
	static void * CommandThreadLoop(void * command);
	float runningTime;
	float deltaTime;
	long _startTime;
	long _prevTime;
	bool running;
	static void CleanUpList();
	inline static bool IsDone(Command cmd) { return !cmd.running; }

	pthread_t thread;
};
}

#endif
