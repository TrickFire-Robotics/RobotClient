#ifndef ARUCODRIVECOMMAND_H_
#define ARUCODRIVECOMMAND_H_

#include "ProjectConstants.h"

#if OPENCV and CAMERA

#include "CameraSendCommand.h"

#include "Command.h"
#include "RobotIO.h"
#include "Main.h"
#include <opencv.hpp>
#include <aruco.hpp>

namespace trickfire {

class ArucoDriveCommand: public Command {
public:
	ArucoDriveCommand();

	void OnStart() override;
	void OnFinish() override;
	void Update() override;
	bool IsFinished() override;
	inline std::string GetCommandName() override {
		return "ArUco Drive Command";
	}
	inline void SetCamera(CameraSendCommand * camera) {
		this->camera = camera;
	}

	CameraSendCommand * camera;
	cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(
			cv::aruco::DICT_4X4_50);
	std::vector<int> markerIds;
	std::vector<std::vector<cv::Point2f> > markerCorners;
	float centerX, centerY;
private:
	void UpdateArucoData(cv::Mat& img);
};
}

#endif
#endif
