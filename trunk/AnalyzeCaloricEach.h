#pragma once


// CAnalyzeCaloricEach ��ȭ �����Դϴ�.

class CAnalyzeCaloricEach : public CDialog
{
	DECLARE_DYNAMIC(CAnalyzeCaloricEach)

public:
	CAnalyzeCaloricEach(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnalyzeCaloricEach();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ANALYZE_CALORIC_EACH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
