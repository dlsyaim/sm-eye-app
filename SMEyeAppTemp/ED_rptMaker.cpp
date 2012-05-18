#include "StdAfx.h"
#include ".\ed_rptmaker.h"
#include "corwlUtil.h"
#include "ed_treeSearch.h"
#include "corwldefines.h"

#define A4_PAGE_HEIGHT	11.5
#define LINE_GAP		0.1
#define BLANK_LINE_HEIGHT	0.2
#define TEXT1_HEIGHT	0.2
#define TEXT2_HEIGHT	0.2

#include "math.h"

#include "dlganalyzepursuit.h"
#include "dlgAnalSaccade.h"
#include "dlganalnystag.h"
//#include "DlgMakeTChartImage.h"

ED_rptMaker::ED_rptMaker(void)
{
	
}

ED_rptMaker::~ED_rptMaker(void)
{
}

bool ED_rptMaker::init(CString strFolder)
{
	CString strProgram;
	corGetProgramFolder(&strProgram);

	//subject folder
	this->m_strSubjectFolder = strFolder;

	//from이 들어 있는 폴더
	this->m_strFormFolder = strProgram + "\\form";
	//print에 필요한 파일들을 생성할 폴더
	m_strPrintTempFolder = strFolder + "\\printTemp";
	//없다면 만든다.
	if(!corFolderExist(m_strPrintTempFolder))
		corCreateDirectory(m_strPrintTempFolder);


	if(m_file.m_hFile != CFile::hFileNull)
		m_file.Close();

	this->m_ucPageNum = 0;
	this->m_dCurrentLine = 0;
	m_listReportFiles.RemoveAll();
	m_nChartNum = 0;

	

	return addPage();

	
}

bool ED_rptMaker::addPage()
{
	//addHader()혹은 addText()에서만 불린다.

	m_ucPageNum++;
	m_dCurrentLine = 0;

	//열려 있는 파일이 있다면 닫는다.
	if(m_file.m_hFile != CFile::hFileNull)
		m_file.Close();

	CString strFile;
	strFile.Format("%s\\page%d.rpt",m_strPrintTempFolder, m_ucPageNum);
	if(m_file.Open(strFile, CFile::modeCreate | CFile::modeWrite))
	{
		this->m_listReportFiles.AddTail(strFile);

		if(m_ucPageNum)	//두번째 페이지부터는. 첫째줄이 안나온다는 보고가 있어서 넣어봤다.
		{
			this->addText("", 1);
			this->addText("", 1);
		}


		return true;
	}

	return false;
}


void ED_rptMaker::close()
{
	if(m_file.m_hFile != CFile::hFileNull)
		m_file.Close();
}

void ED_rptMaker::makeReport(CTreeCtrlXML* pTree)
{
	::AfxGetApp()->BeginWaitCursor();


	//titles
	addTitle(this->m_patientInfo);

	//blank line
	addText("", 1);

	//add test blocks
	addBlockOcularMotorTest(pTree, HORIZONTAL);

	addBlockOcularMotorTest(pTree, VERTICAL);

	addBlockNystagmusTest(pTree);

	addBlockCaloricTest(pTree);

	addBlockCalibrationTest(pTree);

	addBlockHeadThrustTest(pTree);

	addBlockMemo();

		
	//add Chart images
	addBlockCharts(pTree);

	::AfxGetApp()->EndWaitCursor();
	
}

void ED_rptMaker::addBlockCharts(CTreeCtrlXML* pTree)
{
	//add new page for chart
	addPage();
	addBlockOcularMotorTest(pTree, HORIZONTAL, true);
	addBlockOcularMotorTest(pTree, VERTICAL, true);
	addBlockNystagmusTest(pTree, true);
	addBlockCaloricTest(pTree, true);
	addBlockCalibrationTest(pTree, true);
	addBlockHeadThrustTest(pTree, true);
	
}


void ED_rptMaker::addHeader(int nHeader)
{
	CString fnames[] = {"header_gaze_h.rpt", "header_pursuit_h.rpt", "header_saccade_h.rpt", "header_okn_h.rpt",
						"header_gaze_v.rpt", "header_pursuit_v.rpt", "header_saccade_v.rpt", "header_okn_v.rpt", 
						 "header_nystagmus.rpt"};

	CFile formFile;
	if(formFile.Open(m_strFormFolder + "\\" + fnames[nHeader], CFile::modeRead))
	{
		//파일을 읽어 cstring으로 만든다.
		unsigned char* pBuf = new unsigned char[size_t(formFile.GetLength())+1];
		pBuf[formFile.GetLength()] = NULL;
		formFile.Read(pBuf, UINT(formFile.GetLength()));
		CString str(pBuf);
		delete []pBuf;
		formFile.Close();

		//현재 last line으로 offset을 주고 
		// block의 bottom을 리턴한다.
		//double currentLine = m_dCurrentLine;
		CString strOrigin = str;
		double currentLine = setTextBlcokOffset(&str, m_dCurrentLine) + LINE_GAP;
	
		//page를 넘어가게 되면 
		if(currentLine > A4_PAGE_HEIGHT)
		{
			addPage();
			currentLine = this->setTextBlcokOffset(&str, m_dCurrentLine) + LINE_GAP;
		}

		if(m_file.m_hFile != CFile::hFileNull)
		{
			
			m_file.Write(str.GetBuffer(), str.GetLength());
				this->m_dCurrentLine = currentLine;
		}

	}
}

double ED_rptMaker::setTextBlcokOffset(CString* pStrText, double currentLine)
{
	//currentline에 block이 위치하도록
	// top, bottom을 조절한다.
	// 가장 밑의 bottom 값을 리턴한다.

	CString strOrigin = *pStrText;
	CString strOffset;

	double top, bottom;
	double finalBottom = 0;	//가장 큰 bottom 값

	int curPos = 0;
	CString resToken, firstLine;
	//첫번째 줄을 구한 후 offset값을 결정한다.
	firstLine = resToken = strOrigin.Tokenize("\n", curPos);

	//첫째줄에 paper 가 있다면 그 줄은 무시하고 다음줄을 구한다.
	if(firstLine.Find("paper", 0) != -1)
		firstLine = resToken = strOrigin.Tokenize("\n", curPos);


	getLineTopBottom(firstLine, &top, &bottom);
	double offset = currentLine-top;
	//top, bottom 값에 offset값을 더한 text를 얻는다.
	finalBottom = setLineTopBottomOffset(&firstLine, offset);
	
	
	//리턴 text에 더해준다.
	strOffset += (firstLine + "\n");


	while(resToken != "")
	{
		//한줄씩 얻는다.
		resToken = strOrigin.Tokenize("\n", curPos);
		if(resToken != "")
		{
			//offset을 setting한다.
			bottom = setLineTopBottomOffset(&resToken, offset);
			if(bottom > finalBottom)
				finalBottom = bottom;
			strOffset += (resToken + "\n");
		}
	}

	
	*pStrText = strOffset;
	return finalBottom;

}

void ED_rptMaker::getLineTopBottom(CString strLine, double* pTop, double* pBottom)
{
	*pTop = *pBottom = 0;

	int curPos = 0;
	int num = 0;
	CString resToken;
	resToken = strLine.Tokenize(":,", curPos);
	num++;
	while(resToken != "")
	{
		resToken = strLine.Tokenize(":,", curPos);
		num++;
		if(num == 3)
			*pTop = atof(resToken);
		else if(num == 5)
			*pBottom = atof(resToken);
	}
	
}
double ED_rptMaker::setLineTopBottomOffset(CString* pstrLine, double offset)
{
	pstrLine->Replace(",,,,", ", , , ,");
	pstrLine->Replace(",,,", ", , ,");
	pstrLine->Replace(",,", ", ,");

	//double bottom = 0;
	CString strOffset;

	double top, bottom;
	top = bottom = 0;
	CString strTemp;

	int curPos = 0;
	int num = 0;
	CString resToken;
	resToken = pstrLine->Tokenize(":,", curPos);
	num++;
	strOffset += (resToken + ":");

	while(resToken != "")
	{
		if(num != 1)
			strOffset += ",";

		resToken = pstrLine->Tokenize(":,", curPos);
		num++;
		if(num == 3)
		{
			top = atof(resToken) + offset;
			strTemp.Format("%.6f", top);
			strOffset += strTemp;
		}
		else if(num == 5)
		{
			bottom = atof(resToken) + offset;
			strTemp.Format("%.6f", bottom);
			strOffset += strTemp;
		}
		else
			strOffset += resToken;
		

	}

	strOffset = strOffset.Left(strOffset.GetLength()-1);

	*pstrLine = strOffset;
	return bottom;
}



void ED_rptMaker::addTitle(ED_PatientInfo pi)
{
	this->m_patientInfo = pi;

	CFile formFile;
	if(formFile.Open(this->m_strFormFolder + "\\title.rpt", CFile::modeRead))
	{
		unsigned char* pBuf = new unsigned char[size_t(formFile.GetLength())+1];
		pBuf[formFile.GetLength()] = 0;

		formFile.Read(pBuf, UINT(formFile.GetLength()));
		formFile.Close();
		CString str(pBuf);

		CString temp;

		str.Replace("SUBJECT_ID", pi.m_chID);
		str.Replace("SUBJECT_NAME", pi.m_chName);
		temp.Format("%s %d",  pi.m_nSex ? "Male":"Femail", pi.m_nAge);
		str.Replace("SUBJECT_SEX_AGE", temp);
		temp.Format("%d/%02d/%02d", pi.m_timeTime.GetYear(), pi.m_timeTime.GetMonth(), pi.m_timeTime.GetDay());
		str.Replace("SUBJECT_DATE", temp);
		temp.Format("%02d-%02d-%02d", pi.m_timeTime.GetHour(), pi.m_timeTime.GetMinute(), pi.m_timeTime.GetSecond());
		str.Replace("SUBJECT_TIME", temp);


		if(m_file.m_hFile != CFile::hFileNull)
			m_file.Write(str.GetBuffer(), str.GetLength());
		
		// text의 bottom 확인
		this->m_dCurrentLine = ED_rptMaker::getBottom(str) + LINE_GAP;
		delete []pBuf;
	}

}

void ED_rptMaker::addBlankLine()
{
	m_dCurrentLine = m_dCurrentLine + BLANK_LINE_HEIGHT;
}

 
void ED_rptMaker::addTable(CString title, CString* values, int count, CString strTitleJustify)
{
	//titleJustify 0(LEFT), 1(CENTER), 2(RIGHT)

	if(m_dCurrentLine + LINE_GAP + TEXT1_HEIGHT > A4_PAGE_HEIGHT)
		addPage();
	//comma는 허용되지 않는다.


	// title
	title.Replace(",", "_");
	CString text;
	double textHeight = TEXT1_HEIGHT;
	text = "report_label:LEFT,TOP,RIGHT,BOTTOM,TEXT,,0,Times New Roman,FONTSIZE,0,0,0,0,0,JUSTIFY,0;\n";

	CString strTop, strLeft, strBottom, strRight, strText, strFont, strJustify;
	//LEFT
	strLeft = "0.2";
	text.Replace("LEFT", strLeft);
	//RIGHT
	strRight = "2.8";
	text.Replace("RIGHT", strRight);

	//top
	strTop.Format("%f", m_dCurrentLine);
	text.Replace("TOP", strTop);
	
	//bottom
	strTop.Format("%f", m_dCurrentLine + TEXT1_HEIGHT);
	text.Replace("BOTTOM", strTop);
	
	//text
	text.Replace("TEXT", title);

	//font
	strFont = "100";
	text.Replace("FONTSIZE", strFont);

	//JUSTIFY
	text.Replace("JUSTIFY", strTitleJustify);
	

	//write
	if(m_file.m_hFile != CFile::hFileNull)
		m_file.Write(text.GetBuffer(), text.GetLength());

	//values
	float left = 3.;
	float width = (8-left)/count;
	for(int i=0; i<count; i++)
	{
		text = "report_label:LEFT,TOP,RIGHT,BOTTOM,TEXT,,0,Times New Roman,FONTSIZE,0,0,0,0,0,0,0;\n";
		//LEFT
		strLeft.Format("%.2f", left + width*i);
		text.Replace("LEFT", strLeft);
		//RIGHT
		strRight.Format("%.2f", left + width*(i+1));
		text.Replace("RIGHT", strRight);

		//top
		strTop.Format("%f", m_dCurrentLine);
		text.Replace("TOP", strTop);
		
		//bottom
		strTop.Format("%f", m_dCurrentLine + TEXT1_HEIGHT);
		text.Replace("BOTTOM", strTop);
		
		//text
		text.Replace("TEXT", values[i]);

		//font
		strFont = "120";
		text.Replace("FONTSIZE", strFont);

		//write
		if(m_file.m_hFile != CFile::hFileNull)
			m_file.Write(text.GetBuffer(), text.GetLength());
	}
	
	

	

	//
	m_dCurrentLine += textHeight;

}

void ED_rptMaker::addText(CString label, int templateIdx)
{
	//page를 넘어섰는지 확인하고 넘었으면 다음 페이지를 만든다.
	if(m_dCurrentLine + LINE_GAP + TEXT1_HEIGHT > A4_PAGE_HEIGHT)
		addPage();

	//comma는 허용되지 않는다.
	label.Replace(",", "_");


	CString text;
	double textHeight = 0;
	switch(templateIdx)
	{
	case 1:
		text = "report_label:0.15,TOP,10.0 ,BOTTOM,TEXT,,0,휴먼엑스포,120,1,0,0,0,0,0,129;\n";
		textHeight = TEXT1_HEIGHT;
		break;
	case 2:
		text = "report_label:0.3,TOP,10.0 ,BOTTOM,TEXT,,0,휴먼엑스포,120,0,0,0,0,0,0,129;\n";
		textHeight = TEXT2_HEIGHT;
		break;

	}
	
	

	//top
	CString strTop;
	strTop.Format("%f", m_dCurrentLine);
	text.Replace("TOP", strTop);
	
	//bottom
	strTop.Format("%f", m_dCurrentLine + TEXT1_HEIGHT);
	text.Replace("BOTTOM", strTop);
	
	//label
	text.Replace("TEXT", label);


	//write
	if(m_file.m_hFile != CFile::hFileNull)
		m_file.Write(text.GetBuffer(), text.GetLength());

	//
	m_dCurrentLine += textHeight;
}

double ED_rptMaker::getBottom(CString str)
{
	
	double bottom = 0;


	int curPos = 0;
	CString resToken, lastLine;
	//한줄씩 나눠서 마지막 줄을 얻는다.
	lastLine = resToken = str.Tokenize("\n", curPos);
	while(resToken != "")
	{
		lastLine = resToken;
		resToken = str.Tokenize("\n", curPos);
	}

	//마지막 줄의 5번째가  bottom 위치
	str = lastLine;
	curPos = 0;
	int num = 0;
	resToken = str.Tokenize(":,", curPos);
	num++;
	while(resToken != "")
	{
		resToken = str.Tokenize(":,", curPos);
		num++;
		if(num == 5)
		{
			bottom = atof(resToken);
			break;
		}
	}

	return bottom;
}

void ED_rptMaker::addBlockOcularMotorTest(CTreeCtrlXML* pTree, int hv, bool bChart)
{
	CList<ED_TestInfo* , ED_TestInfo*> listGaze, listPursuit, listSaccade, listOKN;
	ED_TreeSearch::getOcularMotorGaze(pTree, &listGaze, hv);
    ED_TreeSearch::getOcularMotorPursuit(pTree, &listPursuit, hv);
	ED_TreeSearch::getOcularMotorSaccade(pTree, &listSaccade, hv);
	ED_TreeSearch::getOcularMotorOKN(pTree, &listOKN, hv);

	CString strResultLine;

	//ADD HORIZONTAL OCULAR MOTOR TEST
	
	if (listGaze.GetCount() ||
		listPursuit.GetCount() ||
		listSaccade.GetCount() ||
		listOKN.GetCount())
	{
		CString text;
		text.Format("%s OCULAR MOTOR TESTS", (hv==HORIZONTAL) ? "HORIZONTAL" : "VERTICAL");
		this->addText(text, 1);

		POSITION pos;
		ED_TestInfo* pTest = NULL;

		//gaze가 있으면 gaze header를 붙이고 결과를 적는다.
		if(listGaze.GetCount())
		{
			if(!bChart)
				this->addHeader((hv == HORIZONTAL) ? HEADER_GAZE_H : HEADER_GAZE_V);
			
			pos = listGaze.GetHeadPosition();
			for(int i=0; i<listGaze.GetCount(); i++)
			{
				pTest = listGaze.GetNext(pos);
				if(pTest)
				{
					if(!bChart)
					{
						//analyze
						CString fname(pTest->m_chFileName);

						CDlgAnalNystag dlgAnal;
						dlgAnal.setTestFile(this->m_strSubjectFolder + fname);
						structPrintNystag nystag[2];
						dlgAnal.analyzeForPrint(nystag);

                        //test name
					
						fname.Replace(".emd", "");
						CString values[5];
						values[0].Format("%.1f", nystag[LEFT].LeftUpVel);
						values[1].Format("%.1f", nystag[LEFT].RightDownVel);
						values[2].Format("%.1f", nystag[RIGHT].LeftUpVel);
						values[3].Format("%.1f", nystag[RIGHT].RightDownVel);
						values[4].Format("%.1f", nystag[LEFT].region);
						this->addTable(fname, values, 5, "0");

						
					}
					else
						addChart(pTest, 40);
					



				}


			}

			addText("", 1);


		}

		//pursuit가 있으면 pursuit header를 붙이고 결과를 적는다.
		if(listPursuit.GetCount())
		{
			if(!bChart)
                this->addHeader((hv == HORIZONTAL) ? HEADER_PURSUIT_H : HEADER_PURSUIT_V);

			pos = listPursuit.GetHeadPosition();
			for(int i=0; i<listPursuit.GetCount(); i++)
			{
				//test info를 읽는다.
				pTest = listPursuit.GetNext(pos);
				if(pTest)
				{
					if(!bChart)
					{
						CString fname(pTest->m_chFileName);

						CDlgAnalyzePursuit dlgAnal;
						dlgAnal.setTestFile(m_strSubjectFolder+pTest->m_chFileName);
						structPrintPursuit pursuit[2];
						dlgAnal.analyzeForPrint(pursuit);

						fname.Replace(".emd", "");
						CString values[4];
						values[0].Format("%.2f", pursuit[LEFT].gain[LEFT]);
						values[1].Format("%.2f", pursuit[LEFT].gain[RIGHT]);
						values[2].Format("%.2f", pursuit[RIGHT].gain[LEFT]);
						values[3].Format("%.2f", pursuit[RIGHT].gain[RIGHT]);
						this->addTable(fname, values, 4, "0");
                      
					}
					else
						addChart(pTest, 40);

				}
			}
			addText("", 1);
		}

		//saccade가 있으면 saccade header를 붙이고 결과를 적는다.
		if(listSaccade.GetCount())
		{
			if(!bChart)
				this->addHeader((hv == HORIZONTAL) ? HEADER_SACCADE_H : HEADER_SACCADE_V);

			pos = listSaccade.GetHeadPosition();
			for(int i=0; i<listSaccade.GetCount(); i++)
			{
				//test info를 읽는다.
				pTest = listSaccade.GetNext(pos);
				if(pTest)
				{
					if(!bChart)
					{
						CString fname(pTest->m_chFileName);

						CDlgAnalSaccade dlgAnal;
						dlgAnal.setTestFile(m_strSubjectFolder+pTest->m_chFileName);
						structPrintSaccade saccade[2];
						dlgAnal.analyzeForPrint(saccade);	

						//TITLE만 한줄 붙인다.
						fname.Replace(".emd", "");
						addTable(fname, NULL, 0, "0");



						//LEFT EYE 라인						
						CString values[6];
						values[0].Format("%.1f", saccade[0].peakVel[0]);
						values[1].Format("%.1f", saccade[0].peakVel[1]);
						values[2].Format("%.1f", saccade[0].latency[0]);
						values[3].Format("%.1f", saccade[0].latency[1]);
						values[4].Format("%.1f", saccade[0].accuracy[0]);
						values[5].Format("%.1f", saccade[0].accuracy[1]);
						this->addTable("Left_Eye", values, 6, "2");

						//RIGHT EYE 라인
						values[0].Format("%.1f", saccade[1].peakVel[0]);
						values[1].Format("%.1f", saccade[1].peakVel[1]);
						values[2].Format("%.1f", saccade[1].latency[0]);
						values[3].Format("%.1f", saccade[1].latency[1]);
						values[4].Format("%.1f", saccade[1].accuracy[0]);
						values[5].Format("%.1f", saccade[1].accuracy[1]);
						this->addTable("Right_Eye", values, 6, "2");



					}
					else
						addChart(pTest, 40);
				}
			}

			addText("", 1);
			
		}

		//OKN가 있으면 OKN header를 붙이고 결과를 적는다.
		if(listOKN.GetCount())
		{
			if(!bChart)
                this->addHeader((hv == HORIZONTAL) ? HEADER_OKN_H : HEADER_OKN_V);

			pos = listOKN.GetHeadPosition();
			for(int i=0; i<listOKN.GetCount(); i++)
			{
				//test info를 읽는다.
				pTest = listOKN.GetNext(pos);
				if(pTest)
				{
					if(!bChart)
					{
						CString fname(pTest->m_chFileName);

						CDlgAnalNystag dlgAnal;
						dlgAnal.setTestFile(m_strSubjectFolder+pTest->m_chFileName);
						structPrintNystag nystag[2];
						dlgAnal.analyzeForPrint(nystag);

						fname.Replace(".emd", "");
						CString values[6];
						values[0].Format("%.2f", fabs(nystag[LEFT].LeftUpVel/nystag[LEFT].targetVel));
						values[1].Format("%.2f", fabs(nystag[LEFT].RightDownVel/nystag[LEFT].targetVel));
						values[2].Format("%.2f", fabs(nystag[RIGHT].LeftUpVel/nystag[RIGHT].targetVel));
						values[3].Format("%.2f", fabs(nystag[RIGHT].RightDownVel/nystag[RIGHT].targetVel));
						values[4].Format("%.2f", nystag[LEFT].region);
						values[5].Format("%.2f", nystag[RIGHT].region);
						this->addTable(fname, values, 6, "0");


					}
					else
						addChart(pTest, 40);

				}
			}

			addText("", 1);
			
		}
		addText("", 1);
	}
}

void ED_rptMaker::addBlockNystagmusTest(CTreeCtrlXML* pTree, bool bChart)
{
	CList<ED_TestInfo*, ED_TestInfo*> list;
	if(ED_TreeSearch::getNystagmusTest(pTree, &list))
	{
		addText("NYSTAGMUS TEST", 1);

		if(!bChart)
			addHeader(HEADER_NYSTAGMUS);

		ED_TestInfo* pTest = NULL;
		CString strResultLine;
		POSITION pos = list.GetHeadPosition();
		for(int i=0; i<list.GetCount(); i++)
		{
			//test info를 읽는다.
			pTest = list.GetNext(pos);
			if(pTest)
			{
				if(!bChart)
				{
					CString fname(pTest->m_chFileName);

					CDlgAnalNystag dlgAnal;
					dlgAnal.setTestFile(m_strSubjectFolder+pTest->m_chFileName);
					structPrintNystag nystag[2];
					dlgAnal.analyzeForPrint(nystag);

					fname.Replace(".emd", "");
					CString values[8];
					values[0].Format("%.1f", nystag[LEFT].LeftUpVel);
					values[1].Format("%.1f", nystag[LEFT].RightDownVel);
					values[2].Format("%.1f", 0);
					values[3].Format("%.1f", 0);
					values[4].Format("%.1f", nystag[RIGHT].LeftUpVel);
					values[5].Format("%.1f", nystag[RIGHT].RightDownVel);
					values[6].Format("%.1f", 0);
					values[7].Format("%.1f", 0);
					this->addTable(fname, values, 8, "0");			
				}
				else
					addChart(pTest, 30);	//nystagmus의 경우  bottom axis를 30으로 

			}
		}

		addText("", 1);
	}
}
void ED_rptMaker::addBlockCaloricTest(CTreeCtrlXML* pTree, bool bChart)
{
	//Caloric
	
	/*
	//새로운 페이지에서 하자
	
	addPage();

	CList<ED_TestInfo*, ED_TestInfo*> list;
	if(ED_TreeSearch::getCaloricTest(pTree, &list))
	{
		addText("CALORIC TEST", 1);




		addText("",1);
	}*/

}
void ED_rptMaker::addBlockCalibrationTest(CTreeCtrlXML* pTree, bool bChart)
{
	// Calibration
	addText("CALIBRATION", 1);
	CList<ED_TestInfo*, ED_TestInfo*> list;
	if(ED_TreeSearch::getCalibrationTest(pTree, &list))
	{
		addText("Done",2);
	}
	else
		addText("Default",2);
	
	addText("",1);
	
}
void ED_rptMaker::addBlockHeadThrustTest(CTreeCtrlXML* pTree, bool bChart)
{
	//head Thrust
	CList<ED_TestInfo*, ED_TestInfo*> list;
	if(ED_TreeSearch::getHeadThrust(pTree, &list))
	{
		addText("HEAD THRUST TEST", 1);

	

		addText("",1);
	}
}


void ED_rptMaker::addBlockMemo()
{
	CString str(this->m_patientInfo.m_chMemo);
	if(str != "")
	{
		addText("MEMO", 1);
		addText(str, 2);
	}
}

#define PRINT_CHART_HEIGHT 1.9

void ED_rptMaker::addChart(ED_TestInfo* pTest, double chartAxisDuration)
{
	CList<printImgParam, printImgParam> listImg;
	this->m_nChartNum = makeChartImages(pTest, &listImg, m_nChartNum, chartAxisDuration);

	int count = listImg.GetCount();
	POSITION pos = listImg.GetHeadPosition();
	for(int i=0; i<count; i++)
	{
		printImgParam imgName = listImg.GetNext(pos);
		addImageToReport(imgName);
	}

	
}

unsigned int ED_rptMaker::makeChartImages(ED_TestInfo* pTest, CList<printImgParam, printImgParam>* pList, int chartNum, int bottomAxisDuration)
{
	//일단 horizontal vertical 만 그리자.
	CDlgMakeTChartImage dlg;
	dlg.m_chartNum = chartNum;
	dlg.m_pTI = pTest;
	dlg.m_strPrintTempFolder = this->m_strPrintTempFolder;
	dlg.m_strSubjectFolder = this->m_strSubjectFolder;

	dlg.setBottomAxisDuration(bottomAxisDuration);

	//dialog에 그림을 만들라고 준다. 
	if(dlg.DoModal() == IDOK)
	{
		//만들어진 그림 리스트를 받아온다.
		int count = dlg.m_listImgParam.GetCount();
		POSITION pos = dlg.m_listImgParam.GetHeadPosition();
		for(int i=0; i<count; i++)
		{
			pList->AddTail(dlg.m_listImgParam.GetNext(pos));
		}
		//chartNum을 업데이트한다.
		chartNum += count;
	}


	return chartNum;
}

bool ED_rptMaker::addImageToReport(printImgParam imgParam)
{
	bool bRtn = false;

	//check page
	if(m_dCurrentLine + PRINT_CHART_HEIGHT + TEXT2_HEIGHT> A4_PAGE_HEIGHT)
		addPage();

	addText(imgParam.text, 2);
	


	// set chart image file name
	CString fullname;
	fullname.Format("%s\\%s", this->m_strPrintTempFolder, imgParam.fname);
	fullname.Replace(":", "\\colon");
	
	CString text = "report_picture:0.3,TOP ,7.5,BOTTOM,FILE_NAME,PICTURE_ID,0,1,0.000000,0,0,FULL_PATH;\n";	

	//file name
	text.Replace("FILE_NAME", imgParam.fname);
	text.Replace("FULL_PATH", fullname);

	//top
	CString strTop;
	strTop.Format("%f", this->m_dCurrentLine);
	text.Replace("TOP", strTop);
	
	//bottom
	strTop.Format("%f", m_dCurrentLine + PRINT_CHART_HEIGHT);
	text.Replace("BOTTOM", strTop);

	
	CString strID;
	text.Replace("PICTURE_ID", imgParam.fname);


	if(m_file.m_hFile != CFile::hFileNull)
	{
		m_file.Write(text.GetBuffer(), text.GetLength());
		bRtn = true;
	}

	// text의 bottom 확인
	this->m_dCurrentLine += PRINT_CHART_HEIGHT;

	return bRtn;

}