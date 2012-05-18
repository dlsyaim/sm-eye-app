#pragma once

#include "vfw.h"
#include "cv.h"


#define CV_CAP_PROP_POS_MSEC1 1
#define CV_CAP_PROP_POS_FRAMES1 2
#define CV_CAP_PROP_POS_AVI_RATIO1 3
#define CV_CAP_PROP_FRAME_WIDTH1 4
#define CV_CAP_PROP_FRAME_HEIGHT1 5
#define CV_CAP_PROP_FPS1 6
#define CV_CAP_PROP_FOURCC1 7
#define CV_CAP_PROP_FRAME_COUNT1 8

class CAviLoader
{
public:
	CAviLoader(void);
	~CAviLoader(void);

	bool open(const char* filename);
	void close();

	double getProperty(int);
    bool setProperty(int, double);
    bool grabFrame();
    unsigned char* retrieveFrame();

	double              fps;

protected:
    void init();

	PAVIFILE            avifile;
    PAVISTREAM          avistream;
    PGETFRAME           getframe;
    AVISTREAMINFO       aviinfo;
    BITMAPINFOHEADER  * bmih;
    CvSlice             film_range;
    
    int                 pos;
    int                 data_offset;
    unsigned char*      frame;
    CvSize              size;


};
