#define WIN32 1

#include <Windows.h>
#include <functional>

#include "FlyCapture2.h"

#include "Flea3.h"
#include "ARInterface.h"

namespace fc2 = FlyCapture2;

Flea3::Flea3(void) : arInterface(nullptr), m_connected(false)
{
	// Connect to the first camera we find
	fc2::Error error;

    error = m_camera.Connect( 0 );
    if ( error != fc2::PGRERROR_OK )
    {
		OutputDebugString("Failed to connect to camera");
		return;
    }
	m_connected = true;

    error = m_camera.GetCameraInfo( &m_camInfo );
    if ( error != fc2::PGRERROR_OK )
    {
		OutputDebugString("Failed to get camera info from camera");
    }
}

Flea3::~Flea3(void)
{
    fc2::Error error = m_camera.StopCapture();
    if ( error != fc2::PGRERROR_OK )
    {
		OutputDebugString("Failed to stop capture");
    }  
	m_camera.Disconnect();
}

void Flea3::startCapture (void) {
	if (!connected()) return;

	// Register our static method as a callback with pointer to this object as it's parameter
	fc2::Error error = m_camera.StartCapture(Flea3::image_callback, this);
	if (error != fc2::PGRERROR_OK) {
		OutputDebugString("Failed to start capture");
	}
}

void Flea3::setARInterface (ARInterface *ari) {
	arInterface = ari;
}

void Flea3::image_callback(fc2::Image *rawImage, const void *data) {
	Flea3 *that = reinterpret_cast<Flea3*>(const_cast<void*>(data)); // yes, strip the evil const away
	that->_image_callback(rawImage, NULL);
}

void Flea3::_image_callback(fc2::Image *rawImage, const void *data) {
	// convert to rgb
	rawImage->SetDefaultColorProcessing(fc2::IPP);
	rawImage->Convert(FlyCapture2::PIXEL_FORMAT_BGR, &m_rgbImage);
	
	// convert to OpenCV Mat
	unsigned int rowBytes = (double)m_rgbImage.GetReceivedDataSize() / (double)m_rgbImage.GetRows();
	m_frame = cv::Mat(m_rgbImage.GetRows(), m_rgbImage.GetCols(), CV_8UC3, m_rgbImage.GetData(), rowBytes);

	if (arInterface != nullptr) {
		arInterface->processFrame(m_frame);
	}
}