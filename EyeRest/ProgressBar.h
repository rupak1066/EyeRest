#pragma once
#include<gdiplus.h>

// ProgressBar

class ProgressBar : public CStatic
{
	DECLARE_DYNAMIC(ProgressBar)

public:
	ProgressBar();
	virtual ~ProgressBar();
	

protected:
	DECLARE_MESSAGE_MAP()
//	virtual void DoDataExchange(CDataExchange* pDX);
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	// Core data state
	float m_minVal{0.0};
	float m_maxVal{20.0};
	float m_currentPos;
	float m_dialLength{ 5.0 };
	CString m_text;
	
	Gdiplus::Pen *m_thinBlackPen;
	Gdiplus::Pen* m_thickBlackPen;
	Gdiplus::Pen* m_trackPen;

	Gdiplus::Color m_trackColor{ Gdiplus::Color(255,255,255)};
	Gdiplus::Color m_blackColor{Gdiplus::Color(0,0,0) };
	
	int intervals{ 20 };
public:
	void setpos(float pos) { this->m_currentPos = pos; Invalidate(true);}
	void setMinMax(float min, float max) { this->m_maxVal = max; this->m_minVal = min;}
	void setIntervals(int n) { this->intervals = n; }
	void setText(CString text) { this->m_text = text; }
};


