#include "StdAfx.h"
#include "./DxLobyStage.h"

#include "NETCOMMENTS.h"
#include "DxGlobalStage.h"

#include "DxEffectMan.h"
#include "DxEnvironment.h"
#include "DxSurfaceTex.h"
#include "DxGlowMan.h"
#include "DxPostProcess.h"
#include "DxCubeMap.h"
#include "DxSkyMan.h"
#include "DxEffGroupPlayer.h"

#include "DxDynamicVB.h"	// 삭제 해야한다.
#include "../Lib -- ClientUI/Interface/GameTextControl.h"
#include "../Lib -- ClientUI/Interface/ModalCallerID.h"
#include "../Lib -- ClientUI/Interface/ModalWindow.h"
#include "DxRenderStates.h"

#include "../Lib -- ClientUI/Interface/SelectCharacterPage.h"
#include "../Lib -- ClientUI/Interface/OuterInterface.h"

#include "../Lib -- Engine/G-Logic/GLPeriod.h"
#include "../Lib -- Engine/DxCommon/DxViewPort.h"
#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "../Lib -- Engine/DxCommon/DxInputDevice.h"
#include "../Lib -- Engine/DxCommon/DxLightMan.h"

#include "../Lib -- Engine/DxSound/BgmSound.h"


//#include "../Lib -- EngineUI/GUInterface/UIRenderQueue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DxLobyStage::DxLobyStage(void) :
	m_nStartCharNum(0),
	m_nStartCharLoad(0),
	m_nChaSNum(0),
	m_dwCharSelect(MAX_SELECTCHAR),
	m_bGameJoin(FALSE),
	m_nCharStart(0),
	m_fShiftTime(0),
	m_bShiftButton(FALSE),
	m_bShiftLBDown(FALSE),
	m_bShiftRBDown(FALSE),
	m_bUseInterface(TRUE),
	m_bRenderSkip(FALSE),
	m_fRenderSkipTime(0.0f),
	m_bStyle( false ),
	m_vDir ( D3DXVECTOR3( -1.0f,-10.0f,-0.35f) )
{
}

DxLobyStage::~DxLobyStage(void)
{
}

HRESULT DxLobyStage::ReSizeWindow ( WORD wWidth, WORD wHeight )
{
	//	Note : 인터페이스 초기화.
	//
	COuterInterface::GetInstance().SetResolution (MAKELONG(wHeight,wWidth));

	InvalidateDeviceObjects();
	RestoreDeviceObjects();

	return S_OK;
}

void DxLobyStage::OnInitLogin ()
{
	m_nStartCharLoad = 0;
	m_nChaSNum = 0;
	m_bGameJoin = FALSE;
	m_dwCharSelect = MAX_SELECTCHAR;
	m_nCharStart = 0;
	m_bGameJoin = FALSE;
	m_fShiftTime = 0;
	m_bShiftButton = FALSE;
	m_bShiftLBDown = FALSE;
	m_bShiftRBDown = FALSE;
	m_bUseInterface = TRUE;

	for ( int i=0; i<MAX_SERVERCHAR; i++ ) m_CharInfo[i] = SCHARINFO_LOBBY();
	for ( int i=0; i<MAX_SELECTCHAR; i++ ) m_CharSham[i].ResetCharInfo ();

	m_NewCharSham.ResetCharInfo();
}

SCHARINFO_LOBBY* DxLobyStage::GetSelectCharInfo ()
{
	if ( m_dwCharSelect >= MAX_SELECTCHAR )	
		return NULL;

	return &(m_CharInfo[m_dwCharSelect+m_nCharStart]);
}

SCHARINFO_LOBBY* DxLobyStage::GetCharInfo( int nSelect )
{
	if ( nSelect < 0 || nSelect >= MAX_SERVERCHAR || nSelect >= m_nChaSNum )	return NULL;
	return &(m_CharInfo[nSelect]);
}


//***********************************************
// 초기 캐릭터 렌더링시 애니메이션을 1초정도 스킵
//***********************************************
void DxLobyStage::InitRenderSkip()
{
	m_bRenderSkip = TRUE;
	m_fRenderSkipTime = 0.0f;
}

HRESULT DxLobyStage::OneTimeSceneInit ( HWND hWnd, WORD wWidth, WORD wHeight, const char* szAppPath )
{
	GASSERT(hWnd);

	StringCchCopy ( m_szAppPath, MAX_PATH, szAppPath );
	m_hWnd = hWnd;

	m_dwCharSelect = MAX_SELECTCHAR;
	m_nCharStart = 0;

	return S_OK;
}

HRESULT DxLobyStage::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr;

	m_pd3dDevice = pd3dDevice;

	hr = m_pd3dDevice->GetDeviceCaps ( &m_d3dCaps );
	if ( FAILED(hr) )	return hr;

	WORD wWidth = 800;
	WORD wHeight = 600;

	//	Note : 현재해상도 받아오기.
	//
	LPDIRECT3DSURFACEQ pBackBuffer=NULL;
	m_pd3dDevice->GetBackBuffer ( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

	if ( pBackBuffer )
	{
		pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
		pBackBuffer->Release();

		wWidth = static_cast<WORD> ( m_d3dsdBackBuffer.Width );
		wHeight = static_cast<WORD> ( m_d3dsdBackBuffer.Height );
	}

	//	Note : 인터페이스 초기화.
	//
	COuterInterface::GetInstance().SetResolution ( MAKELONG ( wHeight, wWidth ) );
	COuterInterface::GetInstance().SetResolutionBack ( MAKELONG ( 768, 1024 ) );
	COuterInterface::GetInstance().OneTimeSceneInit ();

	//	Note : 뷰포트.
	//
	DxViewPort::GetInstance().SetLobyCamera ();
	D3DXVECTOR3 vFromPt		= D3DXVECTOR3( 0.0f, 70.0f, -70.0f );
	D3DXVECTOR3 vLookatPt	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );

	//	Note : 인터페이스 초기화.
	//
	hr = COuterInterface::GetInstance().InitDeviceObjects ( m_pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

	//	Note : 캐릭터 선택 배경 지형 파일 초기화.
	//
	m_LandCharSlt.InitDeviceObjects ( m_pd3dDevice );
	m_LandCharSlt.LoadFile ( "cha_select.wld", m_pd3dDevice, FALSE );

	m_LandLogin.InitDeviceObjects ( m_pd3dDevice );
	m_LandLogin.LoadFile ( "login.wld", m_pd3dDevice, FALSE );

	m_LandCreate.InitDeviceObjects ( m_pd3dDevice );
	m_LandCreate.LoadFile ( "cha_select.wld", m_pd3dDevice, FALSE );

	// Note : 미리 로딩..
	D3DXVECTOR3 vMax = D3DXVECTOR3(100000,100000,100000);
	D3DXVECTOR3 vMin = D3DXVECTOR3(-100000,-100000,-100000);
	m_LandCharSlt.LoadBasicPos( m_pd3dDevice, vMax, vMin );
	m_LandLogin.LoadBasicPos( m_pd3dDevice, vMax, vMin );
	m_LandCreate.LoadBasicPos( m_pd3dDevice, vMax, vMin );

	m_LandLogin.ActiveMap ();

	//	NOTE
	//		BGM 세팅, 로비는 고정
	DxBgmSound::GetInstance().SetFile ( ID2GAMEWORD("LOBY_BGM") );
	DxBgmSound::GetInstance().Play ();

	//	Note : 캐릭터 선택 오브젝트 초기화.
	//
	char szName[MAX_PATH] = {0};
	for ( int i=0; i<MAX_SELECTCHAR; i++ )
	{
		StringCchPrintf ( szName, MAX_PATH, "character[%02d]", i+1 );
		DxLandGateMan* pLandGateMan = m_LandCharSlt.GetLandGateMan();
		PDXLANDGATE pLandGate = pLandGateMan->FindLandGate ( szName );
		GASSERT(pLandGate&&"character[%02d]를 찾지 못하였습니다." );
		if ( pLandGate )
		{
			D3DXVECTOR3 vPos = (pLandGate->GetMax()+pLandGate->GetMin())/2.0f;
			m_CharSham[i].Create( &vPos, m_pd3dDevice, FALSE, TRUE, i );
		}
	}

	D3DXVECTOR3 vPos = D3DXVECTOR3(-7.36f, 3.56f, -417.77f);
	m_NewCharSham.Create( &vPos, m_pd3dDevice, FALSE, TRUE, MAX_SELECTCHAR );

	return S_OK;
}

HRESULT DxLobyStage::RestoreDeviceObjects ()
{
	HRESULT hr;
	LPDIRECT3DSURFACEQ pBackBuffer;
	m_pd3dDevice->GetBackBuffer ( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

    pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
    pBackBuffer->Release();

	//	Note : 뷰포트
	//
	DxViewPort::GetInstance().SetProjection ( D3DX_PI/4, (float)m_d3dsdBackBuffer.Width, 
		(float)m_d3dsdBackBuffer.Height, 5.0f, 3000.0f );
	
	hr = DxViewPort::GetInstance().SetViewPort ();
	if ( FAILED(hr) )	return hr;

	COuterInterface::GetInstance().RestoreDeviceObjects ( m_pd3dDevice );

	//	Note : 캐릭터 선택 배경 지형 파일 초기화.
	//
	m_LandCharSlt.RestoreDeviceObjects ( m_pd3dDevice );
	m_LandLogin.RestoreDeviceObjects ( m_pd3dDevice );
	m_LandCreate.RestoreDeviceObjects ( m_pd3dDevice );

	//	Note : 캐릭터 선택 오브젝트 초기화.
	//
	for ( int i=0; i<MAX_SELECTCHAR; i++ )
	{
		m_CharSham[i].RestoreDeviceObjects ( m_pd3dDevice );
	}

	m_NewCharSham.RestoreDeviceObjects( m_pd3dDevice );

	return S_OK;
}

HRESULT DxLobyStage::InvalidateDeviceObjects ()
{
	COuterInterface::GetInstance().InvalidateDeviceObjects ();

	//	Note : 캐릭터 선택 배경 지형 파일 초기화.
	//
	m_LandCharSlt.InvalidateDeviceObjects ();
	m_LandLogin.InvalidateDeviceObjects ();
	m_LandCreate.InvalidateDeviceObjects ();

	//	Note : 캐릭터 선택 오브젝트 초기화.
	//
	for ( int i=0; i<MAX_SELECTCHAR; i++ )
	{
		m_CharSham[i].InvalidateDeviceObjects ();
	}

	m_NewCharSham.InvalidateDeviceObjects();
	return S_OK;
}

HRESULT DxLobyStage::DeleteDeviceObjects ()
{
	//	Note : 캐릭터 선택 배경 지형 파일 초기화.
	//
	m_LandCharSlt.DeleteDeviceObjects ();
	m_LandLogin.DeleteDeviceObjects ();
	m_LandCreate.DeleteDeviceObjects ();

	COuterInterface::GetInstance().DeleteDeviceObjects ();

	DxBgmSound::GetInstance().ForceStop ();

	return S_OK;
}

HRESULT DxLobyStage::FinalCleanup ()
{
	//	Note : 캐릭터 선택 배경 지형 파일 초기화.
	//
	m_LandCharSlt.CleanUp ();
	m_LandLogin.CleanUp ();
	m_LandCreate.CleanUp ();

	COuterInterface::GetInstance().FinalCleanup ();

	m_bGameJoin = FALSE;

	return S_OK;
}

HRESULT DxLobyStage::FrameMove ( float fTime, float fElapsedTime )
{
	DxInputDevice &dxInputDev = DxInputDevice::GetInstance();
	DWORD MouseLKey = dxInputDev.GetMouseState ( DXMOUSE_LEFT );

	if( (dxInputDev.GetKeyState( DIK_F12 )&DXKEY_DOWN) )
	{
		m_bUseInterface = !m_bUseInterface;
	}

	if( m_bUseInterface )
	{
		COuterInterface::GetInstance().FrameMove ( m_pd3dDevice, fTime, fElapsedTime );
	}

	UIGUID ControlID = COuterInterface::GetInstance().GetRenderStage ();
	
	//	Note : 스테이지 변경을 감지하여 
	//
	static UIGUID ControlIDBack = NO_ID;
	if ( ControlIDBack != ControlID )
	{
		switch ( ControlIDBack )
		{
		case SELECT_CHINA_AREA_PAGE:
		case SELECT_SERVER_PAGE:
		case LOGIN_PAGE:
		case RESETPASS_PAGE: //userpanel
		case RESETPASS_PAGE2:		
		case PASS_KEYBOARD:
		case SECPASS_SETPAGE:
		case SECPASS_CHECKPAGE:
		case REGISTER_PAGE: /*register page, Juver, 2017/11/17 */
			break;

		case SELECT_CHARACTER_PAGE:
			for ( int i=0; i<MAX_SELECTCHAR; i++ )
			{
				m_CharSham[i].SetSelect ( FALSE );
				m_CharSham[i].FrameMove ( 0, 0 );
			}
			break;

		//userpanel
		case SELECT_CHARACTER_USERPANEL:
		case CHANGEPASS_PAGE:
		case CHANGEPIN_PAGE:
		case CHANGEEMAIL_PAGE:
		case RESETPIN_PAGE:
		case TOPUP_PAGE:
		case GAMETIMECVT_PAGE:
		case PREMIUMPOINTCVT_PAGE:
			{
				for ( int i=0; i<MAX_SELECTCHAR; i++ )
				{
					m_CharSham[i].SetSelect ( FALSE );
					m_CharSham[i].FrameMove ( 0, 0 );
				}
			}
			break;

		case CHARACTER_CREATE_PAGE:
		case CHARACTER_CREATE_INFO:
		case CHARACTER_CREATE_ROTATE:
			m_NewCharSham.SetSelect ( FALSE );
			m_NewCharSham.FrameMove ( 0, 0 );
			break;
		};

		DxWeatherMan::GetInstance()->ReSetSound ();
//		DxBgmSound::GetInstance().Stop ();

		switch ( ControlID )
		{
		case SELECT_CHINA_AREA_PAGE:
		case SELECT_SERVER_PAGE:
		case LOGIN_PAGE:
		case RESETPASS_PAGE: //userpanel
		case RESETPASS_PAGE2:
		case PASS_KEYBOARD:
		case SECPASS_SETPAGE:
		case SECPASS_CHECKPAGE:
		case REGISTER_PAGE: /*register page, Juver, 2017/11/17 */
			{
				if( /*ControlIDBack==SELECT_SERVER_PAGE ||*/ 
					ControlIDBack==LOGIN_PAGE ||
					ControlIDBack==PASS_KEYBOARD || 
					ControlIDBack==SELECT_CHINA_AREA_PAGE ||
					ControlIDBack==SECPASS_SETPAGE ||
					ControlIDBack==SECPASS_CHECKPAGE ||
					ControlIDBack==REGISTER_PAGE )  /*register page, Juver, 2017/11/17 */
					break;
				
				m_LandLogin.ActiveMap ();
				DxCamAniMan* pCamAniMan = m_LandLogin.GetCamAniMan();
				pCamAniMan->ActiveCameraAni ( "main" );
				
				//D3DXVECTOR3 vFromPt		= D3DXVECTOR3( 50.5265f, 1.40827f, -153.122f );
				//D3DXVECTOR3 vLookatPt	= D3DXVECTOR3( 45.0f, 0.0f, 0.0f );
				//D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
				//DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );

			}break;

		case SELECT_CHARACTER_PAGE:
			{
				if( ControlIDBack==SELECT_CHARACTER_PAGE ) 
					break;

				//	선택된 케릭터 초기화.
				m_dwCharSelect = MAX_SELECTCHAR;

				m_LandCharSlt.ActiveMap ();
				DxCamAniMan* pCamAniMan = m_LandCharSlt.GetCamAniMan();
				pCamAniMan->ActiveCameraPos ( "select_character" );
				pCamAniMan->ActiveCameraAni ( "select_character" );

				InitRenderSkip();

				DxLightMan::GetInstance()->EnableLighting( true );
			}break;
		
		//userpanel
		case SELECT_CHARACTER_USERPANEL:
		case CHANGEPASS_PAGE:
		case CHANGEPIN_PAGE:
		case CHANGEEMAIL_PAGE:
		case TOPUP_PAGE:
		case RESETPIN_PAGE:
		case GAMETIMECVT_PAGE:
		case PREMIUMPOINTCVT_PAGE:
			{
				if( ControlIDBack==SELECT_CHARACTER_PAGE ||
					ControlIDBack==SELECT_CHARACTER_USERPANEL||
					ControlIDBack==CHANGEPIN_PAGE||
					ControlIDBack==RESETPIN_PAGE||
					ControlIDBack==CHANGEPASS_PAGE||
					ControlIDBack==TOPUP_PAGE||
					ControlIDBack==TOPUP_PAGE||
					ControlIDBack==GAMETIMECVT_PAGE||
					ControlIDBack==PREMIUMPOINTCVT_PAGE) 
					break;

				//	선택된 케릭터 초기화.
				m_dwCharSelect = MAX_SELECTCHAR;
				m_LandCharSlt.ActiveMap ();
				DxCamAniMan* pCamAniMan = m_LandCharSlt.GetCamAniMan();
				pCamAniMan->ActiveCameraPos ( "main" );

				InitRenderSkip();

				DxLightMan::GetInstance()->EnableLighting( true );
				DxLightMan::GetInstance()->EnableNightAndDay( true );
			}
			break;

		case CHARACTER_CREATE_PAGE:
		case CHARACTER_CREATE_INFO:
		case CHARACTER_CREATE_ROTATE:
			{
				if( ControlIDBack==CHARACTER_CREATE_PAGE ||
					ControlIDBack==CHARACTER_CREATE_INFO ||
					ControlIDBack==CHARACTER_CREATE_ROTATE )	
					break;

				m_sCharNewInfo.RESET();
				SelCharSchool( m_sCharNewInfo.m_wSchool );
						
				m_LandCreate.ActiveMap ();
				DxCamAniMan* pCamAniMan = m_LandCreate.GetCamAniMan();
				pCamAniMan->ActiveCameraPos ( "create_camera" );
				pCamAniMan->ActiveCameraAni ( "create_camera" );

				InitRenderSkip();
				DxLightMan::GetInstance()->EnableLighting( true );	

			}break;
		};

		ControlIDBack = ControlID;
	}

	switch ( ControlID )
	{
	case SELECT_CHINA_AREA_PAGE:
	case SELECT_SERVER_PAGE:
	case LOGIN_PAGE:
	case RESETPASS_PAGE: //userpanel
	case RESETPASS_PAGE2:
	case PASS_KEYBOARD:
	case SECPASS_SETPAGE:
	case SECPASS_CHECKPAGE:
	case REGISTER_PAGE: /*register page, Juver, 2017/11/17 */
		{
			//	Note : 뷰포트
			//
			DxViewPort::GetInstance().SetProjection ( D3DX_PI/4, (float)m_d3dsdBackBuffer.Width, 
				(float)m_d3dsdBackBuffer.Height, 5.0f, 3000.0f );

			GLPeriod::GetInstance().SetPeriod ( 2003, 180, 1, 0 );
			m_LandLogin.FrameMove ( fTime, fElapsedTime );
		}break;

	case SELECT_CHARACTER_PAGE:
	case SELECT_CHARACTER_USERPANEL: //userpanel
	case CHANGEPASS_PAGE:
	case CHANGEPIN_PAGE:
	case CHANGEEMAIL_PAGE:
	case TOPUP_PAGE:
	case RESETPIN_PAGE:
	case GAMETIMECVT_PAGE:
	case PREMIUMPOINTCVT_PAGE:		{
			//	Note : 뷰포트
			//
			DxViewPort::GetInstance().SetProjection ( D3DX_PI/4, (float)m_d3dsdBackBuffer.Width, 
				(float)m_d3dsdBackBuffer.Height, 5.0f, 600.0f );

			GLPeriod::GetInstance().SetPeriod ( 2003, 180, 1, 0 );

			//	Note : 캐릭터 선택 배경 지형 파일.
			//
			m_LandCharSlt.ActiveMap ();
			m_LandCharSlt.FrameMove ( fTime, fElapsedTime );

			D3DXVECTOR3 vDir = DxViewPort::GetInstance().GetFromPt() - DxViewPort::GetInstance().GetLookatPt();

			//	Note : 캐릭터 선택 오브젝트.
			//
			for ( int i=0; i<MAX_SELECTCHAR; i++ )
			{
				m_CharSham[i].SetDirection ( vDir );
				m_CharSham[i].FrameMove ( fTime, fElapsedTime );
			}

			BOOL bOnInterface = COuterInterface::GetInstance().IsOnTheInterface();

			DWORD dwCharSelect = MAX_SELECTCHAR;
			if ( !bOnInterface && MouseLKey&(DXKEY_UP|DXKEY_DUP) && m_bGameJoin == FALSE )
			{
				D3DXVECTOR3 vFrom, vLook;
				vFrom = DxViewPort::GetInstance().GetFromPt ();
				BOOL bOk = DxViewPort::GetInstance().GetMouseTargetPosWnd ( vLook );

				if ( bOk )
				{
					//for ( int i=0; i<m_nChaSNum; i++ )
					for ( int i=0; i<MAX_SELECTCHAR; i++ )
					{
						if ( !m_CharSham[i].IsValidData() )		continue;

						D3DXVECTOR3 vMax, vMin;
						m_CharSham[i].GetAABB ( vMax, vMin );

						BOOL bCollision = COLLISION::IsCollisionLineToAABB ( vFrom, vLook, vMax, vMin );
						if ( bCollision )
						{
							dwCharSelect = i;
							DxGlobalStage::GetInstance().GetGameStage()->SetSelectChar ( &m_CharInfo[dwCharSelect+m_nCharStart] );
							break;
						}
					}
				}
			}

			//	Note : 클릭된 케릭이 있을때.
			//
			if ( dwCharSelect != MAX_SELECTCHAR )
			{
				//	Note : 케릭터 선택 효과 표시.
				//
				if( m_dwCharSelect != dwCharSelect )
				{
					m_dwCharSelect = dwCharSelect;

					for ( int i=0; i<MAX_SELECTCHAR; i++ )	
						m_CharSham[i].SetSelect ( FALSE, TRUE );

					m_CharSham[m_dwCharSelect].SetSelect ( TRUE );
				}

				if ( !bOnInterface && MouseLKey&DXKEY_DUP && !m_bGameJoin )
				{
					//	TODO : 선택된 케릭으로 게임 접속.
					//
					m_bGameJoin = TRUE;

					GLMSG::SNETLOBBY_REQ_GAME_JOIN NetMsgReq;
					COuterInterface::GetInstance().MsgProcess ( &NetMsgReq );
				}
			}

			ShiftChar( fElapsedTime ); // 캐릭터 쉬프트

			if( m_bRenderSkip )
			{
				m_fRenderSkipTime += fElapsedTime;
			}
		}break;

	case CHARACTER_CREATE_PAGE:
	case CHARACTER_CREATE_INFO:
	case CHARACTER_CREATE_ROTATE:
		{
			DxCamAniMan* pCamAniMan = m_LandCreate.GetCamAniMan();
			pCamAniMan->ActiveCameraPos ( "create_camera" );
			pCamAniMan->ActiveCameraAni ( "create_camera" );

			DxViewPort::GetInstance().SetProjection ( D3DX_PI/4, (float)m_d3dsdBackBuffer.Width, 
				(float)m_d3dsdBackBuffer.Height, 5.0f, 600.0f );

			GLPeriod::GetInstance().SetPeriod ( 2003, 180, 1, 0 );

			m_LandCreate.ActiveMap ();
			m_LandCreate.FrameMove ( fTime, fElapsedTime );

			D3DXVECTOR3 vFromPt		= DxViewPort::GetInstance().GetFromPt();
			D3DXVECTOR3 vLookatPt	= DxViewPort::GetInstance().GetLookatPt();
			D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

			//hard coded values must be adjusted if map changed or map camera changed
			vFromPt.x += 0.0f;
			vFromPt.y += 2.5f;
			vFromPt.z += -19.8f;

			DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );

			if ( m_bStyle )
			{
				m_NewCharSham.SetSelect( FALSE );
				m_NewCharSham.SetDirection( m_vDir );

				m_NewCharSham.m_wFace = m_sCharNewInfo.m_wFace;
				m_NewCharSham.m_wHair = m_sCharNewInfo.m_wHair;
				m_NewCharSham.m_wHairColor = m_sCharNewInfo.m_wHairColor;
				m_NewCharSham.m_fScaleRange = m_sCharNewInfo.m_fScaleRange;

				m_NewCharSham.UpdateSuit();
				m_NewCharSham.FrameMove( fTime, fElapsedTime );	
			}

			if( m_bRenderSkip ){
				m_fRenderSkipTime += fElapsedTime;
			}

		}break;
	};
	 
	return S_OK;
}

void DxLobyStage::UpdateSelSkinChar ( DxSkinChar* pSkinObj, EMCHARINDEX emIndex, WORD wFace, WORD wHair, WORD wHairColor )
{
	if ( !pSkinObj )	return;

	const GLCONST_CHARCLASS &sCONST = GLCONST_CHAR::cCONSTCLASS[emIndex];

	if ( sCONST.dwHEADNUM > wFace )
	{
		std::string strHEAD_CPS = sCONST.strHEAD_CPS[wFace];
	
		PDXCHARPART pCharPart = NULL;
		pCharPart = pSkinObj->GetPiece(PIECE_HEAD);		//	현제 장착 스킨.

		if ( !pCharPart || !pCharPart->GetFileName() || strcmp(strHEAD_CPS.c_str(),pCharPart->GetFileName()) )
		{
			pSkinObj->SetPiece( strHEAD_CPS.c_str(), m_pd3dDevice );
		}
	}

	if ( sCONST.dwHAIRNUM > wHair )
	{
		std::string strHAIR_CPS = sCONST.strHAIR_CPS[wHair];
	
		PDXCHARPART pCharPart = NULL;
		pCharPart = pSkinObj->GetPiece(PIECE_HAIR);		//	현제 장착 스킨.

		if ( !pCharPart || !pCharPart->GetFileName() || strcmp(strHAIR_CPS.c_str(),pCharPart->GetFileName()) )
		{
			pSkinObj->SetPiece( strHAIR_CPS.c_str(), m_pd3dDevice );
		}
	}

	// 헤어컬러 적용
	pSkinObj->SetHairColor( wHairColor );
}

HRESULT DxLobyStage::Render ()
{
	CLIPVOLUME &CV = DxViewPort::GetInstance().GetClipVolume ();

	UIGUID ControlID = COuterInterface::GetInstance().GetRenderStage ();

	if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX )	
	{
		m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE, FALSE );
	}

	//	Note : 하늘 렌더링..!!
	DxSkyMan::GetInstance().Render ( m_pd3dDevice );

	//	Note : 캐릭터 선택 배경 지형 파일.
	//
	switch ( ControlID )
	{
	case SELECT_CHINA_AREA_PAGE:
	case SELECT_SERVER_PAGE:
	case LOGIN_PAGE:
//	case REGISTER_PAGE:
	case RESETPASS_PAGE: //userpanel
	case RESETPASS_PAGE2:
	case PASS_KEYBOARD:
	case SECPASS_SETPAGE:
	case SECPASS_CHECKPAGE:
	case REGISTER_PAGE: /*register page, Juver, 2017/11/17 */
		{
			PROFILE_BEGIN2("Environment Render");
			DxEnvironment::GetInstance().Render( m_pd3dDevice, NULL, &m_LandLogin );
			PROFILE_END2("Environment Render");

			m_LandLogin.Render ( m_pd3dDevice, CV );
			m_LandLogin.Render_EFF ( m_pd3dDevice, CV );
			m_LandLogin.RenderPickAlpha( m_pd3dDevice );
			DxWeatherMan::GetInstance()->Render ( m_pd3dDevice );
		}break;

	case SELECT_CHARACTER_PAGE:
		{
			PROFILE_BEGIN2("Environment Render");
			DxEnvironment::GetInstance().Render( m_pd3dDevice, NULL, &m_LandCharSlt );
			PROFILE_END2("Environment Render");

			m_LandCharSlt.Render ( m_pd3dDevice, CV );

			//	Note : 캐릭터 선택 오브젝트.
			//
			if( m_fRenderSkipTime > 1.0f )
			{
				for ( int i=0; i<MAX_SELECTCHAR; i++ )
				{
					m_CharSham[i].Render ( m_pd3dDevice, CV, FALSE );
					m_CharSham[i].RenderShadow ( m_pd3dDevice );
				}

				m_bRenderSkip = FALSE;
			}

			m_LandCharSlt.Render_EFF ( m_pd3dDevice, CV );
			m_LandCharSlt.RenderPickAlpha( m_pd3dDevice );

			DxWeatherMan::GetInstance()->Render ( m_pd3dDevice );
		}break;
	
	//userpanel
	case SELECT_CHARACTER_USERPANEL:
	case CHANGEPASS_PAGE:
	case CHANGEPIN_PAGE:
	case CHANGEEMAIL_PAGE:
	case TOPUP_PAGE:
	case RESETPIN_PAGE:
	case GAMETIMECVT_PAGE:
	case PREMIUMPOINTCVT_PAGE:
		{
			PROFILE_BEGIN2("Environment Render");
			DxEnvironment::GetInstance().Render( m_pd3dDevice, NULL, &m_LandCharSlt );
			PROFILE_END2("Environment Render");

			m_LandCharSlt.Render ( m_pd3dDevice, CV );

			//	Note : 캐릭터 선택 오브젝트.
			//
			if( m_fRenderSkipTime > 1.0f )
			{
				for ( int i=0; i<MAX_SELECTCHAR; i++ )
				{
					m_CharSham[i].Render ( m_pd3dDevice, CV, FALSE );
					m_CharSham[i].RenderShadow ( m_pd3dDevice );
				}

				m_bRenderSkip = FALSE;
			}

			m_LandCharSlt.Render_EFF ( m_pd3dDevice, CV );
			m_LandCharSlt.RenderPickAlpha( m_pd3dDevice );

			DxWeatherMan::GetInstance()->Render ( m_pd3dDevice );
		}
		break;


	case CHARACTER_CREATE_PAGE:
	case CHARACTER_CREATE_INFO:
	case CHARACTER_CREATE_ROTATE:
		{
			PROFILE_BEGIN2("Environment Render");
			DxEnvironment::GetInstance().Render( m_pd3dDevice, NULL, &m_LandCreate );
			PROFILE_END2("Environment Render");

			m_LandCreate.Render ( m_pd3dDevice, CV );

			if( m_fRenderSkipTime > 1.0f ){
				if ( m_bStyle ){
				m_NewCharSham.Render ( m_pd3dDevice, CV, FALSE );
				}

				m_bRenderSkip = FALSE;
			}

			m_LandCreate.Render_EFF ( m_pd3dDevice, CV );
			m_LandCreate.RenderPickAlpha( m_pd3dDevice );
			DxWeatherMan::GetInstance()->Render ( m_pd3dDevice );
		}break;
	};

	if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX )		m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE, FALSE );

	PROFILE_BEGIN("DxEffGroupPlayer::Render");
	DxEffGroupPlayer::GetInstance().Render ( m_pd3dDevice );
	PROFILE_END("DxEffGroupPlayer::Render");

	// Note : 최적화한 Eff
	OPTMManager::GetInstance().Render( m_pd3dDevice );

	//	Note : 큐브 텍스쳐 렌더링..!!
	DxCubeMap::GetInstance().Render ( m_pd3dDevice );

	//	Note : 글로우 처리를 하기위해 사용 한다.
	//
	DxGlowMan::GetInstance().RenderTex ( m_pd3dDevice );
	DxGlowMan::GetInstance().Render ( m_pd3dDevice );
	DxGlowMan::GetInstance().RenderBURN( m_pd3dDevice );

	//	Note : Wave 되야 하는것을 저장후 마지막으로 뿌린다.
	//
	PROFILE_BEGIN("DxEnvironment::UserToRefraction()");
	DxEnvironment::GetInstance().UserToRefraction ( m_pd3dDevice );
	PROFILE_END("DxEnvironment::UserToRefraction()");

	PROFILE_BEGIN("DxEnvironment::RenderWave()");
	DxEnvironment::GetInstance().RenderWave ( m_pd3dDevice, DxEffectMan::GetInstance().GetBumpTexture() );
	PROFILE_END("DxEnvironment::RenderWave()");

	// Note : 후처리
	DxPostProcess::GetInstance().Render( m_pd3dDevice );

	PROFILE_BEGIN("COuterInterface::Render");
	if( m_bUseInterface )
	{
		COuterInterface::GetInstance().Render ( m_pd3dDevice );
	}
	PROFILE_END("COuterInterface::Render");

	return S_OK;
}

void DxLobyStage::MsgProcess ( NET_MSG_GENERIC* nmg )
{
	CString str;

	switch ( nmg->nType )
	{
	case NET_MSG_LOGIN_FB:
	//case CHINA_NET_MSG_REGISTER_FB:
	case CHINA_NET_MSG_PASS_FB: //userpanel
	case CHINA_NET_MSG_PIN_FB:
	case CHINA_NET_MSG_EMAIL_FB:
	case CHINA_NET_MSG_GT2VP_FB:
	case CHINA_NET_MSG_TOPUP_FB:
		/*security key on charselect, DevArt22*/
	case CHINA_NET_MSG_CHARSELECT_PASS_FB:
	case CHINA_NET_MSG_RESETPASS_FB:
	case CHINA_NET_MSG_RESETPASS2_FB:
	case CHINA_NET_MSG_RESETPIN_FB:
	case CHINA_NET_MSG_PP2VP_FB:
	case NET_MSG_PASSCHECK_FB:
	case JAPAN_NET_MSG_UUID:
	case NET_MSG_REGISTER_ACTION_FB: /*register page, Juver, 2017/11/18 */
		{
			COuterInterface::GetInstance().MsgProcess ( nmg );
		}
		break;

	case NET_MSG_CHA_BAINFO:
		{
			NET_CHA_BBA_INFO* ncbi = (NET_CHA_BBA_INFO*) nmg;

			m_nStartCharNum = m_nChaSNum = ncbi->nChaSNum;

			for ( int i=0; i<m_nChaSNum; i++ )
			{
				m_nChaIDs[i] = ncbi->nChaNum[i];
				DxGlobalStage::GetInstance().GetNetClient()->SndChaBasicInfo ( m_nChaIDs[i] );
			}

			//no character to select move to create page instead
			if ( GetChaSNum() == 0 && COuterInterface::GetInstance().GetCharRemain() > 0 ){
				COuterInterface::GetInstance().ToCreateCharacterPage( SELECT_CHARACTER_PAGE );
			}
		}
		break;

	case NET_MSG_LOBBY_CHAR_SEL:
		{
			GLMSG::SNETLOBBY_CHARINFO* snci = (GLMSG::SNETLOBBY_CHARINFO*) nmg;
		
			BOOL bInserted = FALSE;

			//	Note : 기존 캐릭터 정보 수신시.
			for ( int i=0; i<m_nChaSNum; i++ )
			{
				if ( m_nChaIDs[i] == snci->Data.m_dwCharID )
				{
					bInserted = TRUE;
					m_CharInfo[i] = snci->Data;

					if( i < MAX_SELECTCHAR )
						m_CharSham[i].SetCharInfo( m_CharInfo[i], FALSE );

					++m_nStartCharLoad;
					break;
				}
			}

			//	Note : 새로이 생성된 캐릭터 정보 수신시.
			if ( !bInserted )
			{
				if ( m_nChaSNum < MAX_SERVERCHAR )
				{
					m_nChaIDs[m_nChaSNum] = snci->Data.m_dwCharID;
					m_CharInfo[m_nChaSNum] = snci->Data;
					m_CharSham[m_nChaSNum%MAX_SELECTCHAR].SetCharInfo( m_CharInfo[m_nChaSNum], FALSE );

					++m_nChaSNum;

					ShiftRBDown();
				}
			}
		}
		break;

	case NET_MSG_CHA_DEL_FB_OK:
		{
			COuterInterface::GetInstance().SetLobyStage( this );
		}
	case NET_MSG_CHA_DEL_FB_CLUB:
	case NET_MSG_CHA_DEL_FB_ERROR:
	case NET_MSG_CHA_NEW_FB:
	case NET_MSG_LOBBY_CHAR_JOIN_FB:
	case NET_MSG_LOBBY_CHINA_ERROR:
		{
			m_bGameJoin = FALSE;

			COuterInterface::GetInstance().MsgProcess ( nmg );

			// 에러일 경우 재접속 방지
			if( nmg->nType == NET_MSG_LOBBY_CHAR_JOIN_FB )
			{
				GLMSG::SNETLOBBY_CHARJOIN_FB *pNetMsg = (GLMSG::SNETLOBBY_CHARJOIN_FB *) nmg;
				if( pNetMsg->emCharJoinFB == EMCJOIN_FB_ERROR )
					m_bGameJoin = TRUE;
			}
			
		}
		break;

	default:
		CDebugSet::ToListView ( "DxLobyStage::MsgProcess() 분류되지 않은 메시지 수신. TYPE[%d]", nmg->nType );
		break;
	};
}


