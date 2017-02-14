#ifndef TFFREENECT_H_
#define TFFREENECT_H_

#include "ProjectConstants.h"

#if OPENCV
#if KINECT

#include <libfreenect.hpp>
#include <opencv.hpp>
#include <SFML/System.hpp>

namespace trickfire {

class TFFreenect: public Freenect::FreenectDevice {
public:
	TFFreenect(freenect_context *_ctx, int _index);

	void VideoCallback(void* _rgb, uint32_t timestamp);
	void DepthCallback(void* _depth, uint32_t timestamp);
	bool getVideo(cv::Mat& output);
	bool getDepth(cv::Mat& output);

private:
	std::vector<uint8_t> m_buffer_depth;
	std::vector<uint8_t> m_buffer_rgb;
	std::vector<uint16_t> m_gamma;
	cv::Mat depthMat;
	cv::Mat rgbMat;
	cv::Mat ownMat;
	sf::Mutex m_rgb_mutex;
	sf::Mutex m_depth_mutex;
	bool m_new_rgb_frame;
	bool m_new_depth_frame;
};
}

#endif
#endif
#endif
