
// EyeRestDlg.h : header file
//

#pragma once
#include"constraints.h"
#include"Input.h"

// CEyeRestDlg dialog
class CEyeRestDlg : public CDialogEx
{
// Construction
public:
	CEyeRestDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EYEREST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	bool isRestTime = false;
	Input mInput;
	static const UINT_PTR TIMER_WORK_ID = 1111;
	static const UINT_PTR TIMER_REST_ID = 1112;

	// Durations in milliseconds (20 mins work, 1 sec tick for countdown)
	 double WORK_DURATION;// = m_constraints.WorkTime;//m_constraints.w;//5000;//20 * 60 * 1000;
	 double REST_TICK; //= 1000;
	 double REST_DURATION;

	int m_nSecondsLeft; // Tracks the 20-second countdown
	void UpdateTimerText();
	void PositionWindowTopRight();
	void update_time_data();
	//
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg LRESULT OnStartInitialTimer(WPARAM wParam, LPARAM lParam);
	// update it to reflect remaining resting time
	CStatic mRestTimer;
	CStatic mWorkedTime;
	Constraints m_constraints;
public:
	afx_msg void OnBnClickedButtonSee();
	afx_msg void OnClickedButtonSet();
};


