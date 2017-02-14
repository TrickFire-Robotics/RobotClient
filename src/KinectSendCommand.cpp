#include "KinectSendCommand.h"

#if OPENCV
#if KINECT

namespace trickfire {
KinectSendCommand::KinectSendCommand(Client * client, sf::Mutex * mut_client,
		TFFreenect& kinect) :
		_client(client), mut_client(mut_client), device(kinect), frameRGBOrig(
				cv::Size(640, 480), CV_8UC3, cv::Scalar(0)), frameRGBFixed(
				cv::Size(640, 480), CV_8UC3, cv::Scalar(0)), depthOrig(
				cv::Size(640, 480), CV_16UC1), depthReformat(cv::Size(640, 480),
		CV_8UC1), depthFixed(cv::Size(640, 480), CV_8UC3), _last_time(0) {
}

void KinectSendCommand::OnStart() {
	device.startVideo();
	device.startDepth();
}

void KinectSendCommand::OnFinish() {
}

void KinectSendCommand::Update() {
	device.getVideo(frameRGBOrig);
	device.getDepth(depthOrig);

	if (GetRunningTime() - _last_time > 1.0 / KINECT_SEND_FPS) {
		if (!frameRGBOrig.empty()) {
			double scale = 0.35;
			cv::resize(frameRGBOrig, frameRGBFixed, cv::Size(0, 0), scale,
					scale);
			Packet camPacket;
			camPacket << CAMERA_PACKET;
			camPacket << 1;
			camPacket << frameRGBFixed.rows;
			camPacket << frameRGBFixed.cols;
			uint8_t* pixelPtr = (uint8_t*) frameRGBFixed.data;
			for (int y = 0; y < frameRGBFixed.rows; y++) {
				for (int x = 0; x < frameRGBFixed.cols; x++) {
					camPacket
							<< pixelPtr[y * frameRGBFixed.cols * 3 + x * 3 + 0];
					camPacket
							<< pixelPtr[y * frameRGBFixed.cols * 3 + x * 3 + 1];
					camPacket
							<< pixelPtr[y * frameRGBFixed.cols * 3 + x * 3 + 2];
				}
			}
			mut_client->lock();
			_client->Send(camPacket);
			mut_client->unlock();
		}
		if (!depthOrig.empty()) {
			depthOrig.convertTo(depthReformat, CV_8UC1, 255.0 / 2048.0);
			cv::cvtColor(depthReformat, depthRGB, cv::COLOR_GRAY2BGR);
			double scale = 0.35;
			cv::resize(depthRGB, depthFixed, cv::Size(0, 0), scale,
					scale);
			Packet kinectPacket;
			kinectPacket << CAMERA_PACKET;
			kinectPacket << 2;
			kinectPacket << depthFixed.rows;
			kinectPacket << depthFixed.cols;
			uint8_t* pixelPtr = (uint8_t*) depthFixed.data;
			for (int y = 0; y < depthFixed.rows; y++) {
				for (int x = 0; x < depthFixed.cols; x++) {
					kinectPacket
							<< pixelPtr[y * depthFixed.cols * 3 + x * 3 + 0];
					kinectPacket
							<< pixelPtr[y * depthFixed.cols * 3 + x * 3 + 1];
					kinectPacket
							<< pixelPtr[y * depthFixed.cols * 3 + x * 3 + 2];
				}
			}
			mut_client->lock();
			_client->Send(kinectPacket);
			mut_client->unlock();
		}
		_last_time = GetRunningTime();
	}
	/*cap >> frameRGB;
	 double scale = 0.15;
	 cv::resize(frameRGB, frameRGB, cv::Size(0, 0), scale, scale);
	 Packet camPacket;
	 camPacket << CAMERA_PACKET;
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
	 _client->Send(camPacket);*/
}

bool KinectSendCommand::IsFinished() {
	return false;
}
}

#endif
#endif
