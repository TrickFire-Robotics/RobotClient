#ifndef ARUCODRIVECOMMAND_H_
#define ARUCODRIVECOMMAND_H_

#include "ProjectConstants.h"

#include "Command.h"
#include "RobotIO.h"
#include "Main.h"
#include <opencv.hpp>
#include <aruco.hpp>

namespace trickfire {

class ArucoDriveCommand: public Command {
public:
	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;

	sf::Mutex mutex_aruco;
	cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(
			cv::aruco::DICT_4X4_50);
	std::vector<int> markerIds;
	std::vector<std::vector<cv::Point2f> > markerCorners;
	float centerX, centerY;
};
}

#endif
