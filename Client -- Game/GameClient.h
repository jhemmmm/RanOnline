
// GameClient.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define NO_GAMEGUARD

class CGameClientWnd;

// CGameClientApp:
// See GameClient.cpp for the implementation of this class
//

class CGameClientApp : public CWinApp
{
public:
	CGameClientApp();

protected:
	CGameClientWnd*	m_pWnd;
	BOOL		m_bIsActive;    
	BOOL		m_bRedrawScreen;
	BOOL		m_bShutDown;

public:
	CString m_strAppPath;
	TCHAR	m_szAppPath[MAX_PATH];

public:
	void SetActive(BOOL bActive);
	void ShutDown() { m_bShutDown = TRUE; };
	void SetAppPath();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CGameClientApp theApp;