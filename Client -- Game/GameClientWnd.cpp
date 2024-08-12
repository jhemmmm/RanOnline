#include "stdafx.h"

#ifndef STRICT
	#define STRICT
#endif

#include "DxGlobalStage.h"
#include "SUBPATH.h"
#include "RANPARAM.h"
#include "dxparamset.h"
#include "../Lib -- Engine/GUInterface/Cursor.h"
#include "DxCursor.h"

#include "GameClient.h"
#include "GameClientWnd.h"
#include "BlockProg.h"
#include "GLogicData.h"
#include "GLogic.h"
#include "s_NetClient.h"
#include "rol_clipboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd(NULL);

/////////////////////////////////////////////////////////////////////////////
// CGameClientWnd

CGameClientWnd::CGameClientWnd() :
	CD3DApplication(),
	m_bCreated(FALSE),
	m_wndSizeX(1024),
	m_wndSizeY(768),
	m_hCursorDefault(FALSE),
	m_nGGTimer(0),
	m_nGGATimer(0),
	m_nGGA12Timer(0),

	m_hMutex( NULL )
{
	m_bUseDepthBuffer = TRUE;
	m_pApp = (CGameClientApp*)AfxGetApp();
}

CGameClientWnd::~CGameClientWnd()
{
}

BEGIN_MESSAGE_MAP(CGameClientWnd, CWnd)
	ON_WM_ACTIVATEAPP()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_MESSAGE(NET_NOTIFY,  OnNetNotify)
	ON_WM_SYSCOMMAND()
	ON_WM_MOUSEMOVE()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCACTIVATE()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameClientWnd message handlers

BOOL CGameClientWnd::Create() 
{
	CGameClientApp *pRanClientApp = (CGameClientApp *) AfxGetApp();

	CRect rectWnd;
	LONG nWindowStyle = WS_POPUP | WS_VISIBLE;
	
	if ( RANPARAM::bScrWindowed )
	{
		m_nScreenWinX = GetSystemMetrics(SM_CXSCREEN);
		m_nScreenWinY = GetSystemMetrics(SM_CYSCREEN);

		m_wndSizeX = RANPARAM::dwScrWidth;
		m_wndSizeY = RANPARAM::dwScrHeight;

		if ( m_wndSizeX>m_nScreenWinX || m_wndSizeY>m_nScreenWinY )
		{
			m_wndSizeX = m_nScreenWinX;
			m_wndSizeY = m_nScreenWinY;
		}

		if ( RANPARAM::bScrWndFullSize )
		{
			m_wndSizeX = m_nScreenWinX;
			m_wndSizeY = m_nScreenWinY;

			/*float fScreen = m_nScreenWinX / float(m_nScreenWinY);
			if ( fScreen > (16.0f/9.0f) )
			{
			m_wndSizeX = m_nScreenWinX/2;
			}*/

			rectWnd = CRect( 0, 0, m_wndSizeX, m_wndSizeY );
		}else{
			if ( m_wndSizeX>=(m_nScreenWinX) || m_wndSizeY>=(m_nScreenWinY) )
			{
				m_wndSizeX = 1024;
				m_wndSizeY = 768;

				if ( m_wndSizeX>=m_nScreenWinX || m_wndSizeY>=m_nScreenWinY )
				{
					m_wndSizeX = m_nScreenWinX-60;
					m_wndSizeY = m_nScreenWinY-80;
				}
			}

			nWindowStyle = WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION |  WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_CLIPCHILDREN;

			RECT rt = {0, 0, m_wndSizeX, m_wndSizeY};
			AdjustWindowRectEx(&rt, nWindowStyle, FALSE, WS_EX_APPWINDOW);

			m_wndSizeX = rt.right - rt.left;
			m_wndSizeY = rt.bottom - rt.top;
			INT nX = (m_nScreenWinX - m_wndSizeX) / 2;
			INT nY = (m_nScreenWinY - m_wndSizeY) / 2;

			rectWnd = CRect( nX, nY, m_wndSizeX, m_wndSizeY );
		}
	}
	else
	{
		m_nScreenWinX = GetSystemMetrics(SM_CXSCREEN);
		m_nScreenWinY = GetSystemMetrics(SM_CYSCREEN);

		m_wndSizeX = m_nScreenWinX;
		m_wndSizeY = m_nScreenWinY;

		rectWnd = CRect( 0, 0, m_wndSizeX, m_wndSizeY );
	}

	if ( GetLastError() == ERROR_ALREADY_EXISTS ) 
	{ 
		return FALSE;
	}

	LPCSTR strClass = AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), AfxGetApp()->LoadIcon(IDR_MAINFRAME) );
	if( !strClass )
	{
		AfxMessageBox( _T( "Class Registration Failed\n" ) );
		return FALSE;
	}

	if ( CreateEx ( WS_EX_APPWINDOW, strClass, AfxGetAppName(), nWindowStyle, rectWnd.left, rectWnd.top, rectWnd.right, rectWnd.bottom, NULL, NULL, 0 ) == FALSE )
	{
		return FALSE;
	}

	SetWindowText (_T(RANPARAM::ClientWindowTitle));

	UpdateWindow();

	CD3DApplication::SetScreen( RANPARAM::dwScrWidth, RANPARAM::dwScrHeight, RANPARAM::emScrFormat, RANPARAM::uScrRefreshHz, RANPARAM::bScrWindowed );

	if ( FAILED( CD3DApplication::Create ( m_hWnd, m_hWnd, AfxGetInstanceHandle() ) ) )
		return FALSE;
	
	m_bCreated = TRUE;
	g_hWnd = m_hWnd;

	return TRUE;
}

void CGameClientWnd::PostNcDestroy() 
{
	::UnregisterClass( AfxGetAppName(), AfxGetInstanceHandle() ); 

	Cleanup3DEnvironment();

	CWnd::PostNcDestroy();

	m_pApp->ShutDown();

	if ( RANPARAM::bGameCursor )
	{
		::SetCursor ( NULL );
		if ( m_hCursorDefault )		::DestroyCursor ( m_hCursorDefault );
	}

	delete this;

	HWND hTrayWnd = ::FindWindow("Shell_TrayWnd", NULL);
	::SendMessage( hTrayWnd, WM_COMMAND, 416, 0 );
}

void CGameClientWnd::OnActivateApp(BOOL bActive, DWORD hTask)
{
	CWnd::OnActivateApp(bActive, hTask);
}

BOOL CGameClientWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if ( RANPARAM::bGameCursor )
	{
		HCURSOR hCursl = CCursor::GetInstance().GetCurCursor();
		if ( hCursl )
		{
			CCursor::GetInstance().SetCursorNow ();
			return TRUE;
		}
	}

	return CWnd::OnSetCursor ( pWnd, nHitTest, message );
}

void CGameClientWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( RANPARAM::bGameCursor )
	{
		HCURSOR hCursl = CCursor::GetInstance().GetCurCursor();
		if ( hCursl )
		{
			CCursor::GetInstance().SetCursorNow ();
			return;
		}
	}

	__super::OnMouseMove(nFlags, point);
}

void CGameClientWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar)
	{
	case VK_ESCAPE:
		break;
	};

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

LRESULT CGameClientWnd::OnNetNotify(WPARAM wParam, LPARAM lParam)
{
	DxGlobalStage::GetInstance().OnNetNotify ( wParam, lParam );
	return NULL;
}
