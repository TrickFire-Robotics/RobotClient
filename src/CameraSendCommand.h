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
	CameraSendCommand(Client * client, sf::Mutex * mut_client, int c = 0);

	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "Camera Send Command";
	}
	inline cv::Mat GetImage() {
		sf::Lock lock(mut_image);
		return cv::Mat(frameRGB);
	}
	bool transmit = true;
	sf::Mutex mut_Transmit;

private:
	Client * _client;
	sf::Mutex * mut_client;
	sf::Mutex mut_image;
	int cam;
	cv::VideoCapture cap;
	cv::Mat frameRGB;
	double _last_time;
};
}

#endif

#endif
