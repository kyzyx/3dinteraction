#include "ARInterface.h"
#include "Flea3.h"
#include "timestamp.h"
#include <opencv2/core/eigen.hpp>

ARInterface& ARInterface::getInstance (void) {
	static ARInterface instance;
	return instance;
}

ARInterface::ARInterface(void) : m_markerSize(5.08), m_tagID(0), m_camera(nullptr)
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
	m_camParams.setParams(camMat, distMat, cv::Size(1280,640));
	m_detector.setCornerRefinementMethod(aruco::MarkerDetector::LINES);
	m_detector.setThresholdMethod(aruco::MarkerDetector::FIXED_THRES);
	m_detector.setThresholdParams(70,0);

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
	for (size_t idx = 0; idx < m_markers.size(); ++idx) {
		aruco::Marker &m = m_markers[idx];
		InputStatus s;
		cv2eigen(m.Tvec, s.pos);
		double x = m.Rvec.at<float>(0);
		double y = m.Rvec.at<float>(1);
		double z = m.Rvec.at<float>(2);
		double angle = sqrt(x*x + y*y + z*z);
		Eigen::Vector3d axis(x/angle, y/angle, z/angle);
		s.rot = Eigen::Quaterniond(Eigen::AngleAxisd(angle, axis));
		s.flags = m.id;
		s.inputType = InputStatus::ARTAG;
		s.timestamp = timestamp();
		m_stati.push_back(s);
		m_statusMap[m.id] = s;
		m.draw(bw, cv::Scalar(0,0,0));
	}

	// Uncomment here for debugging view of thresholding
	//cv::imshow("t", bw);
	//cv::waitKey(1);
}

void ARInterface::setActiveTag (int tagID) {
	m_tagID = tagID;
	if (m_tagID < 1) m_tagID = 1;
	if (m_tagID > 1023) m_tagID = 1023;
	if (m_statusMap.count(tagID) == 0) {
		InputStatus s;
		s.inputType = InputStatus::NONE;
		m_statusMap[tagID] = s;
	}
}

// Returns inputStatus for the currently active tag ID
InputStatus ARInterface::getStatus (void) {
	if (m_tagID == 0) {
		InputStatus s;
		s.inputType = InputStatus::NONE;
		return s;
	}
		
	return m_statusMap[m_tagID];
}