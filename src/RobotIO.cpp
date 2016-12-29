#include "RobotIO.h"
#include "Logger.h"
#include <unistd.h>
#include <chrono>
#include <thread>

#define PSOC_SEND_DELAY 50

#define FRONT_LEFT 'a'
#define FRONT_RIGHT 'b'
#define REAR_LEFT 'c'
#define REAR_RIGHT 'd'

namespace trickfire {
sf::Thread RobotIO::commThread(&RobotIO::ThreadLoop);
map<unsigned char, double> RobotIO::motorValues;
int RobotIO::psocFD;

void RobotIO::SetMotor(unsigned char motorId, double value) {
	motorValues[motorId] = value;
}

void RobotIO::SimpleArcade(double forwards, double rot) {
	double left = min(max(forwards + rot, -1.0), 1.0);
	double right = min(max(forwards - rot, -1.0), 1.0);

	SetMotor(FRONT_LEFT, left);
	SetMotor(REAR_LEFT, left);
	SetMotor(FRONT_RIGHT, right);
	SetMotor(REAR_RIGHT, right);
}

void RobotIO::Start() {
	psocFD = open(PSOC_PORT, O_RDWR);
	if (psocFD == -1) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL, "Failed to open PSoC port");
	}

	if (!isatty(psocFD)) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL, "PSoC port not TTY");
	}

	struct termios tio;

	if (tcgetattr(psocFD, &tio) < 0) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL,
				"Error getting PSoC attributes");
	}

	tio.c_iflag = 0;
	tio.c_oflag = 0;
	tio.c_cflag = CS8 | CREAD | CLOCAL; // 8n1, see termios.h for more information
	tio.c_lflag = 0;
	tio.c_cc[VMIN] = 1;
	tio.c_cc[VTIME] = 5;

	if (cfsetispeed(&tio, B9600) < 0 || cfsetospeed(&tio, B9600) < 0) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL, "Error setting PSoC baud");
	}

	if (tcsetattr(psocFD, TCSAFLUSH, &tio) < 0) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL,
				"Error setting PSoC attributes");
	}

	commThread.launch();
}

void RobotIO::Stop() {
	close(psocFD);
}

void RobotIO::ThreadLoop() {
	while (true) {
		// TODO: Make periodic (add a delay)
		// TODO: Reserved char values for something special (disabled or something?)
		for (map<unsigned char, double>::iterator iterator =
				motorValues.begin(); iterator != motorValues.end();
				iterator++) {
			SendPSoCByte(iterator->first);
			usleep(PSOC_SEND_DELAY);
			unsigned char pwm = DoubleToPWM(iterator->second);
			if (pwm == 0)
				pwm = 1;
			SendPSoCByte(pwm);
			usleep(PSOC_SEND_DELAY);
			SendPSoCByte(255);
			usleep(PSOC_SEND_DELAY);
		}
	}
}

void RobotIO::SendPSoCByte(const unsigned char byte) {
	write(psocFD, &byte, 1);
	fsync(psocFD);
}

void RobotIO::SendPSoCBytes(const unsigned char byte[]) {
	write(psocFD, &byte, 1);
	fsync(psocFD);
}
}
