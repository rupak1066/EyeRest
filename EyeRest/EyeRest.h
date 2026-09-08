
// EyeRest.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include<gdiplus.h>

// CEyeRestApp:
// See EyeRest.cpp for the implementation of this class
//

class CEyeRestApp : public CWinApp
{
public:
	ULONG_PTR m_gdiplusToken;
	CEyeRestApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();//to explicitly destroy gdiPlus

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CEyeRestApp theApp;
