#ifndef KINECTSENDCOMMAND_H_
#define KINECTSENDCOMMAND_H_

#include "ProjectConstants.h"

#include "TFFreenect.h"

#include <libfreenect.hpp>

#if OPENCV

#include "Command.h"
#include "Client.h"
#include "NetworkingConstants.h"
#include <opencv.hpp>

namespace trickfire {

class KinectSendCommand: public Command {
public:
	KinectSendCommand(Client * client, TFFreenect& kinect);

	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;

private:
	Client * _client;
	TFFreenect& device;
	cv::Mat frameRGBOrig;
	cv::Mat frameRGBFixed;
};
}

#endif

#endif
