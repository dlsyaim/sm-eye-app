
// testsetse.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CtestsetseApp:
// �� Ŭ������ ������ ���ؼ��� testsetse.cpp�� �����Ͻʽÿ�.
//

class CtestsetseApp : public CWinApp
{
public:
	CtestsetseApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CtestsetseApp theApp;