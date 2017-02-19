#ifndef CAMERASENDCOMMAND_H_
#define CAMERASENDCOMMAND_H_

class ArucoDriveCommand;

#include "ProjectConstants.h"

#if OPENCV

#include "Command.h"
#include "Client.h"
#include "NetworkingConstants.h"
#include "ArucoDriveCommand.h"
#include <opencv.hpp>
#include <aruco.hpp>

namespace trickfire {
class CameraSendCommand: public Command {
public:
	CameraSendCommand(Client * client, sf::Mutex * mut_client);
	CameraSendCommand(Client * client, sf::Mutex * mut_client, ArucoDriveCommand * arucoDrive);

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
	ArucoDriveCommand * arucoDrive;
	/*cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(
			cv::aruco::DICT_4X4_50);
	std::vector<int> markerIds;
	std::vector<std::vector<cv::Point2f> > markerCorners;*/
};
}

#endif

#endif
