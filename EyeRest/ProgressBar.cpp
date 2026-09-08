// ProgressBar.cpp : implementation file
//

#include "pch.h"
#include "EyeRest.h"
#include "ProgressBar.h"


// ProgressBar

IMPLEMENT_DYNAMIC(ProgressBar, CStatic)

ProgressBar::ProgressBar()
{

	m_thinBlackPen = new Gdiplus::Pen(m_blackColor, 1.0);
	m_thickBlackPen = new Gdiplus::Pen(m_blackColor, 3.0);
	m_trackPen = new Gdiplus::Pen(m_trackColor, 3.0);

	m_thinBlackPen->SetStartCap(Gdiplus::LineCapRound);  // Smooth center joint
	m_thinBlackPen->SetEndCap(Gdiplus::LineCapRound);    // Smooth outer tip

	m_thickBlackPen->SetStartCap(Gdiplus::LineCapRound);  // Smooth center joint
	m_thickBlackPen->SetEndCap(Gdiplus::LineCapRound);    // Smooth outer tip
}

ProgressBar::~ProgressBar()
{
}


BEGIN_MESSAGE_MAP(ProgressBar, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// ProgressBar message handlers




//void ProgressBar::DoDataExchange(CDataExchange* pDX)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	CStatic::DoDataExchange(pDX);
//}


void ProgressBar::OnPaint()
{
	CPaintDC dc(this);
	CRect clientRect;
	GetClientRect(&clientRect);

	// FIX 1: Safety check! If the window has no size yet, exit immediately 
	// to prevent allocating an empty bitmap and passing a NULL context to GDI+.
	if (clientRect.Width() <= 0 || clientRect.Height() <= 0)
	{
		return;
	}

	// Double Buffering setup
	CDC memDC;
	CBitmap memBitmap;
	if (!memDC.CreateCompatibleDC(&dc)) return;
	if (!memBitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height())) return;

	CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

	// Fill memory buffer background with parent's dialog color
	COLORREF bgParent = ::GetSysColor(COLOR_3DFACE);
	memDC.FillSolidRect(&clientRect, bgParent);

	{
		// GDI+ scope
		Gdiplus::Graphics graphics(memDC.GetSafeHdc());
		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

		// Calculate size keeping padding intact
		int size = min(clientRect.Width(), clientRect.Height()) - static_cast<int>(m_dialLength*2);
		int x = (clientRect.Width() - size) / 2;
		int y = (clientRect.Height() - size) / 2;
		Gdiplus::RectF boundingRec(static_cast<Gdiplus::REAL>(x), static_cast<Gdiplus::REAL>(y),
			static_cast<Gdiplus::REAL>(size), static_cast<Gdiplus::REAL>(size));

		// Draw track base
		graphics.DrawArc(m_trackPen, boundingRec, 0.0f, 360.0f);

		// Draw current progress arc
		float range = m_maxVal - m_minVal;
		if (range > 0.0f)
		{
			Gdiplus::REAL startAngle = -90.0f; // Start at 12 o'clock
			Gdiplus::REAL sweepAngle = ((m_currentPos - m_minVal) / range) * 360.0f;

			if (sweepAngle > 0.0f)
			{
				graphics.DrawArc(m_thickBlackPen, boundingRec, startAngle, sweepAngle);
			}
		}

		// Draw text
		{

			    Gdiplus::FontFamily fontFamily(L"Calibri");
			    Gdiplus::Font font(&fontFamily, 12, Gdiplus::FontStyleBold, Gdiplus::UnitPoint);
			    Gdiplus::SolidBrush textBrush(Gdiplus::Color(60, 60, 60));

			    Gdiplus::StringFormat stringFormat;
			    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
			    stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

			    // FIX 2: BSTR memory safety! AllocSysString allocations must be explicitly freed. 
			    // Using a standard wide-string cast (PCWSTR) avoids allocation leaks and crashes.
			    graphics.DrawString(m_text.GetString(), -1, &font, boundingRec, &stringFormat, &textBrush);
		}

		//draw clock hand
		{
			// 1. Explicitly pull the actual center point relative to the bounding rectangle
			Gdiplus::REAL centerX = boundingRec.X + (boundingRec.Width / 2.0f);
			Gdiplus::REAL centerY = boundingRec.Y + (boundingRec.Height / 2.0f);

			// 2. Adjust radius slightly so the line sits perfectly within the middle of the track
			Gdiplus::REAL radius = (boundingRec.Width / 2.0f);
			
			//draw intervals
			{
				float interval_theta = 360.0 / intervals;
				for (int i = 0; i < intervals; i++)
				{
					float angle = -90.0 + i * interval_theta;
					float angle_rad = angle * (3.14159265f / 180.0f);
					Gdiplus::REAL dial_start_x = centerX + radius * cosf(angle_rad);
					Gdiplus::REAL dial_start_y = centerY + radius * sinf(angle_rad);
					Gdiplus::REAL extended_rad = radius + m_dialLength;
					Gdiplus::REAL dial_end_x = centerX + (extended_rad * cosf(angle_rad));
					Gdiplus::REAL dial_end_y = centerY + (extended_rad * sinf(angle_rad));
					graphics.DrawLine(m_thinBlackPen,dial_start_x,dial_start_y, dial_end_x, dial_end_y);
				}
			}

			// 6. Set up the hand pen vector configurations
			{
				// 3. Prevent division-by-zero if range is uninitialized
				float range = m_maxVal - m_minVal;

				// 4. Calculate angles accurately in radians
				float targetAngleDegrees = -90.0f + (((m_currentPos - m_minVal) / range) * 360.0f);
				float targetAngleRadians = targetAngleDegrees * (3.14159265f / 180.0f);

				// 5. Calculate outer track head endpoint positions
				Gdiplus::REAL startX = centerX + (20 * cosf(targetAngleRadians));
				Gdiplus::REAL startY = centerY + (20 * sinf(targetAngleRadians));
				Gdiplus::REAL endX = centerX + (radius * cosf(targetAngleRadians));
				Gdiplus::REAL endY = centerY + (radius * sinf(targetAngleRadians));
				graphics.DrawLine(m_thinBlackPen, startX, startY, endX, endY);

				//draw an arrow at edge of the hand
				//calculate 4 points
				Gdiplus::PointF points[3];
				Gdiplus::PointF p(endX, endY);
				Gdiplus::PointF vec = Gdiplus::PointF(endX - startX, endY - startY);
				//calculat the unit vector
				float len = sqrt(vec.X * vec.X + vec.Y * vec.Y);
				vec.X = vec.X / len;
				vec.Y = vec.Y / len;
				p.X -= vec.X * 10;
				p.Y -= vec.Y * 10;
				Gdiplus::PointF rtVec = Gdiplus::PointF(-vec.Y, vec.X);
				Gdiplus::PointF p1 = Gdiplus::PointF(p.X + rtVec.X * 3.0, p.Y + rtVec.Y * 3.0);
				Gdiplus::PointF p2 = Gdiplus::PointF(p.X - rtVec.X * 3.0, p.Y - rtVec.Y * 3.0);
				Gdiplus::PointF p3 = Gdiplus::PointF(p.X + vec.X * 7.0, p.Y + vec.Y * 7.0);
				points[0] = p1; points[1] = p2; points[2] = p3;

				//draw the polygon
				Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0, 215)); // Nice blue fill
				graphics.FillPolygon(&brush, points, 3);


			}

			//draw the central circle
			{
				Gdiplus::RectF crect(centerX - 20, centerY - 20, 40, 40);
				graphics.DrawEllipse(m_thinBlackPen, crect);
			}



		}

	}

	// Copy drawing onto screen at once
	dc.BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
}



BOOL ProgressBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CStatic::OnEraseBkgnd(pDC);

}
