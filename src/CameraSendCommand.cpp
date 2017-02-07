#include "CameraSendCommand.h"

#if OPENCV

namespace trickfire {
CameraSendCommand::CameraSendCommand(Client * client) :
		_client(client), cap(1) {

}

void CameraSendCommand::OnStart() {

}

void CameraSendCommand::OnFinish() {

}

void CameraSendCommand::Update() {
	cap >> frameRGB;
	double scale = 0.5;
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
	_client->Send(camPacket);
}

bool CameraSendCommand::IsFinished() {
	return false;
}
}

#endif
