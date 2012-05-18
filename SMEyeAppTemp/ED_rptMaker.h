#pragma once

#include "ED_PatientInfo.h"
#include "treectrlxml.h"
#include "DlgMakeTChartImage.h"

#define HEADER_GAZE_H 0
#define HEADER_PURSUIT_H 1
#define HEADER_SACCADE_H 2
#define HEADER_OKN_H		3
#define HEADER_GAZE_V 4
#define HEADER_PURSUIT_V 5
#define HEADER_SACCADE_V 6
#define HEADER_OKN_V		7
#define HEADER_NYSTAGMUS 8



class ED_rptMaker
{
public:
	ED_rptMaker(void);
	~ED_rptMaker(void);

	CString getPrintFolder(){return m_strPrintTempFolder;};

private:
	unsigned char m_ucPageNum;
	double m_dCurrentLine;		// line num이 page height를 넘어가면 다음 페이지로 간다.


	
	CFile m_file;
	CString m_strFormFolder;
	CString m_strPrintTempFolder;
	CString m_strSubjectFolder;

	ED_PatientInfo m_patientInfo;
	
	double setTextBlcokOffset(CString* pStrText, double currentLine); 

	bool addPage();
	void addTitle(ED_PatientInfo);
	void addTable(CString title, CString* values, int count, CString strTitleJustify);
	void addText(CString label, int templateIdx);	//가장 큰 텍스트 라인
	void addBlankLine();
	void addHeader(int nHeader);

	void addBlockOcularMotorTest(CTreeCtrlXML* pTree, int hv, bool bCchart=false );
	void addBlockNystagmusTest(CTreeCtrlXML* pTree, bool bCchart = false);
	void addBlockCaloricTest(CTreeCtrlXML* pTree, bool bCchart = false);
	void addBlockCalibrationTest(CTreeCtrlXML* pTree, bool bCchart = false);
	void addBlockHeadThrustTest(CTreeCtrlXML* pTree, bool bCchart = false);
	void addBlockMemo();

	void addBlockCharts(CTreeCtrlXML* pTree);
	void addChart(ED_TestInfo* pTest, double chartAxisDuration = 40);
	unsigned int makeChartImages(ED_TestInfo* pTest, CList<printImgParam, printImgParam>* pList, int chartNum, int bottomAxisDuration = 40);
	bool addImageToReport(printImgParam imgParam);
	int m_nChartNum;

	static double getBottom(CString str);
	void getLineTopBottom(CString strLine, double* pTop, double* pBottom);
	double setLineTopBottomOffset(CString* pstrLine, double offset);


public:
	CList<CString, CString> m_listReportFiles;

	bool init(CString strFolder);
	void setFormFoler(CString folder);
	void setPatientInfo(ED_PatientInfo pinfo) { m_patientInfo = pinfo;};

	void makeReport(CTreeCtrlXML* pTree);
	



	void close();
};
