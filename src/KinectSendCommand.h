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
	KinectSendCommand(Client * client, sf::Mutex * mut_client, TFFreenect& kinect);

	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;

private:
	Client * _client;
	sf::Mutex * mut_client;
	TFFreenect& device;
	cv::Mat frameRGBOrig;
	cv::Mat frameRGBFixed;
	cv::Mat depthOrig;
	cv::Mat depthReformat;
	cv::Mat depthRGB;
	cv::Mat depthFixed;
	double _last_time;
};
}

#endif

#endif
