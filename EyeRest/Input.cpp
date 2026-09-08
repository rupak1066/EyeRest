// Input.cpp : implementation file
//

#include "pch.h"
#include "EyeRest.h"
#include "Input.h"
#include "afxdialogex.h"


// Input dialog

IMPLEMENT_DYNAMIC(Input, CDialogEx)

Input::Input(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INPUT, pParent)
	, workDurationIN(_T(""))
	, restDurationIn(_T(""))
{

}

Input::~Input()
{
}

void Input::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WORK_DURATION, workDurationIN);
	DDX_Text(pDX, IDC_EDIT_REST_DURATION, restDurationIn);
}


BEGIN_MESSAGE_MAP(Input, CDialogEx)
	
END_MESSAGE_MAP()

bool Input::takeInput(Constraints& mConstraint)
{
	if (workDurationIN.IsEmpty() && restDurationIn.IsEmpty())
		return false;

	if (!workDurationIN.IsEmpty())
		mConstraint.WorkTime = _tstof(workDurationIN);

	if(!restDurationIn.IsEmpty())
		mConstraint.RestTime = _tstof(restDurationIn);

	return true;
}
// Input message handlers

