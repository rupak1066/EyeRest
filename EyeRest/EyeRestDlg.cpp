
// EyeRestDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "EyeRest.h"
#include "EyeRestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEyeRestDlg dialog



CEyeRestDlg::CEyeRestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EYEREST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEyeRestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_text_worked, mWorkedTime);
	DDX_Control(pDX, IDC_CIRCULAR_PROGRESS, progress);
}

BEGIN_MESSAGE_MAP(CEyeRestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER + 100, &CEyeRestDlg::OnStartInitialTimer)
	ON_BN_CLICKED(IDC_BUTTON_SEE, &CEyeRestDlg::OnBnClickedButtonSee)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CEyeRestDlg::OnClickedButtonSet)
	
END_MESSAGE_MAP()


// CEyeRestDlg message handlers

BOOL CEyeRestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	 // 1. Remove the taskbar window style and add the tool window style
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	// 2. Force the window manager to redraw the window frame with the new styles
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	//set the button texture
	// 1. Get the button pointer
	CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON_SEE);
	CButton* pButton2 = (CButton*)GetDlgItem(IDC_BUTTON_SET);
	// 2. Load the icon using the global AfxGetApp()
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_SEE_SCREEN);
	HICON hIcon2 = AfxGetApp()->LoadIcon(IDI_ICON_SETTING);
	
	// 3. Apply the icon to the button
	if (pButton && hIcon)
	{
		pButton->SetIcon(hIcon);
	}
	if (pButton2 && hIcon2)
	{
		pButton2->SetIcon(hIcon2);
	}


	//SetTimer(TIMER_WORK_ID, WORK_DURATION, NULL);
	PostMessage(WM_USER + 100, 0, 0);
	ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
LRESULT CEyeRestDlg::OnStartInitialTimer(WPARAM wParam, LPARAM lParam)
{
	// The window is fully loaded now. Safe to start the timer loop!
	m_nSecondsLeft = m_constraints.RestTime;
	WORK_DURATION = m_constraints.WorkTime * 60 * 1000;
	REST_DURATION = m_constraints.RestTime * 1000;
	REST_TICK = m_constraints.TickEvery * 1000;
	SetTimer(TIMER_WORK_ID, WORK_DURATION, NULL);
	return 0;
}
void CEyeRestDlg::update_time_data()
{
	m_nSecondsLeft = m_constraints.RestTime;
	WORK_DURATION = m_constraints.WorkTime * 60 * 1000;
	REST_DURATION = m_constraints.RestTime * 1000;
}
void CEyeRestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEyeRestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CEyeRestDlg::UpdateTimerText()
{
	CString str;
	str.Format(_T("%dS"), m_nSecondsLeft);
	
	
	progress.setIntervals(min(m_constraints.RestTime,60.0));
	progress.setMinMax(0.0, m_constraints.RestTime);
	progress.setText(str);
	progress.setpos(m_constraints.RestTime - m_nSecondsLeft);

	str.Format(_T("You Worked For %.2f Minutes"), m_constraints.WorkTime);
	mWorkedTime.SetWindowTextW(str);
	
}
void CEyeRestDlg::PositionWindowTopRight()
{
	
	CRect rect;
	GetWindowRect(rect);
	int scrn_width = GetSystemMetrics(SM_CXSCREEN);
	int width = rect.Width();
	int height = rect.Height();
	int x = scrn_width - width - 20;
	int y = 20;
	SetWindowPos(&wndTopMost, x, y, width, height, SWP_SHOWWINDOW);
}


void CEyeRestDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanging(lpwndpos);

	// TODO: Add your message handler code here
	// If we are still in the 20-minute work period, force it to stay hidden
	if (!isRestTime && GetSafeHwnd() && !IsWindowVisible())
	{
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
	}
	CDialogEx::OnWindowPosChanging(lpwndpos);
}


void CEyeRestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TIMER_WORK_ID:
		// Try standard MFC KillTimer first
		KillTimer(TIMER_WORK_ID);
		isRestTime = true;
		PositionWindowTopRight();
		m_nSecondsLeft = m_constraints.RestTime;
		UpdateTimerText();
		SetTimer(TIMER_REST_ID, REST_TICK, NULL);
		break;
	case TIMER_REST_ID:
		m_nSecondsLeft--;
		if (m_nSecondsLeft >= 0)
			UpdateTimerText();
		else
		{
			// Try standard MFC KillTimer first
			KillTimer(TIMER_REST_ID);
			ShowWindow(SW_HIDE);
			SetTimer(TIMER_WORK_ID, WORK_DURATION, NULL); \
			isRestTime = false;
		}
	default:
		break;
	}
	
	CDialogEx::OnTimer(nIDEvent);

}


void CEyeRestDlg::OnBnClickedButtonSee()
{
	// TODO: Add your control notification handler code here
	//forcefully finish the rest period
	KillTimer(TIMER_REST_ID);
	ShowWindow(SW_HIDE);
	SetTimer(TIMER_WORK_ID, WORK_DURATION, NULL);
	isRestTime = false;
	m_nSecondsLeft = m_constraints.RestTime;
}


void CEyeRestDlg::OnClickedButtonSet()
{
	
	INT_PTR nResponse = mInput.DoModal();
	if (nResponse == IDOK)
	{
		UpdateData(TRUE);
		mInput.takeInput(m_constraints);
		update_time_data();
	}
	else if (nResponse == IDCANCEL)
	{
		// User clicked Cancel or closed the X button
	}
	// TODO: Add your control notification handler code here
}


