// SMEyeApp.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CSMEyeAppApp:
// �� Ŭ������ ������ ���ؼ��� SMEyeApp.cpp�� �����Ͻʽÿ�.
//

class CSMEyeAppApp : public CWinApp
{
public:
	CSMEyeAppApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CSMEyeAppApp theApp;
