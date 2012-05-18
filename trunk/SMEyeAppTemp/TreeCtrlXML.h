#include "stdafx.h"
#include "TinyXML/tinyxml.h"
#include "ed_testinfo.h"
#include "corwlDefines.h"

#pragma once


#define PRINT_CHECK_ALL 1
#define PRINT_CHECK_NONE -1
#define PRINT_CHECK 0

// CTreeCtrlXML

class CTreeCtrlXML : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlXML)

public:
	CTreeCtrlXML();
	virtual ~CTreeCtrlXML();

protected:
	DECLARE_MESSAGE_MAP()


public:

	bool LoadFromXML( const CString& a_strFileXML, CString strFileTI = "");
	bool SaveToXML( const CString& a_strFile, CString strFileTI = "",  bool bForTestList = false);

	ED_TestInfo* getCurrentTestInfo();
	ED_TestInfo* getTestInfoFromItem(HTREEITEM hItem);
	void getCurrentItemGeneology(CString* pStr);
	CString getDefaultXMLName();
	CString getDefaultTIName();

	HTREEITEM addNewGroup(CString str);

	void deletePlainItem();	//printform 으로 나타낼 때 bold가 아닌 것은 제거한다. 
	void setPrintCheck(int check = PRINT_CHECK);

	void findTests(CList<ED_TestInfo*, ED_TestInfo*>* pList, unsigned char command, char hv = HORIZONTAL_VERTICAL);

protected:
	CString m_strDefaultXMLName;
	CString m_strDefaultTIName;

	void Load( TiXmlNode* a_pNode, CFile* pFile = NULL);
	void Save( TiXmlNode* a_pNode, CFile* pFile = NULL,  bool bForTestList = false);


	void LoadItem( TiXmlNode* a_pNode, HTREEITEM a_hTreeParent, CFile* pFile = NULL);
	void deleteAllItemData(HTREEITEM hItem);
	void deleteLeaf(HTREEITEM hItem);


	void deletePlainLeaf(HTREEITEM hItem);
	void setPrintCheckLeaf(HTREEITEM hItem, int check);
	

	int GetIndentLevel( HTREEITEM hItem );

	

public:
	HTREEITEM GetNextChildItem( HTREEITEM hItem );
	afx_msg void OnDestroy();
	void clear();
	
	void deleteGroup(HTREEITEM hItem);
	void addNewTest(HTREEITEM hItem , CString strTestName, DWORD_PTR pTI);
};


