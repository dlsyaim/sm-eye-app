#pragma once
#include "ed_testinfo.h"
#include "treectrlXML.h"

class ED_TreeSearch
{
public:
	ED_TreeSearch(void);
	~ED_TreeSearch(void);

	//bool getOcularMotorTest(CTreeCtrl* pTree, CList<ED_TestInfo*, ED_TestInfo*>* Plist, char hv);
	static bool getOcularMotorGaze(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* Plist, char hv);
	static bool getOcularMotorPursuit(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* Plist, char hv);
	static bool getOcularMotorSaccade(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* Plist, char hv);
	static bool getOcularMotorOKN(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* Plist, char hv);


	static bool getNystagmusTest(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* Plist);
	static bool getCaloricTest(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* Plist);
	static bool getCalibrationTest(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* Plist);
	static bool getHeadThrust(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* Plist);
};
