#include "RobotIO.h"
#include "Logger.h"
#include <unistd.h>
#include <chrono>
#include <thread>

namespace trickfire {
	pthread_t RobotIO::commThread;
	map<unsigned char, double> RobotIO::motorValues;
	int RobotIO::psocFD;

	void RobotIO::SetMotor(unsigned char motorId, double value) {
		motorValues[motorId] = value;
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
			Logger::Log(Logger::LEVEL_ERROR_CRITICAL, "Error getting PSoC attributes");
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
			Logger::Log(Logger::LEVEL_ERROR_CRITICAL, "Error setting PSoC attributes");
		}

		pthread_create(&commThread, NULL, &ThreadLoop, (void *) NULL);
	}

	void RobotIO::Stop() {
		close(psocFD);
	}

	void * RobotIO::ThreadLoop(void * pointless)  {
		while (true) {
			// TODO: Make periodic (add a delay)
			// TODO: Reserved char values for something special (disabled or something?)
			for(map<unsigned char, double>::iterator iterator = motorValues.begin(); iterator != motorValues.end(); iterator++) {
				SendPSoCByte(iterator->first);
				usleep(50);
				unsigned char pwm = DoubleToPWM(iterator->second);
				if (pwm == 0) pwm = 1;
				SendPSoCByte(pwm);
				usleep(50);
				SendPSoCByte(255);
				usleep(50);
				cout << "Sending " << (int)DoubleToPWM(iterator->second) << " on " << iterator->first << endl;
			}
		}
	}

	void RobotIO::SendPSoCByte(const unsigned char byte)  {
		write(psocFD, &byte, 1);
		fsync(psocFD);
	}

	void RobotIO::SendPSoCBytes(const unsigned char byte[])  {
			write(psocFD, &byte, 1);
			fsync(psocFD);
		}
}
