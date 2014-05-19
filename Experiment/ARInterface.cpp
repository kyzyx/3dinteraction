#include "ARInterface.h"
#include "Flea3.h"
#include "timestamp.h"

ARInterface::ARInterface(void) : m_markerSize(0.0508), m_camera(nullptr)
{
	// Camera Matrix for FLEA3 at 1280x640
	double _camMat[3][3] = {
		{1.1100159162857863e+003, 0.0                    , 6.3950000000000000e+002},
		{0.0                    , 1.1100159162857863e+003, 3.1950000000000000e+002},
		{0.0                    , 0.0                    , 1.0                    }
	};
	cv::Mat camMat(3,3, CV_64F, _camMat);

	double _distCoef[4][1] = {
		{-3.8084348928819045e-001}, 
		{ 2.5257592869700013e-001}, 
		{ 0.0                    },
		{ 0.0                    }
	};
	cv::Mat distMat(4,1, CV_64F, _distCoef);
	m_camParams = aruco::CameraParameters(camMat, distMat, cv::Size(1280,640));
	m_detector.setCornerRefinementMethod(aruco::MarkerDetector::LINES);
	m_detector.setThresholdMethod(aruco::MarkerDetector::FIXED_THRES);
	m_detector.setThresholdParams(164,0);

	m_camera = new Flea3();
	m_camera->setARInterface(this);
	m_camera->startCapture();
}


ARInterface::~ARInterface(void)
{
	delete m_camera;
}


void ARInterface::processFrame (cv::Mat &frame) {
	m_detector.detect(frame, m_markers, m_camParams, m_markerSize);
	cv::Mat bw = m_detector.getThresholdedImage();
	// Delete any old inputs ... if no one grabbed them, they are stale
	//m_stati.clear();
	for (size_t idx = 0; idx != m_markers.size(); ++idx) {
		aruco::Marker &m = m_markers[idx];
		double pos[3];
		double quat[4];
		m.OgreGetPoseParameters(pos, quat);
		InputStatus s;
		s.pos = Eigen::Map<const Eigen::Vector3d>(pos);
		s.rot = Eigen::Map<const Eigen::Quaterniond>(quat);
		s.flags = m.id;
		s.inputType = InputStatus::ARTAG;
		s.timestamp = timestamp();
		m_stati.push_back(s);
		m_statusMap[m.id] = s;
		m.draw(bw, cv::Scalar(0,0,0));
	}
	cv::imshow("t", bw);
	cv::waitKey(1);
}

InputStatus ARInterface::_getStatus (void) {
	// Pump the list of queued inputs
	if (m_stati.size() > 0) {
		InputStatus s = m_stati.front();
		m_stati.pop_front();
		return s;
	} else {
		InputStatus s;
		s.inputType = InputStatus::NONE;
		return s;
	}
}