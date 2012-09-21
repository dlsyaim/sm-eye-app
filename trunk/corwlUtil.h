#include ".\tchart\tchart.h"


//#define DEL(x){if(x) {delete []x; x = NULL;}}

void corGetProgramFolder(CString* pStr);
bool corFolderExist(CString folderName);
void corCreateDirectory(CString strFolderName);
void corDeleteUnfilenameCharacter(CString* pStr, CString strReplaceChar);
void corDeleteAllFilesInFolder(CString strFolder);

CString setCurrentWorkPath(CString strPath);

//tchart 관련 함수들
unsigned long corGetChartValue(CTChart* pChart, unsigned int seriesIdx, bool bX, double* pBuf);
void corSetChartValue(CTChart* pChart, unsigned int seriesIdx, double* pBuf, unsigned long count, double dX);

//sort
void corSort(double* pData, int count);


//statistics
double corMedian(double* pDat, long n);
double corMean(double* pDat, long n);
double corFindMaximum(double* pDat, long n);
double corFindMinimum(double* pDat, long n);

double corFindMaximum(double* pDat, unsigned long startIdx, unsigned long endIdx, unsigned long* pMaxIdx);
double corFindMinimum(double* pDat, unsigned long startIdx, unsigned long endIdx, unsigned long* pMinIdx);

//zero cross
unsigned long corFindZeroCrossIdx(double* pDat, unsigned long startIdx, unsigned long count, char direct);


void corLocateWindow(CWnd* pWnd, double left, double top, double width, double height, CRect winRect);


void corAdd2List(CListCtrl* pList, CString strParam, double* pValue, int count);
void corChangeListValue(CListCtrl* pList, int nItem, double* pValue, int count);


short get2sComplement(unsigned short);


// folder 선택
bool corwlSelectFolder(CString* pStrFolderName, CWnd* pWnd, CString strRoot);


//report generator관련
void insertString2Report(CString* pStr, double value, CString ID);

//tchart export
void exportTChart(CTChart* pChart, CString fname);