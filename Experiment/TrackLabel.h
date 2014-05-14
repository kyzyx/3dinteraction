#pragma once

class TrackLabel {
public:
	TrackLabel();
	~TrackLabel();

	bool init();
	bool update();

private:
	int patt_id;
	double lastKnownPosition[3][4];
};