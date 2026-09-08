#pragma once

#include"constraints.h"
#include <tchar.h>
// Input dialog

class Input : public CDialogEx
{
	DECLARE_DYNAMIC(Input)

public:
	Input(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Input();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	bool takeInput(Constraints &mConstraint);
	CString workDurationIN;
	CString restDurationIn;
};
