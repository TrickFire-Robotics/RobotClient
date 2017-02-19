#include "RobotIO.h"
#include "Logger.h"
#include "DisplayVariables.h"
#include <unistd.h>
#include <chrono>
#include <thread>

#define PSOC_SEND_DELAY 100

#define FRONT_LEFT 'a'
#define FRONT_RIGHT 'b'
#define REAR_LEFT 'c'
#define REAR_RIGHT 'd'

namespace trickfire {
sf::Thread RobotIO::commThread(&RobotIO::ThreadLoop);
map<unsigned char, double> RobotIO::motorValues;
sf::Mutex RobotIO::mutex_motorValues;
bool RobotIO::_running = false;
int RobotIO::psocFD;

void RobotIO::SetMotor(unsigned char motorId, double value) {
	Logger::Log(Logger::LEVEL_INFO_VERY_FINE, "Setting motor " + to_string(motorId) + " to output " + to_string(value));
	sf::Lock motorValuesLock (mutex_motorValues);
	motorValues[motorId] = value;
}

void RobotIO::SimpleArcade(double forwards, double rot) {
	double left = min(max(forwards + rot, -1.0), 1.0);
	double right = min(max(forwards - rot, -1.0), 1.0);

	SetMotor(FRONT_LEFT, -left);
	SetMotor(REAR_LEFT, -left);
	SetMotor(FRONT_RIGHT, right);
	SetMotor(REAR_RIGHT, right);

	DisplayVariables::SetDrive(forwards);
	DisplayVariables::SetRot(rot);
}

void RobotIO::Start() {
	psocFD = open(PSOC_PORT, O_RDWR);
	if (psocFD == -1) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL, "Failed to open PSoC port");
		return;
	}

	if (!isatty(psocFD)) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL, "PSoC port not TTY");
		return;
	}

	struct termios tio;

	if (tcgetattr(psocFD, &tio) < 0) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL,
				"Error getting PSoC attributes");
		return;
	}

	tio.c_iflag = 0;
	tio.c_oflag = 0;
	tio.c_cflag = CS8 | CREAD | CLOCAL; // 8n1, see termios.h for more information
	tio.c_lflag = 0;
	tio.c_cc[VMIN] = 1;
	tio.c_cc[VTIME] = 5;

	if (cfsetispeed(&tio, B9600) < 0 || cfsetospeed(&tio, B115200) < 0) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL, "Error setting PSoC baud");
		return;
	}

	if (tcsetattr(psocFD, TCSAFLUSH, &tio) < 0) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL,
				"Error setting PSoC attributes");
		return;
	}

	commThread.launch();
	_running = true;
}

void RobotIO::Stop() {
	close(psocFD);
	_running = false;
}

void RobotIO::ThreadLoop() {
	while (_running) {
		// TODO: Possibly change system to something more than one unsigned char
		for (map<unsigned char, double>::iterator iterator =
				motorValues.begin(); iterator != motorValues.end();
				iterator++) {

			mutex_motorValues.lock();
			unsigned char pwm = DoubleToPWM(iterator->second);
			unsigned char key = iterator->first;
			mutex_motorValues.unlock();

			SendPSoCByte(key);
			usleep(PSOC_SEND_DELAY);
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
