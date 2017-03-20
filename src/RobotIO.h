#ifndef ROBOTIO_H_
#define ROBOTIO_H_

#include <SFML/System.hpp>
#include <map>
#include <vector>
#include <iostream>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define PSOC_DEFAULT_PORT "/dev/ttyUSB0"
#define PI 3.14159265358979323846

#define PSOC_SEND_DELAY 500

#define DRIVE_FRONT_LEFT 'a'
#define DRIVE_FRONT_RIGHT 'b'
#define DRIVE_REAR_LEFT 'c'
#define DRIVE_REAR_RIGHT 'd'
#define MINER_SPIN 'e'
#define MINER_RAISE_LOWER 'f'
#define BIN_SLIDE 'g'
#define CONVEYOR 'h'

using namespace std;

namespace trickfire {

class RobotIO {
public:
	static void SetMotor(unsigned char motorId, double value);

	static void SimpleArcade(double forwards, double rot);

	static void Start();

	static void Stop();

	static void SendPSoCByte(const unsigned char val);

	static void SendPSoCBytes(const unsigned char val[]);

	static inline double GetGyroYaw() {
		return gyroYaw;
	}

	static inline double GetGyroPitch() {
		return gyroPitch;
	}

	static inline double GetGyroRoll() {
		return gyroRoll;
	}

private:
	static map<unsigned char, double> motorValues;
	static sf::Mutex mutex_motorValues;

	static bool _running;

	static sf::Thread commThread;

	static int ardFD;

	static unsigned char buffer[];
	static int bufferSize;
	static int sensorMode;

	static double gyroYaw, gyroPitch, gyroRoll;

	static void ThreadLoop();

	static void ProcessSensorPacket(unsigned char c);

	// PWM = 0 to 254, this converts from 128 being zero, 0 being full reverse, and 254 being full forwards
	static inline double PWMToDouble(unsigned char pwm) {
		return (double) (pwm - 127) / 127;
	}

	static inline unsigned char DoubleToPWM(double doub) {
		return (unsigned char)((doub * 127) + 127);
	}
};
}

#endif
