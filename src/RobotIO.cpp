#include "RobotIO.h"
#include "Logger.h"
#include "DisplayVariables.h"
#include <unistd.h>
#include <chrono>
#include <thread>

namespace trickfire {
sf::Thread RobotIO::commThread(&RobotIO::ThreadLoop);
map<unsigned char, double> RobotIO::motorValues;
map<unsigned char, bool> RobotIO::limitSwitches;
sf::Mutex RobotIO::mutex_motorValues, RobotIO::mutex_limitSwitches;
bool RobotIO::_running = false;
int RobotIO::ardFD;
unsigned char RobotIO::buffer[50];
int RobotIO::bufferSize = 0;
int RobotIO::sensorMode = -1;
double RobotIO::gyroYaw, RobotIO::gyroPitch, RobotIO::gyroRoll;

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
	Logger::Log(Logger::LEVEL_INFO_VERY_FINE,
			"Setting motor " + to_string(motorId) + " to output "
					+ to_string(value));
	sf::Lock motorValuesLock(mutex_motorValues);
	motorValues[motorId] = value;

	/*if (motorId == MINER_MOVE_LOWER_LEFT) {
		cout << "Lower left: " << value << endl;
	}

	if (motorId == MINER_MOVE_UPPER_LEFT) {
		cout << "Upper left: " << value << endl;
	}*/
}

void RobotIO::SimpleArcade(double forwards, double rot) {
	double left = min(max(forwards + rot, -1.0), 1.0);
	double right = min(max(forwards - rot, -1.0), 1.0);

	SetMotor(DRIVE_FRONT_LEFT, -left);
	SetMotor(DRIVE_REAR_LEFT, -left);
	SetMotor(DRIVE_FRONT_RIGHT, right);
	SetMotor(DRIVE_REAR_RIGHT, right);

	DisplayVariables::SetLeft(forwards);
	DisplayVariables::SetRight(rot);
}

void RobotIO::SimpleTank(double l, double r) {
	SetMotor(DRIVE_FRONT_LEFT, l);
	SetMotor(DRIVE_REAR_LEFT, l);
	SetMotor(DRIVE_FRONT_RIGHT, -r);
	SetMotor(DRIVE_REAR_RIGHT, -r);

	DisplayVariables::SetLeft(l);
	DisplayVariables::SetRight(r);
}

void RobotIO::Start() {
	std::string lsOut = GetStdoutFromCommand("ls /dev/ttyACM*");

	if (true || lsOut[0] == 'l' && lsOut[1] == 's' && lsOut[2] == ':') {
		Logger::Log(Logger::LEVEL_WARNING,
				"No ports found at /dev/ttyACM*, resorting to default.");
		ardFD = open(PSOC_DEFAULT_PORT, O_RDWR);
	} else {
		char path[15];
		for (int i = 0; i < 15; i++) {
			if (lsOut[i] == 10 || lsOut[i] == 0)
				break;

			path[i] = lsOut[i];
		}
		Logger::Log(Logger::LEVEL_INFO_FINE,
				"Found device at " + std::string(path)
						+ ", attempting to open");
		ardFD = open(path, O_RDWR);
	}
	if (ardFD == -1) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL,
				"Failed to open arduino port");
		return;
	}

	if (!isatty(ardFD)) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL, "Arduino port not TTY");
		return;
	}

	struct termios tio;

	if (tcgetattr(ardFD, &tio) < 0) {
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

	if (tcsetattr(ardFD, TCSAFLUSH, &tio) < 0) {
		Logger::Log(Logger::LEVEL_ERROR_CRITICAL,
				"Error setting arduino attributes");
		return;
	}

	commThread.launch();
	_running = true;
}

void RobotIO::Stop() {
	close(ardFD);
	_running = false;
}

void RobotIO::ThreadLoop() {
	while (_running) {
		/*		unsigned char val;
		 if (read(ardFD, &val, 1) > 0) {
		 ProcessSensorPacket(val);
		 }*/

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

void RobotIO::ProcessSensorPacket(unsigned char c) {
	if (sensorMode == -1) {
		sensorMode = c;
	} else {
		if (c == 255) {
			if (sensorMode == 1) {
				char yC[10], pC[10], rC[10];

				int i = 0;
				{
					int yI = 0;
					while (buffer[i] != ',') {
						yC[yI] = buffer[i];
						i++;
						yI++;
					}
					i++;
				}
				{
					int pI = 0;
					while (buffer[i] != ',') {
						pC[pI] = buffer[i];
						i++;
						pI++;
					}
					i++;
				}
				{
					int rI = 0;
					while (buffer[i] != 0) {
						rC[rI] = buffer[i];
						i++;
						rI++;
					}
				}
				gyroYaw = std::stod(std::string(yC));
				gyroPitch = std::stod(std::string(pC));
				gyroRoll = std::stod(std::string(rC));
			} else if (sensorMode == 2) {
				char switchId = buffer[0];
				char switchVal = buffer[1];

				{
					sf::Lock lock(mutex_limitSwitches);

					limitSwitches[switchId] = (switchVal == 1);
				}

				cout << "Switch: " << switchId << ", Val: " << switchVal
						<< endl;
			}

			sensorMode = -1;
			bufferSize = 0;
		} else {
			buffer[bufferSize] = c;
			bufferSize++;
		}
	}
}

void RobotIO::SendPSoCByte(const unsigned char byte) {
	write(ardFD, &byte, 1);
	fsync(ardFD);
}

void RobotIO::SendPSoCBytes(const unsigned char byte[]) {
	write(ardFD, &byte, 1);
	fsync(ardFD);
}
}
