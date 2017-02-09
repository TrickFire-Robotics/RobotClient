#include "KinectSendCommand.h"

#if OPENCV

namespace trickfire {
KinectSendCommand::KinectSendCommand(Client * client, TFFreenect& kinect) :
		_client(client), device(kinect), frameRGBOrig(cv::Size(640, 480), CV_8UC3,
				cv::Scalar(0)), frameRGBFixed(
				cv::Size(640, 480), CV_8UC3,
				cv::Scalar(0)) {
}

void KinectSendCommand::OnStart() {
	device.startVideo();
	device.startDepth();
}

void KinectSendCommand::OnFinish() {
	//device.stopVideo();
}

void KinectSendCommand::Update() {
	device.getVideo(frameRGBOrig);

	if (!frameRGBOrig.empty()) {
		double scale = 0.15;
		cv::resize(frameRGBOrig, frameRGBFixed, cv::Size(0, 0), scale, scale);
		Packet camPacket;
		camPacket << CAMERA_PACKET;
		camPacket << frameRGBFixed.rows;
		camPacket << frameRGBFixed.cols;
		uint8_t* pixelPtr = (uint8_t*) frameRGBFixed.data;
		for (int y = 0; y < frameRGBFixed.rows; y++) {
			for (int x = 0; x < frameRGBFixed.cols; x++) {
				camPacket << pixelPtr[y * frameRGBFixed.cols * 3 + x * 3 + 0];
				camPacket << pixelPtr[y * frameRGBFixed.cols * 3 + x * 3 + 1];
				camPacket << pixelPtr[y * frameRGBFixed.cols * 3 + x * 3 + 2];
			}
		}
		_client->Send(camPacket);
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
