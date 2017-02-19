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

// Courtesy of https://www.jeremymorgan.com/tutorials/c-programming/how-to-capture-the-output-of-a-linux-command-in-c/
std::string GetStdoutFromCommand(std::string cmd) {
	std::string data;
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	cmd.append(" 2>&1");

	stream = popen(cmd.c_str(), "r");
	if (stream) {
		while (!feof(stream))
			if (fgets(buffer, max_buffer, stream) != NULL)
				data.append(buffer);
		pclose(stream);
	}
	return data;
}

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
	std::string lsOut = GetStdoutFromCommand("ls /dev/ttyUSB*");
	if (lsOut[0] == 'l' && lsOut[1] == 's' && lsOut[2] == ':') {
		Logger::Log(Logger::LEVEL_WARNING, "No ports found at /dev/ttyUSB*, resorting to default.");
		psocFD = open(PSOC_DEFAULT_PORT, O_RDWR);
	} else {
		char path[15];
		for (int i = 0; i < 15; i++) {
			if (lsOut[i] == 10)
				break;

			path[i] = lsOut[i];
		}
		Logger::Log(Logger::LEVEL_INFO_FINE, "Found device at " + std::string(path) + ", attempting to open");
		psocFD = open(path, O_RDWR);
	}
	if (psocFD == -1) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL, "Failed to open arduino port");
		return;
	}

	if (!isatty(psocFD)) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL, "Arduino port not TTY");
		return;
	}

	struct termios tio;

	if (tcgetattr(psocFD, &tio) < 0) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL,
				"Error getting arduino attributes");
		return;
	}

	tio.c_iflag = 0;
	tio.c_oflag = 0;
	tio.c_cflag = CS8 | CREAD | CLOCAL; // 8n1, see termios.h for more information
	tio.c_lflag = 0;
	tio.c_cc[VMIN] = 1;
	tio.c_cc[VTIME] = 5;

	if (cfsetispeed(&tio, B9600) < 0 || cfsetospeed(&tio, B115200) < 0) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL, "Error setting arduino baud");
		return;
	}

	if (tcsetattr(psocFD, TCSAFLUSH, &tio) < 0) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL,
				"Error setting arduino attributes");
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
