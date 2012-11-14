#include "stdafx.h"
#include "corwlUtil.h"
#include ".\tchart\series.h"
#include ".\tchart\valuelist.h"
#include ".\tchart\axes.h"
#include ".\tchart\axis.h"
#include ".\tchart\export.h"
#include ".\tchart\bmpexport.h"
#include ".\tchart\panel.h"
#include <algorithm>




void corGetProgramFolder(CString* pStr)
{
	char szModulePath[256] = "";
	GetModuleFileName(GetModuleHandle(NULL), szModulePath, 256);
	*pStr = szModulePath;

	CString sFullPath;
	sFullPath.Format("%s", szModulePath);
	//�������� �̸� ����
	*pStr = sFullPath.Left(sFullPath.ReverseFind('\\'));        //�������� \�� ����.

}



bool corFolderExist(CString folderName)
{
    DWORD ret = GetFileAttributes(folderName);
    if(ret == FILE_ATTRIBUTE_DIRECTORY)
        return true;
    return false;
}

void corCreateDirectory(CString strFolderName)
 {
	if(strFolderName.GetAt(strFolderName.GetLength()-1) == CString("\\"))
	strFolderName.Delete(strFolderName.GetLength()-1,1);

	if(!corFolderExist(strFolderName))
	{
		int index = strFolderName.ReverseFind('\\');
		if(index != -1)	//root�� �ƴ϶��
		{
			CString strParentFolder = strFolderName.Left(index);

			if(!corFolderExist(strParentFolder))
			corCreateDirectory(strParentFolder);
			
			::CreateDirectory(strFolderName, NULL);
		}
            
	}
}

 
void corDeleteUnfilenameCharacter(CString* pStr, CString strReplaceChar)
{
	//�����̸����� ������ �ʴ� ���ڸ� �����Ѵ�.
	pStr->Replace("\\", strReplaceChar);
	pStr->Replace("/", strReplaceChar);
	pStr->Replace("?", strReplaceChar);
	pStr->Replace(":", strReplaceChar);
	pStr->Replace("\"", strReplaceChar);
	pStr->Replace("*", strReplaceChar);
	pStr->Replace("<", strReplaceChar);
	pStr->Replace(">", strReplaceChar);
	pStr->Replace("|", strReplaceChar);
}

void corDeleteAllFilesInFolder(CString strFolderName)
{
	if(strFolderName.GetAt(strFolderName.GetLength()-1) != CString("\\"))
	strFolderName += "\\";

    CFileFind FileFinder;
    strFolderName = strFolderName + _T("*.*");
    BOOL bWorking = FileFinder.FindFile(strFolderName); 

    while ( (bWorking) ) 
    {
        bWorking = FileFinder.FindNextFile(); 

        if ( FileFinder.IsDots() ) continue; 

        if ( FileFinder.IsDirectory() )
        {

            // ������������ ó���Ϸ��� ���⼭ ���� �ؾ߰���?
        }
        else
        { 
            CString fname = FileFinder.GetFilePath();
			CFile::Remove(fname);
        }
    }

}


CString setCurrentWorkPath(CString strPath)
{
	CString strPrevPath;

	char work_path[MAX_PATH];
	char prev_path[MAX_PATH];
	memset(work_path, 0, MAX_PATH);
	memset(prev_path, 0, MAX_PATH);

	GetCurrentDirectory(MAX_PATH, prev_path);
	strPrevPath.Format("%s", prev_path);

	memcpy(work_path, strPath, strPath.GetLength());

	SetCurrentDirectory(work_path);

	return strPrevPath;
}
unsigned long corGetChartValue(CTChart* pChart, unsigned int seriesIdx, bool bX, double* pBuf)
{
	//bx�� true�� x value��, false�� yvalue��
	CSeries series = pChart->Series(seriesIdx);
	unsigned long count = series.GetCount();

	
	CValueList valueList = bX ? series.GetXValues() : series.GetYValues();
	for(unsigned long i=0; i< count; i++)
		pBuf[i] = valueList.GetValue(i);

	return count;
	
}

void corSetChartValue(CTChart* pChart, unsigned int seriesIdx, double* pBuf, unsigned long count, double dX)
{
//	if(pChart->GetSeriesCount() <= seriesIdx)
//		;

	CSeries series = pChart->Series(seriesIdx);
	for(unsigned long i=0; i<count; i++)
		series.AddXY(i*dX, pBuf[i], 0, NULL);

	//set axis range
	if(series.GetYValues().GetMaximum() > pChart->GetAxis().GetLeft().GetMaximum())
		pChart->GetAxis().GetLeft().SetMaximum(series.GetYValues().GetMaximum());
	if(series.GetYValues().GetMinimum() < pChart->GetAxis().GetLeft().GetMinimum())
		pChart->GetAxis().GetLeft().SetMinimum(series.GetYValues().GetMinimum());
}


double corMean(double* pDat, long n)
{
	if(n <1)
		return 0;

	double s = 0;
	for(int i=0; i<n; i++)
		s += pDat[i];
	return s/n;
}

double corMedian(double* pDat, long n)
{
	CArray<double> tempArray;
	tempArray.SetSize(n);
	for(int i=0; i<n; i++)
		tempArray.SetAt(i, pDat[i]);

	//sort
	std::sort(tempArray.GetData(), tempArray.GetData() + tempArray.GetSize());

	return tempArray.GetAt(n>>1);

}


/*
void shellSort(double* pDat, long n);
double corMedian(double* pDat, long n)
{
	double* pTemp = new double[n];
	::memcpy(pTemp, pDat, sizeof(double)*n);
	shellSort(pTemp, n);
	double m = pTemp[n>>1];
	delete []pTemp;
	return m;
	
}

void shellSort(double* pDat, long n)
{

	//Sorts an array a[1..n] into ascending numerical order by Shell's method (diminishing increment
	//sort). n is input; a is replaced on output by its sorted rearrangement.

	//1 based index algorithm.
	//pDat[0]�� ��������.
	n = n-1;

	long i,j,inc;
	float v;
	inc=1; //Determine the starting increment.
	do 
	{
		inc *= 3;
		inc++;
	} while (inc <= n);
	do 
	{ //Loop over the partial sorts.
		inc /= 3;
		for (i=inc+1;i<=n;i++) //Outer loop of straight insertion.
		{ 
			v=float(pDat[i]);
			j=i;
			while (pDat[j-inc] > v) //
			{ 
				pDat[j]=pDat[j-inc];
				j -= inc;
				if (j <= inc) break;
			}
			pDat[j]=v;
		}
	} while (inc > 1);

}
*/

double corFindMaximum(double* pDat, long n)
{
	if(n<1)
		return 0;

	double m = -100000;
	for(int i=0; i<n; i++)
	{
		if(m < pDat[i] )
			m = pDat[i];
	}
	return m;
}

double corFindMaximum(double* pDat, unsigned long startIdx, unsigned long endIdx, unsigned long* pMaxIdx)
{
	double m = -1000000;
	for(unsigned long i=startIdx; i<=endIdx; i++)
	{
		if( m < pDat[i])
		{
			m = pDat[i];
			if(pMaxIdx)
                *pMaxIdx = i;
		}
	}
	return m;
}

double corFindMinimum(double* pDat, long n)
{
	if(n<1)
		return 0;

	double m = 100000;
	for(int i=0; i<n; i++)
	{
		if(pDat[i] < m )
			m = pDat[i];
	}
	return m;
}



double corFindMinimum(double* pDat, unsigned long startIdx, unsigned long endIdx, unsigned long* pMinIdx)
{
	double m = 1000000;
	for(unsigned long i=startIdx; i<=endIdx; i++)
	{
		if( m > pDat[i])
		{
			m = pDat[i];
			if(pMinIdx)
                *pMinIdx = i;
		}
	}
	return m;
}

void corLocateWindow(CWnd* pWnd, double left, double top, double width, double height, CRect winRect)
{
	//left, top, width, height�� 0~1�� ����
	pWnd->MoveWindow(int(winRect.Width()*left), int(winRect.Height()*top), int(winRect.Width()*width), int(winRect.Height()*height));
}

void corAdd2List(CListCtrl* pList, CString strParam, double* pValue, int count)
{
	CString str;
	
	int nIdx = pList->InsertItem(0, strParam);

	for(int i=0; i<count; i++)
	{
		if(pValue[i] == -1.1111111) // �� ���� ������ �׳� ����д�.
			str = "-";
		else if(strParam == "count")
			str.Format("%.0f", pValue[i]);
		else
			str.Format("%.2f", pValue[i]);
		pList->SetItemText(nIdx, i+1, str);
	}
}

void corChangeListValue(CListCtrl* pList, int nItem, double* pValue, int count)
{
	if(!pList || !pList->GetSafeHwnd())
		return;
	if(nItem >= pList->GetItemCount())
		return;

	CString str;
	for(int i=0; i<count; i++)
	{
		str.Format("%.2f", pValue[i]);
		pList->SetItemText(nItem, i+1, str);
	}
}

unsigned long corFindZeroCrossIdx(double* pDat, unsigned long startIdx, unsigned long count, char direct)
{
	unsigned long idx = -1;
	if(direct > 0)
	{
		for(unsigned long i=startIdx; i<count-1; i++)
		{
			if(pDat[i]*pDat[i+1] <= 0)
			{
				idx = i;
				break;
			}
		}
	}
	else
	{
		for(unsigned long i=startIdx; i>0; i--)
		{
			if(pDat[i]*pDat[i-1] <= 0)
			{
				idx = i;
				break;
			}
		}
	}

	return idx;
}


short get2sComplement(unsigned short ts)
{
  short s = 0;
  if(ts & (1<<15))
  {
    s = -1*(~(ts-1));
  }
  else
    s = ts;
  
  return s;  
}



static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
bool corwlSelectFolder(CString* pStrFolderName, CWnd* pWnd, CString strRoot)
{
	bool bRtn = false;

	if(!pWnd || !pWnd->GetSafeHwnd())
		return false;

	LPITEMIDLIST pildBrowse;

	char pszPathname[MAX_PATH] = "";

	BROWSEINFO bInfo;
	memset(&bInfo, 0, sizeof(bInfo));

	bInfo.hwndOwner = pWnd->GetSafeHwnd();
	bInfo.pidlRoot = NULL;
	bInfo.pszDisplayName = pszPathname;
	bInfo.lpszTitle = "Choose Data folder";
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_VALIDATE | BIF_USENEWUI;
	bInfo.lpfn = BrowseCallbackProc;
	bInfo.lParam = (LPARAM)strRoot.GetBuffer(strRoot.GetLength());
	

	pildBrowse = ::SHBrowseForFolder(&bInfo);
	if(pildBrowse != NULL)
	{
		::SHGetPathFromIDList(pildBrowse, pszPathname);
		pStrFolderName->Format("%s", pszPathname);
		bRtn = true;
	}



	return bRtn;
}

static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch(uMsg)
	{
		case BFFM_INITIALIZED:
		{
			SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpData);
		}
		break;
	}
	return 0;
}


void corSort(double* pData, int count)
{
	if(count <= 0)
		return;

	if(!pData)
		return;

	CArray<double> tempArray;
	tempArray.SetSize(count);
	for(unsigned long i=0; i<count; i++)
	{
		tempArray.SetAt(i, pData[i]);
	}

	//sort
	std::sort(tempArray.GetData(), tempArray.GetData() + tempArray.GetSize());

	for(int i=0; i<count; i++)
		pData[i] = tempArray.GetAt(i);

	tempArray.RemoveAll();
}


void insertString2Report(CString* pStr, double value, CString ID)
{
	if(!pStr)
		return;

	CString str;
	str.Format("%.2f", value);
	pStr->Replace(ID, str);
}

void exportTChart(CTChart* pChart, CString fname)
{
	unsigned long color = pChart->GetPanel().GetColor();
	CRect rect;
	pChart->GetWindowRect(&rect);
	pChart->GetPanel().SetColor(RGB(255, 255, 255));
	pChart->GetExport().GetAsBMP().SetWidth(rect.Width());
	pChart->GetExport().GetAsBMP().SetHeight(rect.Height());
	pChart->GetExport().GetAsBMP().SaveToFile(fname);
	pChart->GetPanel().SetColor(color);

}
