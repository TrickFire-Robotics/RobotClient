#include "CameraSendCommand.h"

#if OPENCV

namespace trickfire {
CameraSendCommand::CameraSendCommand(Client * client, sf::Mutex * mut_client) :
		_client(client), mut_client(mut_client), cap(-1), _last_time(0.0), arucoDrive(nullptr) {

}

CameraSendCommand::CameraSendCommand(Client * client, sf::Mutex * mut_client, ArucoDriveCommand * arucoDrive) :
		_client(client), mut_client(mut_client), cap(-1), _last_time(0.0), arucoDrive(arucoDrive) {

}

void CameraSendCommand::OnStart() {

}

void CameraSendCommand::OnFinish() {

}

void CameraSendCommand::Update() {
	cap >> frameRGB;

	if (arucoDrive != nullptr) {
		arucoDrive->mutex_aruco.lock();
		arucoDrive->centerX = -1;
		arucoDrive->centerY = -1;
		cv::aruco::detectMarkers(frameRGB, arucoDrive->dict, arucoDrive->markerCorners, arucoDrive->markerIds);
		cv::aruco::drawDetectedMarkers(frameRGB, arucoDrive->markerCorners, arucoDrive->markerIds);

		if (!arucoDrive->markerIds.empty()) {
			for (uint j = 0; j < arucoDrive->markerCorners.size(); j++) {
				std::vector<cv::Point2f> * corners = &arucoDrive->markerCorners[j];

				float sumX = 0.0f, sumY = 0.0f;
				for (unsigned int i = 0; i < corners->size(); i++) {
					sumX += corners[0][i].x;
					sumY += corners[0][i].y;
				}

				sumX /= corners->size();
				sumY /= corners->size();
				arucoDrive->centerX = sumX;
				arucoDrive->centerY = sumY;

				cv::circle(frameRGB, cv::Point2f(sumX, sumY), 16,
						cv::Scalar(0, 0, 255), -1, 8);
			}
		}
		arucoDrive->mutex_aruco.unlock();
	}

	if (GetRunningTime() - _last_time > 1.0 / CAMERA_SEND_FPS) {
		double scale = 0.35;
		cv::resize(frameRGB, frameRGB, cv::Size(0, 0), scale, scale);
		Packet camPacket;
		camPacket << CAMERA_PACKET;
		camPacket << 0;
		camPacket << frameRGB.rows;
		camPacket << frameRGB.cols;
		uint8_t* pixelPtr = (uint8_t*) frameRGB.data;
		for (int y = 0; y < frameRGB.rows; y++) {
			for (int x = 0; x < frameRGB.cols; x++) {
				camPacket << pixelPtr[y * frameRGB.cols * 3 + x * 3 + 0];
				camPacket << pixelPtr[y * frameRGB.cols * 3 + x * 3 + 1];
				camPacket << pixelPtr[y * frameRGB.cols * 3 + x * 3 + 2];
			}
		}
		mut_client->lock();
		_client->Send(camPacket);
		mut_client->unlock();
		_last_time = GetRunningTime();
	}
}

bool CameraSendCommand::IsFinished() {
	return false;
}
}

#endif
