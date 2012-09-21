#include "ED_EyeInfo.h"


#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 240



//PARAMETERS
#define SM_PUPIL_THRESHOLD	1
#define SM_FIND_TORSION	2
#define SM_DRAW_RESULT 3




extern "C" _declspec(dllimport) void smGetEyeInfo(IplImage* pImg0, ED_EyeInfo* pEyeInfo0, IplImage* pImg1, ED_EyeInfo* pEyeInfo1, bool bFirstImage);
extern "C" _declspec(dllimport) void smGetEyeInfoMono(IplImage* pImg, ED_EyeInfo* pEyeInfo, IplImage* pImageResult, int camIdx, bool bFirstImage);
extern "C" _declspec(dllimport) double smGetParameter(DWORD ParamName, int index);
extern "C" _declspec(dllimport) void smSetParameter(DWORD ParamName, double param, int index);
extern "C" _declspec(dllimport) void smSetOutputWindow(CWnd* pWnd);

