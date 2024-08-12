#include "stdafx.h"

#include "SubPath.h"
#include "Dxviewport.h"
#include "dxincommand.h"
#include "DxInputDevice.h"
#include "DxEffectMan.h"

#include "DxSurfaceTex.h"
#include "DxGlowMan.h"
#include "DxPostProcess.h"

#include "GLPeriod.h"
#include "DxResponseMan.h"
#include "DxGlobalStage.h"
#include "s_NetClient.h"
#include "DxShadowMap.h"
#include "dxparamset.h"
#include "RANPARAM.h"
#include "DxGlobalStage.h"
#include "GLGaeaClient.h"
#include "NsSMeshSceneGraph.h"

#include "GameClient.h"
#include "GameClientWnd.h"

#include "D3DFont.h"
#include "BlockProg.h"

#include "CommonWeb.h"
#include "DxMeshTexMan.h"

#include "LoadingThread.h"
#include "InnerInterface.h"
#include "WaitServerDialogue.h"
#include "GameTextControl.h"

#include "DxFontMan.h"
#include "Cursor.h"

#include "DxSoundMan.h"
#include "BgmSound.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern HWND g_hWnd;

HRESULT CGameClientWnd::ConfirmDevice ( D3DCAPSQ* pCaps, DWORD dwBehavior, D3DFORMAT Format )
{
	if ( dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING )
		return E_FAIL;

	return S_OK;
}

HRESULT CGameClientWnd::Resize3DEnvironment()
{
	HRESULT hr=S_OK;
	hr = CD3DApplication::Resize3DEnvironment();
	if ( FAILED(hr) ) return hr;

	DxGlobalStage::GetInstance().ReSizeWindow ( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight );

	return S_OK;
}

void CGameClientWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect rectWnd;
	GetClientRect ( &rectWnd );

	WORD wWidth = rectWnd.Width();
	WORD wHeight = rectWnd.Height();

	if ( !RANPARAM::bScrWindowed && GetD3dDevice() )
	{
		LPDIRECT3DSURFACEQ pBackBuffer=NULL;
		GetD3dDevice()->GetBackBuffer ( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

		if ( pBackBuffer )
		{
			pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
			pBackBuffer->Release();

			wWidth = static_cast<WORD> ( m_d3dsdBackBuffer.Width );
			wHeight = static_cast<WORD> ( m_d3dsdBackBuffer.Height );
		}
	}

	if ( m_bCreated )	ReSizeWindow ( wWidth, wHeight );
}

HRESULT CGameClientWnd::OneTimeSceneInit()
{
	HRESULT hr(S_OK);
	CGameClientApp *pFrame = (CGameClientApp *) AfxGetApp();

	hr = DxResponseMan::GetInstance().OneTimeSceneInit ( pFrame->m_szAppPath, this, RANPARAM::strFontType, RANPARAM::dwLangSet, RANPARAM::strGDIFont );

	if ( FAILED(hr) )
	{
		return  E_FAIL;
	}

	dxincommand::bDISP_CONSOLE = RANPARAM::bDISP_CONSOLE;
	dxincommand::bDISP_FPS = RANPARAM::bDISP_FPS;

	if( m_bEnableGUI )	CCommonWeb::Get()->Create( (CWnd*)this, &m_bVisibleGUI, &m_rtBoundGUI );

	return S_OK;
}

HRESULT CGameClientWnd::CreateObjects()
{
	DxFontMan::GetInstance().InitDeviceObjects ( m_pd3dDevice );
	CD3DFontPar* pD3dFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pD3dFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW|D3DFONT_ASCII );
	CDebugSet::InitDeviceObjects( pD3dFont9 );

	HRESULT hr = S_OK;

	hr = NLOADINGTHREAD::StartThreadLOAD ( &m_pd3dDevice, m_hWnd, ((CGameClientApp*)AfxGetApp())->m_szAppPath, CString("loading_000.dds"), CString(""), FALSE );
	if( FAILED(hr) )	return E_FAIL;
	
	NLOADINGTHREAD::WaitThread ();

	hr = InitDeviceObjects ();
	if( FAILED(hr) )
	{
		NLOADINGTHREAD::EndThread ();
		return hr;
	}

	NLOADINGTHREAD::EndThread ();	
	
	hr = RestoreDeviceObjects();	
	if( FAILED(hr) )	return hr;

	return hr;
}

HRESULT CGameClientWnd::InitDeviceObjects()
{	
	HRESULT hr = S_OK;
	CGameClientApp *pFrame = (CGameClientApp *) AfxGetApp();

	DxViewPort::GetInstance().InitDeviceObjects ( m_pd3dDevice, m_hWnd );
	DxResponseMan::GetInstance().InitDeviceObjects ( m_pd3dDevice, TRUE );	

	DXLIGHT sDirectional;
	sDirectional.SetDefault();
	sDirectional.m_Light.Diffuse = D3DXCOLOR(0,0,0,1);
	sDirectional.m_Light.Ambient = D3DXCOLOR(0,0,0,1);
	DxLightMan::SetDefDirect ( sDirectional );

	WORD wWidth = m_wndSizeX;
	WORD wHeight = m_wndSizeY;

	DxGlobalStage::GetInstance().SetD3DApp(this);
	hr = DxGlobalStage::GetInstance().OneTimeSceneInit ( pFrame->m_szAppPath, m_hWndApp, FALSE, wWidth, wHeight );
	if ( FAILED(hr) )	return hr;

	hr = DxGlobalStage::GetInstance().InitDeviceObjects ( m_pd3dDevice );	
	if ( FAILED(hr) )	return hr;

	NSOCTREE::EnableDynamicLoad();

	LOADINGSTEP::SETSTEP ( 11 );

	char szFullPath[MAX_PATH] = {0};
	StringCchCopy ( szFullPath, MAX_PATH, pFrame->m_szAppPath );
	StringCchCat ( szFullPath, MAX_PATH, SUBPATH::OBJ_FILE_EDIT );

	CCursor::GetInstance().SetGameCursor ( RANPARAM::bGameCursor );
	CCursor::GetInstance().OneTimeSceneInit(szFullPath,m_hWnd);
	CCursor::GetInstance().InitDeviceObjects ();

	return S_OK;
}

HRESULT CGameClientWnd::RestoreDeviceObjects()
{
	HRESULT hr=S_OK;
	DxResponseMan::GetInstance().RestoreDeviceObjects ();
	DxGlobalStage::GetInstance().RestoreDeviceObjects ();
	DxGlowMan::GetInstance().SetProjectActiveON();
	DxPostProcess::GetInstance().SetProjectActiveON();
	DXPARAMSET::INIT ();
	PROFILE_INIT();
	PROFILE_INIT2();

	return S_OK;
}

HRESULT CGameClientWnd::FrameMove3DEnvironment ()
{
	HRESULT hr=S_OK;

	if ( m_bCreated )
	{
		hr = CD3DApplication::FrameMove3DEnvironment ();
		if ( FAILED(hr) )	return hr;
	}

	PROFILE_BLOCKEND();
	PROFILE_DUMPOUTPUT();

	return S_OK;
}

HRESULT CGameClientWnd::Render3DEnvironment ()
{
	HRESULT hr=S_OK;
	
	if ( m_bCreated )
	{
		hr = CD3DApplication::Render3DEnvironment ();
		if ( FAILED(hr) )	return hr;
	}

	return S_OK;
}

HRESULT CGameClientWnd::FrameMove()
{
	PROFILE_BLOCKSTART();

	PROFILE_BEGIN("FrameMove");

	DxGlobalStage::GetInstance().FrameMove ( m_fTime, m_fElapsedTime );
	DxResponseMan::GetInstance().FrameMove ( m_fTime, m_fElapsedTime, m_bDefWin, TRUE );
	DxViewPort::GetInstance().FrameMove ( m_fTime, m_fElapsedTime );

	PROFILE_END("FrameMove");
	PROFILE_BLOCKEND();
	PROFILE_DUMPOUTPUT();

	DWORD dwL_ALT = DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU );
	DWORD dwRETURN = DxInputDevice::GetInstance().GetKeyState ( DIK_RETURN );
	if ( (dwL_ALT&DXKEY_PRESSED) && (dwRETURN&DXKEY_UP) )	
	{
		if ( SUCCEEDED( ToggleFullscreen() ) )
		{
			RANPARAM::bScrWindowed = !RANPARAM::bScrWindowed;
			DXPARAMSET::GetInstance().m_bScrWindowed = !DXPARAMSET::GetInstance().m_bScrWindowed;
		}
	}

	return S_OK;
}

HRESULT CGameClientWnd::Render()
{
	HRESULT hr(S_OK);
	if ( !m_pd3dDevice )	return S_FALSE;

	PROFILE_BLOCKSTART2();

	PROFILE_BEGIN2("Render");

	D3DCOLOR colorClear = D3DCOLOR_XRGB(89,135,179);
	DxFogMan::GetInstance().RenderFogSB ( m_pd3dDevice );
	colorClear = DxFogMan::GetInstance().GetFogColor();

	// Clear the viewport
	hr = m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,colorClear, 1.0f, 0L );

	// Begin the scene
	if( SUCCEEDED(hr=m_pd3dDevice->BeginScene()) )
	{
		DxLightMan::GetInstance()->Render ( m_pd3dDevice );

		DXLIGHT &Light = *DxLightMan::GetInstance()->GetDirectLight ();
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIRECT, (float*)&Light.m_Light.Direction, 1 );
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIFFUSE, (float*)&Light.m_Light.Diffuse, 1 );
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTAMBIENT, (float*)&Light.m_Light.Ambient, 1 );

		D3DXVECTOR3 &vFromPt = DxViewPort::GetInstance().GetFromPt ();
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_CAMERAPOSITION, (float*)&vFromPt, 1 );

		D3DLIGHTQ	pLight;
		D3DXVECTOR4	vPointPos;
		D3DXVECTOR3	vPointDiff;
		for ( int i=0; i<7; i++ )
		{
			if ( DxLightMan::GetInstance()->GetClosedLight(i+1) )
			{
				pLight = DxLightMan::GetInstance()->GetClosedLight(i+1)->m_Light;
				vPointDiff = D3DXVECTOR3 ( pLight.Diffuse.r, pLight.Diffuse.g, pLight.Diffuse.b );
				vPointPos.x = pLight.Position.x;
				vPointPos.y = pLight.Position.y;
				vPointPos.z = pLight.Position.z;
				vPointPos.w = pLight.Range;
			}
			else
			{
				vPointPos = D3DXVECTOR4 ( 0.f, 0.f, 0.f, 0.1f );
				vPointDiff = D3DXVECTOR3 ( 0.f, 0.f, 0.f );
			}
			m_pd3dDevice->SetVertexShaderConstantF (i*2+VSC_PLIGHTPOS01, (float*)&vPointPos, 1);
			m_pd3dDevice->SetVertexShaderConstantF (i*2+VSC_PLIGHTDIFF01, (float*)&vPointDiff, 1);
		}

		D3DXMATRIX matView = DxViewPort::GetInstance().GetMatView();
		D3DXMATRIX matProj = DxViewPort::GetInstance().GetMatProj();

		m_pd3dDevice->SetVertexShaderConstantF ( VSC_SKIN_DEFAULT, (float*)&D3DXVECTOR4 (1.f, 0.5f, 0.f, 765.01f), 1 );

		D3DXVECTOR3	vLightVector = DxLightMan::GetInstance()->GetDirectLight()->m_Light.Direction;
		D3DXVec3TransformNormal ( &vLightVector, &vLightVector, &matView );
		D3DXVec3Normalize ( &vLightVector, &vLightVector);
		vLightVector = -vLightVector;
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIRECT_VIEW, (float*)&vLightVector, 1 );

		D3DXMatrixTranspose( &matView, &matView );
		D3DXMatrixTranspose( &matProj, &matProj );

		D3DXMATRIX matIdentity;
		D3DXMatrixIdentity( &matIdentity );
		m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matIdentity );

		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_MODULATE );

		PROFILE_BEGIN2("Render_DxGlobalStage");
		DxGlobalStage::GetInstance().Render ();
		PROFILE_END2("Render_DxGlobalStage");

		CCursor::GetInstance().Render ( m_pd3dDevice, DxInputDevice::GetInstance().GetMouseLocateX(), DxInputDevice::GetInstance().GetMouseLocateY() );

		PROFILE_END2("Render");
		PROFILE_BLOCKEND2();
		PROFILE_DUMPOUTPUT2();

		RenderText();

		m_pd3dDevice->EndScene();

		return S_OK;
	}else{
		m_pd3dDevice->EndScene();
	}

	PROFILE_END2("Render");
	PROFILE_BLOCKEND2();
	PROFILE_DUMPOUTPUT2();

	return S_OK;
}

HRESULT CGameClientWnd::RenderText()
{
	if ( !dxincommand::bDISP_CONSOLE && !dxincommand::bDISP_FPS )	return S_OK;

    D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,255);
    D3DCOLOR fontWarningColor = D3DCOLOR_ARGB(255,0,255,255);
    TCHAR szMsg[MAX_PATH] = TEXT("");

	// Output display stats
	CD3DFontPar* pD3dFont = DxFontMan::GetInstance().FindFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW|D3DFONT_ASCII );
	if ( !pD3dFont )	return S_OK;

	if ( dxincommand::bDISP_FPS )
	{
		// Output display stats
		FLOAT fNextLine = 40.0f; 

		StringCchCopy ( szMsg, MAX_PATH, m_strDeviceStats );
		fNextLine -= 20.0f;
		pD3dFont->DrawText( 2, fNextLine, fontColor, szMsg );

		StringCchCopy( szMsg, MAX_PATH, m_strFrameStats );
		fNextLine -= 20.0f;
		pD3dFont->DrawText( 2, fNextLine, fontColor, szMsg );
	}

	if ( dxincommand::bDISP_CONSOLE )
	{
		CDebugSet::Render ();
	}

    return S_OK;
}

HRESULT CGameClientWnd::InvalidateDeviceObjects()
{
	DxResponseMan::GetInstance().InvalidateDeviceObjects ();
	DxGlobalStage::GetInstance().InvalidateDeviceObjects ();

	return S_OK;
}

HRESULT CGameClientWnd::DeleteDeviceObjects()
{
	DxResponseMan::GetInstance().DeleteDeviceObjects ();
	DxGlobalStage::GetInstance().DeleteDeviceObjects ();
	CCursor::GetInstance().DeleteDeviceObjects ();

	return S_OK;
}

HRESULT CGameClientWnd::FinalCleanup()
{
	DxGlobalStage::GetInstance().FinalCleanup ();
	DxResponseMan::GetInstance().FinalCleanup ();
	return S_OK;
}

void CGameClientWnd::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID==SC_KEYMENU)	return;
	__super::OnSysCommand(nID, lParam);
}

BOOL CGameClientWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;
	DxGlobalStage::GetInstance().MsgProc ( &msg );
	return __super::OnWndMsg(message, wParam, lParam, pResult);
}


void CGameClientWnd::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 600;

	__super::OnGetMinMaxInfo(lpMMI);
}

BOOL CGameClientWnd::OnNcActivate(BOOL bActive)
{
	DxInputDevice::GetInstance().OnActivate ( bActive );
	DxBgmSound::GetInstance().SetActivate ( bActive );
	return __super::OnNcActivate(bActive);
}

void CGameClientWnd::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
}

void CGameClientWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);
	m_pApp->SetActive ( !bMinimized );
}
