#include "CameraSendCommand.h"

#if OPENCV

namespace trickfire {
CameraSendCommand::CameraSendCommand(Client * client, sf::Mutex * mut_client) :
		_client(client), mut_client(mut_client), cap(-1), _last_time(0.0) {

}

void CameraSendCommand::OnStart() {

}

void CameraSendCommand::OnFinish() {

}

void CameraSendCommand::Update() {
	cap >> frameRGB;

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
