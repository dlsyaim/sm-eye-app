#pragma once

#define STI_NUM 10

//stimulation mode
#define STI_CALIBRATION			0x00
#define STI_GAZE_A				0x01
#define STI_GAZE_B				0x02
#define STI_PURSUIT				0x03
#define STI_SACCADE_A			0x04
#define STI_SACCADE_B			0x05
#define STI_SACCADE_RANDOM		0x06
#define STI_OKN					0x07
#define STI_LED					0x08



class ED_TestProtocole
{
public:
	ED_TestProtocole(void);
	~ED_TestProtocole(void);


	static CString getStiModeName(byte sti);
	static byte getStiModeCommand(byte sti);
	static byte getStiSubmodeNum(byte sti);
	static CString getStiSubmodeName(byte sti, byte submode);
	static double getStiValue(byte mode, byte submode);
	static bool getStiDirection(byte mode, byte submode);
	

	static CString getAnalMethodName(unsigned char analMethod);
	static CString getAnalTargetName(unsigned char analTarget);
};

