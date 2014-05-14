#include "Tracklabel.h"
#include <AR/param.h>
#include <AR/video.h>
#include <AR/ar.h>
#include <Windows.h>
#include <stdio.h>

int THRESH = 100;
double PATT_WIDTH = 80.0;
double PATT_CENTER[2] = {0.0, 0.0};

TrackLabel::TrackLabel()
{
	
}

TrackLabel::~TrackLabel()
{
	arVideoCapStop();
	arVideoClose();
}

bool TrackLabel::init()
{
	ARParam wparam;

	/* open the video path */
    if( arVideoOpen( "Data\\WDM_camera_flipV.xml" ) < 0 )
	{
		OutputDebugString("Video path failed\n");
		return false;
	}

	/* find the size of the window */
	int xsize, ysize;
    if( arVideoInqSize(&xsize, &ysize) < 0 )
	{
		OutputDebugString("Find size failed\n");
		return false;
	}

	/* set the initial camera parameters */
    if( arParamLoad("Data/camera_para.dat", 1, &wparam) < 0 )
	{
		OutputDebugString("Init camera failed\n");
        return false;
    }

	ARParam cparam;
	arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );
    arParamDisp( &cparam );

	if( (patt_id=arLoadPatt("Data/patt.hiro")) < 0 )
	{
		OutputDebugString("Load pattern failed\n");
        return false;
    }

	if(arVideoCapStart() != 0)
	{
		OutputDebugString("AR start failed\n");
		return false;
	}

	return true;
}

bool TrackLabel::update()
{
	ARUint8 *dataPtr;
	ARMarkerInfo *marker_info;
	int marker_num;
	int j, k;

	/* grab a vide frame */
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL )
	{
		OutputDebugString("No Video\n");
        return true;
    }

	/* detect the markers in the video frame */
    if( arDetectMarker(dataPtr, THRESH, &marker_info, &marker_num) < 0 )
	{
		// Donny: something is wrong
        return false;
    }

	arVideoCapNext();

	/* check for object visibility */
    k = -1;
    for( j = 0; j < marker_num; j++ ) {
        if( patt_id == marker_info[j].id ) {
            if( k == -1 ) k = j;
            else if( marker_info[k].cf < marker_info[j].cf ) k = j;
        }
    }
    if( k == -1 ) {
		// Donny: nothing detected
		OutputDebugString("Nothing Detected\n");
        return true;
    }

	/* get the transformation between the marker and the real camera */
    arGetTransMat(&marker_info[k], PATT_CENTER, PATT_WIDTH, lastKnownPosition);

	char buffer[1024];
	sprintf(buffer, "%f %f %f %f %f %f %f %f %f %f %f %f\n", lastKnownPosition[0][0], lastKnownPosition[0][1], lastKnownPosition[0][2], lastKnownPosition[0][3],
															 lastKnownPosition[1][0], lastKnownPosition[1][1], lastKnownPosition[1][2], lastKnownPosition[1][3],
															 lastKnownPosition[2][0], lastKnownPosition[2][1], lastKnownPosition[2][2], lastKnownPosition[2][3]);
	OutputDebugString(buffer);
}