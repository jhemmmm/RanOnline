
// GameClient.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#ifndef STRICT
#define STRICT
#endif

#include "GameClient.h"
#include "GameClientWnd.h"

#include "RANPARAM.h"
#include "SUBPATH.h"
#include "MinBugTrap.h"
#include "dxparamset.h"
#include "StringUtils.h"
#include "getdxver.h"
#include "GameTextControl.h"
#include "GLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DO_RTC_CATCH _rtc_catch;

// CGameClientApp

BEGIN_MESSAGE_MAP(CGameClientApp, CWinApp)
	ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()


// CGameClientApp construction

CGameClientApp::CGameClientApp()
	: m_bIsActive(TRUE)
	, m_bRedrawScreen(FALSE)
	, m_bShutDown(FALSE)
{
	BUG_TRAP::BugTrapInstall( std::string(_T("GameClient")));
	RANPARAM::emSERVICE_TYPE = EMSERVICE_KOREA;
	SecureZeroMemory( m_szAppPath, sizeof(m_szAppPath) );
}


// The one and only CGameClientApp object

CGameClientApp theApp;


// CGameClientApp initialization

BOOL CGameClientApp::InitInstance()
{
	AfxEnableControlContainer();

	SetAppPath();
	RANPARAM::LOAD ( m_szAppPath );
	DXPARAMSET::INIT ();

	/* anti-zoomout cheat fix, SeiferXIII 2020/05/17 */
	RANPARAM::SETZOOMOUTVALUE( 200.0f );

	CString StrCmdLine = m_lpCmdLine;

	STRUTIL::ClearSeparator ();
	STRUTIL::RegisterSeparator ( "/" );
	STRUTIL::RegisterSeparator ( " " );

	CStringArray strCmdArray;
	STRUTIL::StringSeparate ( StrCmdLine, strCmdArray );

	BOOL bAPM = FALSE;
	BOOL bRCC = FALSE;

	for ( int i=0; i<strCmdArray.GetCount(); i++ )
	{
		CString strTemp = strCmdArray.GetAt(i);

		/*if ( strTemp == "app_run" )
		{
			bAPM = TRUE;
		}*/

		if ( strTemp == "use_rcc" || strTemp == "USE_RCC" )
		{
			bRCC = TRUE;
		}

		if( strTemp == "app_run_use_chinese" ){
			bAPM = TRUE;
			RANPARAM::strCountry = "cn";
			RANPARAM::dwLangSet = 1;
			RANPARAM::strGDIFont = "MingLiu";
		}
		if( strTemp == "app_run_use_english" ){
			bAPM = TRUE;
			RANPARAM::strCountry = "pe";
			RANPARAM::dwLangSet = 0;
			RANPARAM::strGDIFont = "Tahoma";
		}
	}

	if ( !bAPM )
	{
		//MessageBox ( NULL, "Please Use Launcher to Play!", "Info", MB_OK );
		return FALSE;
	}

	//rcc initialize
	GLOGIC::bGLOGIC_PACKFILE = FALSE;
	GLOGIC::bGLOGIC_ZIPFILE = FALSE;
	GLOGIC::bENGLIB_ZIPFILE = FALSE;

	if ( bRCC )
	{
		GLOGIC::bGLOGIC_PACKFILE = FALSE;
		GLOGIC::bGLOGIC_ZIPFILE = TRUE;
		GLOGIC::bENGLIB_ZIPFILE = TRUE;
	}

	DWORD dwDirectXVersion = 0;
	TCHAR strDirectXVersion[10];
	HRESULT hr = getdxversion ( &dwDirectXVersion, strDirectXVersion, 10 );

	if ( hr==S_OK && dwDirectXVersion<0x00090003 )
	{
		MessageBox ( NULL, "DirectX Version too old. must install DirectX 9.0c", "error", MB_OK );
		return FALSE;
	}

	TCHAR szFullPath[MAX_PATH] = {0};
	StringCchCopy( szFullPath, MAX_PATH, ((CGameClientApp*)AfxGetApp())->m_szAppPath );
	StringCchCat( szFullPath, MAX_PATH, SUBPATH::GUI_FILE_ROOT );

	CGameTextMan::GetInstance().SetPath(szFullPath);
	CGameTextMan::GetInstance().LoadText(RANPARAM::strGameWord.GetString(), CGameTextMan::EM_GAME_WORD, RANPARAM::bXML_USE );
	CGameTextMan::GetInstance().LoadText(RANPARAM::strGameInText.GetString(), CGameTextMan::EM_GAME_IN_TEXT, RANPARAM::bXML_USE );
	CGameTextMan::GetInstance().LoadText(RANPARAM::strGameExText.GetString(), CGameTextMan::EM_GAME_EX_TEXT, RANPARAM::bXML_USE );

	CGameClientWnd* pWnd = new CGameClientWnd();
	if ( pWnd->Create() == FALSE )	return FALSE;

	m_pWnd = pWnd;
	
	return TRUE;
}

void CGameClientApp::SetAppPath()
{
	CString strAppPath;

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);
	strAppPath = szPath;

	if ( !strAppPath.IsEmpty() )
	{
		DWORD dwFind = strAppPath.ReverseFind ( '\\' );
		if ( dwFind != -1 )
		{
			m_strAppPath = strAppPath.Left ( dwFind );

			if ( !m_strAppPath.IsEmpty() )
				if ( m_strAppPath.GetAt(0) == '"' )
					m_strAppPath = m_strAppPath.Right ( m_strAppPath.GetLength()-1 );

			StringCchCopy ( m_szAppPath, MAX_PATH, m_strAppPath.GetString() );
		}
	}else {
		MessageBox ( NULL, "SetAppPath Error", "Error", MB_OK );
		return;
	}
}

int CGameClientApp::ExitInstance() 
{
	m_pWnd = NULL;

	return CWinApp::ExitInstance();
}

int CGameClientApp::Run() 
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE );

	while( WM_QUIT != msg.message )
	{
		if ( m_bShutDown == TRUE || m_pWnd == NULL )
		{
			ExitInstance ();
			return 0;
		}

		if( m_bIsActive )
			bGotMsg = PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );
		else
			bGotMsg = GetMessage( &msg, NULL, 0, 0 );

		if( bGotMsg )
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);

			if ( !m_bIsActive && !m_bShutDown )
			{
				if ( FAILED ( m_pWnd->FrameMove3DEnvironment() ) )
					m_pWnd->PostMessage ( WM_CLOSE, 0, 0 );
			}
		}else{
			if ( m_bIsActive )
			{
				if ( FAILED ( m_pWnd->Render3DEnvironment() ) )
					m_pWnd->PostMessage ( WM_CLOSE, 0, 0 );
			}
		}		
	}

	ExitInstance ();
	return static_cast<int>(msg.wParam);
}

void CGameClientApp::SetActive ( BOOL bActive )
{
	m_bIsActive = bActive;
	m_bRedrawScreen = bActive;
}