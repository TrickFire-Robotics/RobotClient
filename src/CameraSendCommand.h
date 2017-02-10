#ifndef CAMERASENDCOMMAND_H_
#define CAMERASENDCOMMAND_H_

#include "ProjectConstants.h"

#if OPENCV

#include "Command.h"
#include "Client.h"
#include "NetworkingConstants.h"
#include <opencv.hpp>

namespace trickfire {

class CameraSendCommand: public Command {
public:
	CameraSendCommand(Client * client, sf::Mutex * mut_client);

	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;

private:
	Client * _client;
	sf::Mutex * mut_client;
	cv::VideoCapture cap;
	cv::Mat frameRGB;
	double _last_time;
};
}

#endif

#endif
