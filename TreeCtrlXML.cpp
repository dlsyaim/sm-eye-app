// TreeCtrlXML.cpp : ���� �����Դϴ�.
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	
}


BEGIN_MESSAGE_MAP(CTreeCtrlXML, CTreeCtrl)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CTreeCtrlXML �޽��� ó�����Դϴ�.


bool CTreeCtrlXML::LoadFromXML( const CString& a_strFileXML, CString strFileTI)
{
	this->clear();

	TiXmlNode* pXML = NULL;
	TiXmlDocument xmlDoc;

	if( xmlDoc.LoadFile( a_strFileXML) )
	{
		//test info file�� �ִ��� �˻�
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
	//bForTestList�� true��� 
	// test list�� �����ϱ� ���� ȣ��

	// false��� �����͸� �����Ҷ� ȣ���Ѵ�.

	// Save XML
	TiXmlNode* pXML = NULL;
	TiXmlDocument xmlDoc;

	// XML header
	xmlDoc.InsertEndChild( TiXmlDeclaration( "1.0", "UTF-8", "yes" ) );

	// XML root
	pXML = xmlDoc.InsertEndChild( TiXmlElement( "XML" ) );

	//test info file�� �ִ��� �˻�
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
		pElement->SetAttribute( "TIExist", GetItemData(hItem) ? "1" : "0");	//test info�� �ִ���
		
		//test info�� ������ �����Ѵ�.
		if(GetItemData(hItem) && pFile && (pFile->m_hFile))
		{
			char buf[TI_SIZE];
			::memset(buf, 0, TI_SIZE);
			
			ED_TestInfo ti;
			ED_TestInfo* pTI = NULL;
			if(bForTestList)
			{
				//test list�� ���� ���̹Ƿ� TI�� ������ �� TI�� �ʿ���� �κ��� �����Ѵ�.
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

	//Test Info�� �ʿ��� node���
	if(CString(pEl->Attribute("TIExist")) == "1") 
	{
		ED_TestInfo* pTI = new ED_TestInfo;
		if(pFile)	//test info ������ �ִٸ�
		{			
			//�д´�.
			char buf[TI_SIZE];
			if(pFile->Read(buf, TI_SIZE) == TI_SIZE)
			{
				::memcpy(pTI, buf, sizeof(ED_TestInfo));	
				if(pTI->m_lEyeDataCount)
				{
					//pTest�� ���� item�� bold��
					this->SetItemState(hItem, TVIS_BOLD, TVIS_BOLD);
					//parent�� bold��
					this->SetItemState(a_hTreeParent, TVIS_BOLD, TVIS_BOLD);

					
				}
			}

		
			
			else
			{
				//������ ����� ���� �ʴٸ� �̻��ѵ�...
			}
		}
		//test file�� ���ٸ� default�� �׳� ����Ѵ�.

		//tree item�� test info�� ���´�.
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

	// child item�� ���ٸ�
	if( false == ItemHasChildren( hItem ) )
	{		
		
		
		while( ( hItemRet = GetNextSiblingItem( hItem ) ) == NULL )
		{
			// ������ ���ٸ� �θ� ����
			if( ( hItem = GetParentItem( hItem ) ) == NULL )
				return NULL;		// �θ� ���ٸ� NULL
		}
		
	}
	else
	{
		// child item�� �ִٸ� ����
		return GetChildItem( hItem );
	}

	// ���� item�� �ִٸ� ������ return
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
	//test data�� ���� leaf�� �����.
	// bold�� �ƴ� ���� ����� �ȴ�.
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
	//m_ucPrint �� true�� �͸� check�Ѵ�.
	if(pTI && (CString(pTI->m_chFileName) != "") && (check == PRINT_CHECK) && (pTI->m_ucPrint))	
		SetCheck(hItem);
	else if (pTI && (CString(pTI->m_chFileName) != "") && (check == PRINT_CHECK_ALL))
	{
		SetCheck(hItem);	//check all�̸� ti�� �ִ� ���� �� üũ�Ѵ�.
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
	//������ �����͸� �����.
	HTREEITEM hItem = GetRootItem();
	this->deleteAllItemData(hItem);


	//tree�� leaf�� �����.
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
		//children�� ������ child ���� �����.
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
		//test info�� ������ ti�� ���� �����.
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
	//test data�� ���� leaf�� �����.
	// bold�� �ƴ� ���� ����� �ȴ�.
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
		//children�� ������ child ���� �����.
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
		//test info�� ������ ti�� ���� �����.
		if(GetItemData(hItem))
		{
			ED_TestInfo* pTI = (ED_TestInfo*)GetItemData(hItem);
			CFileStatus fs;
			if((pTI) && 
				(pTI->m_stiModeCommand == command) &&				//command�� �´���
				pTI->m_chFileName[0] &&			//������ �����ϴ���
				(pTI->m_ucPrint))
			{
				if(hv == HORIZONTAL_VERTICAL)		//�������
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
