// TreeCtrlXML.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "TreeCtrlXML.h"
#include <direct.h>		// Needed for getcwd()
#include ".\treectrlxml.h"
#include "corwlDefines.h"


// CTreeCtrlXML

IMPLEMENT_DYNAMIC(CTreeCtrlXML, CTreeCtrl)
CTreeCtrlXML::CTreeCtrlXML()
{
	m_strDefaultXMLName = TEST_LIST_FILENAME;
	m_strDefaultTIName = TEST_INFO_FILENAME;
}

CTreeCtrlXML::~CTreeCtrlXML()
{
	
}

void CTreeCtrlXML::OnDestroy()
{
	HTREEITEM hItem = GetRootItem();
	this->deleteAllItemData(hItem);	

	CTreeCtrl::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
}


BEGIN_MESSAGE_MAP(CTreeCtrlXML, CTreeCtrl)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CTreeCtrlXML 메시지 처리기입니다.


bool CTreeCtrlXML::LoadFromXML( const CString& a_strFileXML, CString strFileTI)
{
	this->clear();

	TiXmlNode* pXML = NULL;
	TiXmlDocument xmlDoc;

	if( xmlDoc.LoadFile( a_strFileXML) )
	{
		//test info file이 있는지 검사
		bool bTIExist = false;
		CFile file;
		if(file.Open(strFileTI, CFile::modeRead))
			bTIExist = true;

		// XML root
		pXML = xmlDoc.FirstChild( "XML" );

		if( NULL == pXML )
		{
			if(file.m_hFile != CFile::hFileNull)
				file.Close();
			return false;
		}

		// Load our tree control
		Load( pXML, bTIExist ? &file : NULL);

		// Expand all entries
		Expand( GetRootItem(), TVE_EXPAND );

		if(file.m_hFile != CFile::hFileNull)
				file.Close();
			//file.Close();

		return true;
	}
    
	return false;
}


bool CTreeCtrlXML::SaveToXML( const CString& a_strFileXML, CString strFileTI, bool bForTestList)
{
	//bForTestList가 true라면 
	// test list를 저장하기 위한 호출

	// false라면 데이터를 저장할때 호출한다.

	// Save XML
	TiXmlNode* pXML = NULL;
	TiXmlDocument xmlDoc;

	// XML header
	xmlDoc.InsertEndChild( TiXmlDeclaration( "1.0", "UTF-8", "yes" ) );

	// XML root
	pXML = xmlDoc.InsertEndChild( TiXmlElement( "XML" ) );

	//test info file이 있는지 검사
	bool bTIExist = false;
	CFile file;
	if(file.Open(strFileTI, CFile::modeCreate | CFile::modeWrite))
		bTIExist = true;

	// Save our tree control
	Save( pXML, bTIExist ? &file : NULL, bForTestList);

	// Save XML
	CString strFile = a_strFileXML;

	if(bTIExist)
		file.Close();

	return xmlDoc.SaveFile( strFile.GetBuffer( 1 ) );
}

void CTreeCtrlXML::Load( TiXmlNode* a_pNode, CFile* pFile)
{
	ASSERT( NULL != a_pNode );

	// Get node "Items"
	TiXmlNode* pItems = a_pNode->FirstChild( "Items" );
	TiXmlNode* pItem = NULL;

	if( NULL == pItems )
		return;

	// Get first item
	pItem = pItems->FirstChild( "Item" );

	// Iterate all siblings
	while( NULL != pItem )
	{
		LoadItem( pItem, NULL, pFile);
		pItem = pItem->NextSibling( "Item" );
	}
}



void CTreeCtrlXML::Save( TiXmlNode* a_pNode, CFile* pFile,  bool bForTestList )
{
	ASSERT( NULL != a_pNode );

	TiXmlNode* pItems = a_pNode->InsertEndChild( TiXmlElement( "Items" ) );
	TiXmlNode* pParent = pItems;
	TiXmlNode* pNewNode = NULL;
	HTREEITEM hItem = GetRootItem();
	int iIndent = 0; int iLastIndent = 0;

	while( hItem )
	{
		iIndent = GetIndentLevel( hItem );

		int iDiff = iIndent - iLastIndent;
		if( iDiff > 0 )
		{
			ASSERT( NULL != pNewNode );

			while( iDiff-- )
				pNewNode = pNewNode->InsertEndChild( TiXmlElement( "Item" ) );

			ASSERT( NULL != pNewNode );
			pParent = pNewNode->Parent();
		}
		else if( iDiff < 0 )
		{
			iDiff--;					// Correct difference to get the right parent
			pParent = pNewNode;

			while( iDiff++ < 0 )
				pParent = pParent->Parent();

			ASSERT( NULL != pParent );
			pNewNode = pParent->InsertEndChild( TiXmlElement( "Item" ) );
		}
		else 
		{
			ASSERT( NULL != pParent );
			pNewNode = pParent->InsertEndChild( TiXmlElement( "Item" ) );
		}

		iLastIndent = iIndent;			

		// Save current item
		TiXmlElement* pElement = pNewNode->ToElement();
		ASSERT( NULL != pElement );

		TiXmlElement* pSrcElement = a_pNode->ToElement();

		pElement->SetValue( "Item" );
		pElement->SetAttribute( "Title", GetItemText( hItem ) );
		pElement->SetAttribute( "TIExist", GetItemData(hItem) ? "1" : "0");	//test info가 있는지
		
		//test info가 있으면 저장한다.
		if(GetItemData(hItem) && pFile && (pFile->m_hFile))
		{
			char buf[TI_SIZE];
			::memset(buf, 0, TI_SIZE);
			
			ED_TestInfo ti;
			ED_TestInfo* pTI = NULL;
			if(bForTestList)
			{
				//test list만 만들 것이므로 TI를 복사한 후 TI의 필요없는 부분을 변경한다.
				::memcpy(&ti, (const void*)(GetItemData(hItem)), sizeof(ED_TestInfo));
				ti.m_lEyeDataCount = 0;
				ti.m_lAccDataCount = 0;
				::ZeroMemory(ti.m_chFileName, MAX_FILE_NAME);

				pTI = &ti;
			}
			else	
				pTI = (ED_TestInfo*)GetItemData(hItem);

			::memcpy(buf, pTI, sizeof(ED_TestInfo));
			pFile->Write(buf, TI_SIZE);
		}

		// The next item, please ...
		hItem = GetNextChildItem( hItem );
	}
}

void CTreeCtrlXML::LoadItem( TiXmlNode* a_pNode, HTREEITEM a_hTreeParent, CFile* pFile)
{
	ASSERT( NULL != a_pNode );

	TiXmlElement* pEl = a_pNode->ToElement();
	ASSERT( NULL != pEl );

	HTREEITEM hItem = NULL;

	TiXmlNode* pChild = NULL;

	if( NULL == a_hTreeParent )
	{
		hItem = InsertItem( pEl->Attribute( "Title" ), TVI_ROOT );
	}
	else
		hItem = InsertItem( pEl->Attribute( "Title" ), a_hTreeParent );

	//Test Info가 필요한 node라면
	if(CString(pEl->Attribute("TIExist")) == "1") 
	{
		ED_TestInfo* pTI = new ED_TestInfo;
		if(pFile)	//test info 파일이 있다면
		{			
			//읽는다.
			char buf[TI_SIZE];
			if(pFile->Read(buf, TI_SIZE) == TI_SIZE)
			{
				::memcpy(pTI, buf, sizeof(ED_TestInfo));	
				if(pTI->m_lEyeDataCount)
				{
					//pTest를 가진 item을 bold로
					this->SetItemState(hItem, TVIS_BOLD, TVIS_BOLD);
					//parent도 bold로
					this->SetItemState(a_hTreeParent, TVIS_BOLD, TVIS_BOLD);

					
				}
			}

		
			
			else
			{
				//데이터 싸이즈가 맞지 않다면 이상한데...
			}
		}
		//test file이 없다면 default를 그냥 사용한다.

		//tree item에 test info를 엮는다.
		this->SetItemData(hItem, (DWORD_PTR)(pTI));

		//this->SetCheck(hItem);
	}
	else
		this->SetItemData(hItem, 0);

	pChild = a_pNode->IterateChildren( "Item", NULL );

	while( pChild )
	{
		LoadItem( pChild, hItem, pFile);
		pChild = a_pNode->IterateChildren( "Item", pChild );
	}
}

int CTreeCtrlXML::GetIndentLevel( HTREEITEM hItem )
{
	int iIndent = 0;

	while( (hItem = GetParentItem( hItem )) != NULL )
		iIndent++;

	return iIndent;
}

HTREEITEM CTreeCtrlXML::GetNextChildItem( HTREEITEM hItem )
{
	HTREEITEM hItemRet = NULL;

	// child item이 없다면
	if( false == ItemHasChildren( hItem ) )
	{		
		
		
		while( ( hItemRet = GetNextSiblingItem( hItem ) ) == NULL )
		{
			// 형제가 없다면 부모를 리턴
			if( ( hItem = GetParentItem( hItem ) ) == NULL )
				return NULL;		// 부모가 없다면 NULL
		}
		
	}
	else
	{
		// child item이 있다면 리턴
		return GetChildItem( hItem );
	}

	// 형제 item이 있다면 형제를 return
	return hItemRet;
}


void CTreeCtrlXML::setPrintCheck(int check)
{
	HTREEITEM hRoot = this->GetRootItem();
	HTREEITEM hNextItem;
	while(hRoot)
	{
		hNextItem = GetNextItem(hRoot, TVGN_NEXT);
		this->setPrintCheckLeaf(hRoot, check);
		hRoot = hNextItem;
	}

}

void CTreeCtrlXML::setPrintCheckLeaf(HTREEITEM hItem, int check)
{
	if(!hItem)
		return;
	//test data가 없는 leaf를 지운다.
	// bold가 아닌 것을 지우면 된다.
	if(this->ItemHasChildren(hItem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = this->GetChildItem(hItem);
		while (hChildItem != NULL)
		{
			hNextItem =	GetNextItem(hChildItem, TVGN_NEXT);
			setPrintCheckLeaf(hChildItem, check);
			hChildItem = hNextItem;
		}


	}

	ED_TestInfo* pTI = (ED_TestInfo*)GetItemData(hItem);
	//m_ucPrint 가 true인 것만 check한다.
	if(pTI && (CString(pTI->m_chFileName) != "") && (check == PRINT_CHECK) && (pTI->m_ucPrint))	
		SetCheck(hItem);
	else if (pTI && (CString(pTI->m_chFileName) != "") && (check == PRINT_CHECK_ALL))
	{
		SetCheck(hItem);	//check all이면 ti가 있는 것은 다 체크한다.
        pTI->m_ucPrint = 1;
	}
	else if (check == PRINT_CHECK_NONE)
	{
		SetCheck(hItem, false);		
		if(pTI)
			pTI->m_ucPrint = 0;
	}

}

void CTreeCtrlXML::deletePlainItem()
{
	
	HTREEITEM hRoot = this->GetRootItem();
	HTREEITEM hNextItem;
	while(hRoot)
	{
		hNextItem = GetNextItem(hRoot, TVGN_NEXT);
		this->deletePlainLeaf(hRoot);
		hRoot = hNextItem;
	}

}



ED_TestInfo* CTreeCtrlXML::getCurrentTestInfo()
{
	HTREEITEM hItem = this->GetSelectedItem();
	return getTestInfoFromItem(hItem);
}

ED_TestInfo* CTreeCtrlXML::getTestInfoFromItem(HTREEITEM hItem)
{
	if(hItem)
		return (ED_TestInfo*)GetItemData(hItem);
	else
		return NULL;
}

void CTreeCtrlXML::getCurrentItemGeneology(CString* pStr)
{
	HTREEITEM hItem = this->GetSelectedItem();

	CString strItemText;
	while(hItem)
	{
		strItemText = "_" + GetItemText(hItem);
		*pStr = strItemText + *pStr;
		hItem = this->GetParentItem(hItem);
	}
	pStr->Delete(0, 1);
}

CString CTreeCtrlXML::getDefaultXMLName()
{
	return this->m_strDefaultXMLName;
}
CString CTreeCtrlXML::getDefaultTIName()
{
	return this->m_strDefaultTIName;
}


void CTreeCtrlXML::clear()
{
	//아이템 데이터를 지운다.
	HTREEITEM hItem = GetRootItem();
	this->deleteAllItemData(hItem);


	//tree의 leaf를 지운다.
	HTREEITEM hRoot = this->GetRootItem();
	HTREEITEM hNextItem;
	while(hRoot)
	{
		hNextItem = GetNextItem(hRoot, TVGN_NEXT);
		deleteLeaf(hRoot);
		hRoot = hNextItem;
	}
	
}
void CTreeCtrlXML::deleteLeaf(HTREEITEM hItem)
{
	if(this->ItemHasChildren(hItem))
	{
		//children이 있으면 child 부터 지운다.
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = this->GetChildItem(hItem);
		while (hChildItem != NULL)
		{
			hNextItem =	GetNextItem(hChildItem, TVGN_NEXT);
			deleteLeaf(hChildItem);
			hChildItem = hNextItem;
		}
	}
	DeleteItem(hItem);
    
}


void CTreeCtrlXML::deleteAllItemData(HTREEITEM hItem)
{
	
	
	while( hItem )
	{
		//test info가 있으면 ti를 먼저 지운다.
		if(GetItemData(hItem))
		{
			ED_TestInfo* pTI = (ED_TestInfo*)GetItemData(hItem);
			if(pTI)
				delete pTI;
			SetItemData(hItem, 0);
		}

		// The next item, please ...
		hItem = GetNextChildItem( hItem );
	}
}

void CTreeCtrlXML::deletePlainLeaf(HTREEITEM hItem)
{
	if(!hItem)
		return;
	//test data가 없는 leaf를 지운다.
	// bold가 아닌 것을 지우면 된다.
	if(this->ItemHasChildren(hItem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = this->GetChildItem(hItem);
		while (hChildItem != NULL)
		{
			hNextItem =	GetNextItem(hChildItem, TVGN_NEXT);
			deletePlainLeaf(hChildItem);
			hChildItem = hNextItem;
		}


	}

	if(!(GetItemState(hItem, TVIS_BOLD) & TVIS_BOLD))
			DeleteItem(hItem);
}


HTREEITEM CTreeCtrlXML::addNewGroup(CString str)
{
	HTREEITEM hItem = this->InsertItem(str, TVI_ROOT);
	SetItemData(hItem, 0);
	return hItem;
}
void CTreeCtrlXML::deleteGroup(HTREEITEM hItem)
{
	if(this->ItemHasChildren(hItem))
	{
		//children이 있으면 child 부터 지운다.
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = this->GetChildItem(hItem);
		while (hChildItem != NULL)
		{
			hNextItem =	GetNextItem(hChildItem, TVGN_NEXT);
			ED_TestInfo* pTI = (ED_TestInfo*)GetItemData(hChildItem);
			delete pTI;				
			DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}
	}
	DeleteItem(hItem);
}

void CTreeCtrlXML::addNewTest(HTREEITEM hItemParent , CString strTestName, DWORD_PTR pTI)
{
	HTREEITEM hItem = this->InsertItem(strTestName, hItemParent);
	this->SetItemData(hItem, pTI);
}


void CTreeCtrlXML::findTests(CList<ED_TestInfo*, ED_TestInfo*>* pList, unsigned char command, char hv)
{
	if(!pList)
		return;

	HTREEITEM hItem = NULL;
	hItem = this->GetRootItem();
	
	while( hItem )
	{
		//test info가 있으면 ti를 먼저 지운다.
		if(GetItemData(hItem))
		{
			ED_TestInfo* pTI = (ED_TestInfo*)GetItemData(hItem);
			CFileStatus fs;
			if((pTI) && 
				(pTI->m_stiModeCommand == command) &&				//command가 맞는지
				pTI->m_chFileName[0] &&			//파일이 존재하는지
				(pTI->m_ucPrint))
			{
				if(hv == HORIZONTAL_VERTICAL)		//상관없이
					pList->AddTail(pTI);
				else if((hv == HORIZONTAL) && (pTI->m_analTarget == HORIZONTAL))
					pList->AddTail(pTI);
				else if((hv == VERTICAL) && (pTI->m_analTarget == VERTICAL))
					pList->AddTail(pTI);
			}
			
		}

		// The next item, please ...
		hItem = GetNextChildItem( hItem );
	}

}
