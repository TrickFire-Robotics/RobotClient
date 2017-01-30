#ifndef CAMERASENDCOMMAND_H_
#define CAMERASENDCOMMAND_H_

#include "Command.h"
#include "Client.h"
#include "NetworkingConstants.h"
#include <opencv.hpp>

namespace trickfire {

class CameraSendCommand: public Command {
public:
	CameraSendCommand(Client * client);

	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;

private:
	Client * _client;
	cv::VideoCapture cap;
	cv::Mat frameRGB;
};
}

#endif
