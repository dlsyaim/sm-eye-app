#define FOR_ASAN


#define DEL(x) { if(x) {delete []x; x = NULL;}}

#define PATIENT_INFO_FILENAME "PINFO.dat"
#define TEST_LIST_FILENAME "testList.xml"
#define TEST_INFO_FILENAME "testInfo.dat"
#define AVI_FILENAME "result.avi"

#define FRAMERATE 120
#define ACC_SAMPLERATE	FRAMERATE


#define MS20 (int(double(20)/(double(1000)/FRAMERATE)))
#define MS40 (int(double(20)/(double(1000)/FRAMERATE)))
#define MS50 (int(double(50)/(double(1000)/FRAMERATE)))
#define MS100 (int(double(100)/(double(1000)/FRAMERATE)))
#define MS200 (int(double(200)/(double(1000)/FRAMERATE)))
#define MS250 (int(double(250)/(double(1000)/FRAMERATE)))
#define MS300 (int(double(300)/(double(1000)/FRAMERATE)))
#define MS400 (int(double(400)/(double(1000)/FRAMERATE)))
#define MS500 (int(double(500)/(double(1000)/FRAMERATE)))

//display source
#define DISPLAY_SOURCE_CAM 1
#define DISPLAY_SOURCE_AVI 2


#define WM_EYEDATA_READ	(WM_USER+2)

#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 240

//camera grab type
#define LOCK_NEXT 1
#define LOCK_LATEST 2
//#define GRAB_TYPE   LOCK_NEXT
#define GRAB_TYPE   LOCK_LATEST


// DATA SOURCE


// data src

// chart index
#define HORIZONTAL 0
#define VERTICAL 1
#define TORSIONAL 2
#define RADIUS 3
#define ACC 4
#define RT_PROC	5//REAL TIME PROCESSING
#define HORIZONTAL_VERTICAL 10

#define VISIBLE_BIT_H (1<<HORIZONTAL)
#define VISIBLE_BIT_V (1<<VERTICAL)
#define VISIBLE_BIT_T (1<<TORSIONAL)
#define VISIBLE_BIT_R (1<<RADIUS)
#define VISIBLE_BIT_ACC (1<<ACC)
#define VISIBLE_BIT_RT_PROC (1<<RT_PROC)


#define LEFT	0
#define RIGHT	1
#define BOTH	2

#define NEGA 0
#define POSI 1
#define ALL 2


//chart
#define CHART_NUM	6
#define CHART_HORIZONTAL	HORIZONTAL
#define CHART_VERTICAL		VERTICAL
#define CHART_TORSIONAL		TORSIONAL
#define CHART_RADIUS		RADIUS
#define CHART_ACC			ACC
#define CHART_ANALYZE_0		RT_PROC


#define ANAL_METHOD_NUM		6

#define ANAL_METHOD_CALIBRATION 0
#define ANAL_METHOD_NYSTAGMUS 1
#define ANAL_METHOD_GAZE 2
#define ANAL_METHOD_PURSUIT 3
#define ANAL_METHOD_SACCADE 4
#define ANAL_METHOD_HEADTHRUST 5
#define ANAL_METHOD_CALORIC 6

#define ANAL_TARGET_NUM 4

#define ANAL_TARGET_HORIZONTAL 0
#define ANAL_TARGET_VERTICAL 1
#define ANAL_TARGET_TORSION 2
#define ANAL_TARGET_RADIUS 3

//stimulation
#define COMMAND_STI_CALIBRATION		0x01
#define COMMAND_STI_GAZE_A			0x02
#define COMMAND_STI_GAZE_B			0x82
#define COMMAND_STI_PURSUIT			0x03
#define COMMAND_STI_SACCADE_A		0x04
#define COMMAND_STI_SACCADE_B		0x84
#define COMMAND_STI_SACCADE_RANDOM		0x05
#define COMMAND_STI_OKN					0x06
#define COMMAND_STI_LED					0xfe
#define COMMAND_STI_CALORIC				0xa1









#define CHART_ANALYZE_NUM	8

#define SERIES_LEFT_EYE		LEFT
#define SERIES_RIGHT_EYE	RIGHT
#define SERIES_STIMULATION (RIGHT+1)
#define SERIES_NYSTAGMUS_LEFT (RIGHT+2)
#define SERIES_NYSTAGMUS_RIGHT (RIGHT+3)
#define SERIES_LEFT_EOG			(RIGHT+4)
#define SERIES_RIGHT_EOG		(RIGHT+5)
#define SERIES_EVENT			(RIGHT+6)



#define SERIES_GYRO_X 0
#define SERIES_GYRO_Y 1
#define SERIES_GYRO_Z 2



//chart color
#define COLOR_LEFT	RGB(113, 101, 223)
#define COLOR_RIGHT RGB(245, 100, 113)
#define COLOR_STI	RGB(7,254,254)
#define COLOR_TARGET COLOR_STI
#define COLOR_EVENT RGB(255, 0, 0)
#define clBgrd RGB(0,0,0)				//background
#define clAxis RGB(255,255,255)


#define clPlot0 COLOR_LEFT
#define clPlot1 COLOR_RIGHT
#define clPlot2 RGB(130,155,254)
#define clPlot3 RGB(252,209,36)
#define clPlot4 RGB(67, 153, 177)

#define COLOR_SELECTED RGB(0xff,0xff, 0xff)
#define COLOR_EDIT		RGB(0xef, 0xef, 0x00)


//TEST Á¾·ùµé
/*
#define TEST_CLASS_OKN			10
#define TEST_CLASS_HEAD_TRUST	20
#define TEST_CLASS_SACCADE		30
#define TEST_CLASS_GAZE			40
#define	TEST_CLASS_OKAN			50
#define TEST_CLASS_NYSTAGMUS	60*/

//#define IS_NYSTAGMUS(X)			((X==TEST_CLASS_OKN)||(X==TEST_CLASS_GAZE)||(X==TEST_CLASS_OKAN)||(X==TEST_CLASS_NYSTAGMUS))

//test direction
#define TEST_DIRECTION_H		0
#define TEST_DIRECTION_V		1

#define TEST_UNCALIBRATED		0
#define TEST_CALIBRATED			1


//eog gain
#define EOG_GAIN (50./8388608.)

