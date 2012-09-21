#include "StdAfx.h"
#include ".\aviloader.h"

////////////////////////////////////////////////////////////////////////
static void icvInitCapture_VFW()
{
    static int isInitialized = 0;
    if( !isInitialized )
    {
        AVIFileInit();
        isInitialized = 1;
    }
}

static BITMAPINFOHEADER icvBitmapHeader( int width, int height, int bpp, int compression = BI_RGB )
{
    BITMAPINFOHEADER bmih;
    memset( &bmih, 0, sizeof(bmih));
    bmih.biSize = sizeof(bmih);
    bmih.biWidth = width;
    bmih.biHeight = height;
    bmih.biBitCount = (WORD)bpp;
    bmih.biCompression = compression;
    bmih.biPlanes = 1;

    return bmih;
}

/////////////////////////////////////////////////////////////////////////////////////////


CAviLoader::CAviLoader(void)
{
	init();
	
}

CAviLoader::~CAviLoader(void)
{
	if(frame)
		delete []frame;
}


void CAviLoader::init()
{
    avifile = 0;
    avistream = 0;
    getframe = 0;
    memset( &aviinfo, 0, sizeof(aviinfo) );
    bmih = 0;
    film_range = cvSlice(0,0);
    fps = 0;
    pos = 0;
    data_offset = 0;
    frame = 0;
    size = cvSize(0,0);
}


void CAviLoader::close()
{
    if( getframe )
        AVIStreamGetFrameClose( getframe );

    if( avistream )
        AVIStreamRelease( avistream );

    if( avifile )
        AVIFileRelease( avifile );

    if(frame)
	{
		delete []frame;
		frame = NULL;
	}

    init();
}


bool CAviLoader::open( const char* filename )
{
    close();
    icvInitCapture_VFW();

    if( !filename )
        return false;

    HRESULT hr = AVIFileOpen( &avifile, filename, OF_READ, NULL );
    if( SUCCEEDED(hr))
    {
        hr = AVIFileGetStream( avifile, &avistream, streamtypeVIDEO, 0 );
        if( SUCCEEDED(hr))
        {
            hr = AVIStreamInfo( avistream, &aviinfo, sizeof(aviinfo));
            if( SUCCEEDED(hr))
            {
                //int fcc = aviinfo.fccHandler;
                data_offset = 0;
                size.width = aviinfo.rcFrame.right - aviinfo.rcFrame.left;
                size.height = aviinfo.rcFrame.bottom - aviinfo.rcFrame.top;
                BITMAPINFOHEADER bmih = icvBitmapHeader( size.width, size.height, 24 );

				if(frame)
					delete []frame;
				frame = new unsigned char[size.width*size.height*3];
                
                film_range.start_index = (int)aviinfo.dwStart;
                film_range.end_index = film_range.start_index + (int)aviinfo.dwLength;
                fps = (double)aviinfo.dwRate/aviinfo.dwScale;
                pos = film_range.start_index;
                getframe = AVIStreamGetFrameOpen( avistream, &bmih );
                if( getframe != 0 )
                    return true;
            }
        }
    }

    close();
    return false;
}


bool CAviLoader::grabFrame()
{
    if( avistream )
        bmih = (BITMAPINFOHEADER*)AVIStreamGetFrame( getframe, pos++ );
    return bmih != 0;
}


unsigned char* CAviLoader::retrieveFrame()
{
/*    if( avistream && bmih )
    {
        IplImage src;
        cvInitImageHeader( &src, cvSize( bmih->biWidth, bmih->biHeight ),
                           IPL_DEPTH_8U, 3, IPL_ORIGIN_BL, 4 );
        cvSetData( &src, (char*)(bmih + 1) + data_offset, src.widthStep );
        if( !frame || frame->width != src.width || frame->height != src.height )
        {
            cvReleaseImage( &frame );
            frame = cvCreateImage( cvGetSize(&src), 8, 3 );
        }
 
        cvFlip( &src, frame, 0 );
        return frame;
    }
*/
	::memcpy(frame, (char*)(bmih+1)+data_offset, size.width*size.height*3);
    return frame;
}


double CAviLoader::getProperty( int property_id )
{
    switch( property_id )
    {
    case CV_CAP_PROP_POS_MSEC1:
        return cvRound(pos*1000./fps);
    case CV_CAP_PROP_POS_FRAMES1:
        return pos;
    case CV_CAP_PROP_POS_AVI_RATIO1:
        return (pos - film_range.start_index)/
               (film_range.end_index - film_range.start_index + 1e-10);
    case CV_CAP_PROP_FRAME_WIDTH1:
        return size.width;
    case CV_CAP_PROP_FRAME_HEIGHT1:
        return size.height;
    case CV_CAP_PROP_FPS1:
        return fps;
    case CV_CAP_PROP_FOURCC1:
        return aviinfo.fccHandler;
    case CV_CAP_PROP_FRAME_COUNT1:
        return film_range.end_index - film_range.start_index;
    }
    return 0;
}


bool CAviLoader::setProperty( int property_id, double value )
{
	/*
    switch( property_id )
    {
    case CV_CAP_PROP_POS_MSEC1:
    case CV_CAP_PROP_POS_FRAMES1:
		pos = cvRound(value);
		break;
    case CV_CAP_PROP_POS_AVI_RATIO1:
        {
            switch( property_id )
            {
            case CV_CAP_PROP_POS_MSEC1:
                pos = cvRound(value*fps*0.001);
                break;
            case CV_CAP_PROP_POS_AVI_RATIO1:
                pos = cvRound(value*(film_range.end_index - 
                                     film_range.start_index) +
                              film_range.start_index);
                break;
            default:
                pos = cvRound(value);
            }
            if( pos < film_range.start_index )
                pos = film_range.start_index;
            if( pos > film_range.end_index )
                pos = film_range.end_index;
        }
        break;
    default:
        return false;
    }
*/
	pos = cvRound(value);
    return true;
}

