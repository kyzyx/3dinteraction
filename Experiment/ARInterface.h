#pragma once
#include <vector>
#include <list>
#include <map>
#include "InputInterface.h"
#include <opencv2/opencv.hpp>
#include "aruco/aruco.h"
#include "Flea3.h"

class ARInterface {
public:
	static ARInterface& getInstance(void);
	~ARInterface(void);
	void processFrame (cv::Mat &frame);
	InputStatus getTag (int tagID) { return m_statusMap[tagID]; }
	void setActiveTag (int tagID);
	InputStatus getStatus (void);

private:
	ARInterface(void);

	double m_markerSize;
	int m_tagID;
	Flea3 *m_camera; // we manage this memory
	aruco::MarkerDetector m_detector;
	std::vector<aruco::Marker> m_markers;
	aruco::CameraParameters m_camParams;
	std::list<InputStatus> m_stati; // FIFO for Tags
	std::map<int, InputStatus> m_statusMap;
};

// Wrapper class around ARInterface to make it compatible with InputInterface
class ARInputInterface : public InputInterface {
public:
	ARInputInterface (void) : m_interface(ARInterface::getInstance()) {
		m_interface.setActiveTag(10);
	}

protected:
	virtual InputStatus _getStatus (void) { 
		InputStatus s = m_interface.getStatus();
		s.z() -= 0.3; // Move 30cm away from camera to zero plane
		return s;
	}

private:
	ARInterface &m_interface;
};