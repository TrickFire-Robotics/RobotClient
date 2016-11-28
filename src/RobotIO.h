#ifndef ROBOTIO_H_
#define ROBOTIO_H_

#include <map>
#include <iostream>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define PSOC_PORT "/dev/ttyACM0"

using namespace std;

namespace trickfire {

class RobotIO {
public:
	static void SetMotor(unsigned char motorId, double value);

	static void Start();

	static void Stop();

	static void SendPSoCByte(const unsigned char val);

	static void SendPSoCBytes(const unsigned char val[]);

	// PWM = 0 to 254, this converts from 127 being zero, 0 being full reverse, and 254 being full forwards
	static inline double PWMToDouble(unsigned char pwm) {
		return (double) (pwm - 127) / 127;
	}

	static inline unsigned char DoubleToPWM(double doub) {
		return (unsigned char)(doub * 127) + 127;
	}

private:
	static map<unsigned char, double> motorValues;

	static pthread_t commThread;

	static int psocFD;

	static void * ThreadLoop(void * pointless);
};
}

#endif
