#pragma once
#include <vector>
#include <list>
#include <map>
#include "InputInterface.h"
#include <opencv2/opencv.hpp>
#include "aruco.h"
#include "Flea3.h"

class ARInterface :
	public InputInterface {
public:
	ARInterface(void);
	~ARInterface(void);
	void processFrame (cv::Mat &frame);
	InputStatus getTag (int tagID) { return m_statusMap[tagID]; }

private:
	virtual InputStatus _getStatus (void);

	double m_markerSize;
	Flea3 *m_camera; // we manage this memory
	aruco::MarkerDetector m_detector;
	std::vector<aruco::Marker> m_markers;
	aruco::CameraParameters m_camParams;
	std::list<InputStatus> m_stati; // FIFO for Tags
	std::map<int, InputStatus> m_statusMap;
};

