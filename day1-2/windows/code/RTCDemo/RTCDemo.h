
// RTCDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CRTCDemoApp:
// See RTCDemo.cpp for the implementation of this class
//

class CRTCDemoApp : public CWinApp
{
public:
	CRTCDemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CRTCDemoApp theApp;
