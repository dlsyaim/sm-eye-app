#include "StdAfx.h"
#include ".\ed_testprotocole.h"
#include "corwlDefines.h"



ED_TestProtocole::ED_TestProtocole(void)
{
			

	/*static unsigned char m_abyModeValue[STI_NUM];
	static unsigned char m_abySubmodeNum[STI_NUM];
	static CString m_astrModeName[STI_NUM];*/
}

ED_TestProtocole::~ED_TestProtocole(void)
{
}


CString ED_TestProtocole::getStiModeName(byte sti) 
{
	CString strStiName[STI_NUM] = {"Calibration", 
							"Gaze A",
							"Gaze B",
							"Pursuit",
							"Saccade A",
							"Saccade B",
							"Saccade Random",
							"OKN",
							"LED",
							"Caloric"};

	return strStiName[sti];
}
byte ED_TestProtocole::getStiModeCommand(byte sti) 
{
	//caloric은 임의의 command설정. comm class에서 걸러준다.
	//byte modeCommand[STI_NUM] = {0x01, 0x02, 0x82, 0x03, 0x04, 0x84, 0x05, 0x06, 0xfe, 0xa1};
	byte modeCommand[STI_NUM] = {COMMAND_STI_CALIBRATION,		
								COMMAND_STI_GAZE_A,			
                                COMMAND_STI_GAZE_B,			
                                COMMAND_STI_PURSUIT,			
                                COMMAND_STI_SACCADE_A,		
                                COMMAND_STI_SACCADE_B,		
                                COMMAND_STI_SACCADE_RANDOM,		
                                COMMAND_STI_OKN,					
                                COMMAND_STI_LED,					
								COMMAND_STI_CALORIC};

	return modeCommand[sti];
}
byte ED_TestProtocole::getStiSubmodeNum(byte sti) 
{
	//submode의 개수
	byte submodeNum[STI_NUM] = {4, 8, 16, 16, 6, 6, 3, 12, 2, 4}; 
	return submodeNum[sti];
};

CString ED_TestProtocole::getStiSubmodeName(byte sti, byte submode) 
{
	

	CString strSubmodeCalibrationName[4] = {"10°", "15°","20°", "Gyro"};
	CString strSubmodeGazeAName[8] = {"H15°", "H18°", "H20", "H30", "V15°", "V18°", "V20", "V30"};
	CString strSubmodeGazeBName[16] = {"H15°", "H-15°", "H18°", "H-18°", "H20°", "H-20°", "H30", "H-30", 
										"V15°", "V-15°", "V18°", "V-18°", "V20°", "V-20°", "V30", "V-30"};
	CString strSubmodePursuitName[16] = {"H15°10sec", "H15°5sec", "H15°2.5sec", "H17°10sec", "H17°5sec", "H17°2.5sec", "H15°1.6660sec", "H17°1666sec",
										"V15°10sec", "V15°5sec", "V15°2.5sec", "V10°4sec", "V15°4sec", "V15°2.5sec", "V15°1.6660sec", "V17°1666sec",};
	CString strSubmodeSaccadeAName[6] = {"H15°-Center", "H20°-Center", "H30-Center°", "V10-Center°", "V15-Center°", "V30-Center°"};
	CString strSubmodeSaccadeBName[6] = {"H15°", "H20°", "H30°", "V10°", "V15°-", "V30°"};
	CString strSubmodeSaccadeRandName[3] = {"Random0", "Random1", "Random2"};
	CString strSubmodeOKNName[12] = {"H 31:1 20°/sec", 
									"H 29:3 20°/sec", 
									"H 27:5 20°/sec", 
									"H 25:7 20°/sec", 
									"H 22:10 20°/sec", 
									"H 17:15 20°/sec",
                                    "V 31:1 20°/sec", 
									"V 29:3 20°/sec", 
									"V 27:5 20°/sec", 
									"V 25:7 20°/sec", 
									"V 22:10 20°/sec", 
									"V 17:15 20°/sec"};
	CString strSubmodeLEDName[2] = {"ALL LED OFF", "Center LED ON"};
	CString strSubmodeCaloricName[4] = {"Left Warm", "Right Cool", "Left Cool", "Right Warm"};
	

	CString* pstrSubmodeName[STI_NUM] = {strSubmodeCalibrationName, strSubmodeGazeAName, strSubmodeGazeBName, strSubmodePursuitName, 
										strSubmodeSaccadeAName, strSubmodeSaccadeBName, strSubmodeSaccadeRandName, strSubmodeOKNName, 
										strSubmodeLEDName, strSubmodeCaloricName};


	return pstrSubmodeName[sti][submode];
}

double ED_TestProtocole::getStiValue(byte mode, byte submode)
{
	//OKN, Pursuit의 속도를 알려준다.
	double oknValue[12] = {20, 20, 20, 20, 20, 20, 
					20, 20, 20, 20, 20, 20};
	//Pursuit의 angular amplitude를 알려준다.
	double pursuitValue[16] = {15, 15, 15, 17, 17, 17, 15, 17,
								15,15, 15, 10, 15, 15, 15, 17};
	if(mode == getStiModeCommand(STI_OKN))
		return oknValue[submode];
	else if(mode == getStiModeCommand(STI_PURSUIT))
		return pursuitValue[submode];

	return 1;
}
bool ED_TestProtocole::getStiDirection(byte mode, byte submode)
{
	//OKN, Pursuit의 방향을 알려준다.
	bool oknValue[12] = {true, true, true, true, true, true, 
					false, false, false, false, false, false};
	bool pursuitValue[16] = {true, true, true, true, true, true, true, true,
								false, false, false, false, false, false, false, false};
	if(mode == getStiModeCommand(STI_OKN))
		return oknValue[submode];
	else if(mode == getStiModeCommand(STI_PURSUIT))
		return pursuitValue[submode];
	else return 0;
}


CString ED_TestProtocole::getAnalMethodName(unsigned char analMethod)
{
	if(analMethod >= ANAL_METHOD_NUM)
	{
		::AfxMessageBox("anal method constant error");
		return 0;
	}
	CString strAnalMethod[ANAL_METHOD_NUM]	= {"Calibration", "Nystagmus", "Gaze", "Pursuit", "Saccade", "Head Thrust"};
	return strAnalMethod[analMethod];
	
}
CString ED_TestProtocole::getAnalTargetName(unsigned char analTarget)
{
	if(analTarget >= ANAL_TARGET_NUM)
	{
		::AfxMessageBox("anal target constant error");
		return 0;
	}
	CString strAnalTarget[ANAL_TARGET_NUM] = {"Horizontal", "Vertical", "Torsional", "Radius"};
	return strAnalTarget[analTarget];

}