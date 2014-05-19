#pragma once

#include <opencv2/opencv.hpp>

#define WIN32 1
#include "FlyCapture2.h"

class ARInterface; // forward declaration

class Flea3 {
public:
	Flea3(void);
	~Flea3(void);
	void setARInterface (ARInterface *ari);
	void startCapture (void);
	bool connected() { return m_connected; }

private:
	// Static callback to use with C-style function pointer
	static void image_callback (FlyCapture2::Image *rawImage, const void *data);

	// The real callback that does the work
	void _image_callback  (FlyCapture2::Image *rawImage, const void *data);

	cv::Mat m_frame; // latest grabbed frame
	FlyCapture2::Image m_rgbImage; // temporary used for decoding
	ARInterface *arInterface; // we don't manage this memory
	FlyCapture2::Camera m_camera;
	FlyCapture2::CameraInfo m_camInfo;
	bool m_connected;
};

