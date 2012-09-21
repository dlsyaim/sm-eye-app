#include "StdAfx.h"
#include ".\ed_treesearch.h"
#include "corwlDefines.h"


ED_TreeSearch::ED_TreeSearch(void)
{
}

ED_TreeSearch::~ED_TreeSearch(void)
{
}


bool ED_TreeSearch::getOcularMotorGaze(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* pList, char hv)
{
	//stimulaation이 gazeA, gazeB, pursuit, saccade, OKN
	bool bRtn = false;

	if(!((pTree)&&(pList)))
		return false;

	//리스트 삭제
	pList->RemoveAll();

	pTree->findTests(pList, COMMAND_STI_GAZE_A, hv);
	pTree->findTests(pList, COMMAND_STI_GAZE_B, hv);


	if(pList->GetCount())
		bRtn = true;


	return bRtn;
}

bool ED_TreeSearch::getOcularMotorPursuit(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* pList, char hv)
{
	//stimulaation이 gazeA, gazeB, pursuit, saccade, OKN
	bool bRtn = false;

	if(!((pTree)&&(pList)))
		return false;

	//리스트 삭제
	pList->RemoveAll();


	//pursuit
	pTree->findTests(pList, COMMAND_STI_PURSUIT, hv);

	
	if(pList->GetCount())
		bRtn = true;


	return bRtn;

}
bool ED_TreeSearch::getOcularMotorSaccade(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* pList, char hv)
{
	//stimulaation이 gazeA, gazeB, pursuit, saccade, OKN
	bool bRtn = false;

	if(!((pTree)&&(pList)))
		return false;

	//리스트 삭제
	pList->RemoveAll();

	pTree->findTests(pList, COMMAND_STI_SACCADE_A, hv);
	pTree->findTests(pList, COMMAND_STI_SACCADE_B, hv);
	pTree->findTests(pList, COMMAND_STI_SACCADE_RANDOM, hv);


	if(pList->GetCount())
		bRtn = true;


	return bRtn;
}
bool ED_TreeSearch::getOcularMotorOKN(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* pList, char hv)
{

	bool bRtn = false;

	if(!((pTree)&&(pList)))
		return false;

	//리스트 삭제
	pList->RemoveAll();

	pTree->findTests(pList, COMMAND_STI_OKN, hv);


	if(pList->GetCount())
		bRtn = true;


	
	return bRtn;
}

bool ED_TreeSearch::getCalibrationTest(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* pList)
{
	//stimulation이 calibration

	bool bRtn = false;

	if(!((pTree)&&(pList)))
		return false;

	//리스트 삭제
	pList->RemoveAll();

	pTree->findTests(pList, COMMAND_STI_CALIBRATION);


	if(pList->GetCount())
		bRtn = true;


	
	return bRtn;
}


bool ED_TreeSearch::getCaloricTest(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* pList)
{
	//stimulation이 caloric

	bool bRtn = false;

	return bRtn;
}


bool ED_TreeSearch::getNystagmusTest(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* pList)
{
	bool bRtn = false;

	if(!((pTree)&&(pList)))
		return false;

	//리스트 삭제
	pList->RemoveAll();

	pTree->findTests(pList, COMMAND_STI_LED, HORIZONTAL_VERTICAL);


	if(pList->GetCount())
		bRtn = true;

	return bRtn;
}

bool ED_TreeSearch::getHeadThrust(CTreeCtrlXML* pTree, CList<ED_TestInfo*, ED_TestInfo*>* pList)
{
	bool bRtn = false;
	CList<ED_TestInfo*, ED_TestInfo*> list;
//	getTests(&list, clibration, analysis);

	return bRtn;
}