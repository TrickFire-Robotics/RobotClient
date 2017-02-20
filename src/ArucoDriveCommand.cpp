#include "ArucoDriveCommand.h"

namespace trickfire {

#define SIDE 120

ArucoDriveCommand::ArucoDriveCommand() : camera(nullptr), centerX(-1), centerY(-1) {

}

void ArucoDriveCommand::OnStart() {

}

void ArucoDriveCommand::Update() {
	if (camera == nullptr) return;

	cv::Mat img = camera->GetImage();
	UpdateArucoData(img);

	if (centerX >= 0) {
		if (centerX - 320 > SIDE) {
			RobotIO::SimpleArcade(0.0, 0.25);
		} else if (centerX - 320 < -SIDE) {
			RobotIO::SimpleArcade(0.0, -0.25);
		} else {
			RobotIO::SimpleArcade(0.0, 0.0);
		}
	} else {
		RobotIO::SimpleArcade(0.0, 0.0);
	}
}

void ArucoDriveCommand::UpdateArucoData(cv::Mat& img) {
	if (img.empty()) return;

		centerX = -1;
		centerY = -1;
		cv::aruco::detectMarkers(img, dict, markerCorners, markerIds);
		//cv::aruco::drawDetectedMarkers(frameRGB, markerCorners, markerIds);

		if (!markerIds.empty()) {
			for (uint j = 0; j < markerCorners.size(); j++) {
				std::vector<cv::Point2f> * corners = &markerCorners[j];

				float sumX = 0.0f, sumY = 0.0f;
				for (unsigned int i = 0; i < corners->size(); i++) {
					sumX += corners[0][i].x;
					sumY += corners[0][i].y;
				}

				sumX /= corners->size();
				sumY /= corners->size();
				centerX = sumX;
				centerY = sumY;

				//cv::circle(frameRGB, cv::Point2f(sumX, sumY), 16,
				//		cv::Scalar(0, 0, 255), -1, 8);
			}
		}
}

bool ArucoDriveCommand::IsFinished() {
	return false;
}

void ArucoDriveCommand::OnFinish() {
	RobotIO::SimpleArcade(0.0, 0.0);
	Main::ResumeStandardDrive();
}
}
