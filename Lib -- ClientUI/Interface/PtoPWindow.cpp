#include "StdAfx.h"
#include "PtoPWindow.h"
#include "./BasicButton.h"
#include "InnerInterface.h"
#include "GLGaeaClient.h"
#include "DxGlobalStage.h"
#include "ModalWindow.h"
#include "./BasicTextBox.h"
#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "UITextControl.h"
#include "GameTextControl.h"
#include "ModalCallerID.h"
#include "ConftModalWindow.h"
#include "PartyModalWindow.h"
#include "BasicVarTextBox.h"
#include "BasicChat.h"

#include "RANPARAM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPtoPWindow::CPtoPWindow ()
	: m_bSetThisFrame(FALSE)
	, m_pNameBox(NULL)
	, m_pInfo(NULL)
    , m_nOverMenu(NO_ID)
	, m_pGuildButton(NULL)
	, m_pUnionButton ( NULL )
	, m_pGuildBattleButton ( NULL )
	, m_pUnionBattleButton ( NULL )
	, m_pRideButton ( NULL )
{
}

CPtoPWindow::~CPtoPWindow ()
{
}

void CPtoPWindow::ResizeControl ( char* szWindowKeyword )
{
    CUIControl TempControl;
	TempControl.Create ( 1, szWindowKeyword );
	const UIRECT& rcParentOldPos = GetLocalPos ();
	const UIRECT& rcParentNewPos = TempControl.GetLocalPos ();
	AlignSubControl ( rcParentOldPos, rcParentNewPos );

	SetLocalPos ( D3DXVECTOR2 ( rcParentNewPos.left, rcParentNewPos.top ) );
}

void CPtoPWindow::CreateSubControl ()
{
	CreateTitle ( "PTOPMENU_WINDOW_TITLE", "PTOPMENU_WINDOW_TITLE_LEFT", "PTOPMENU_WINDOW_TITLE_MID", "PTOPMENU_WINDOW_TITLE_RIGHT", "PTOPMENU_CHARNAME", "" );
//	CreateCloseButton ( "PTOPMENU_WINDOW_CLOSE", "PTOPMENU_WINDOW_CLOSE_F", "PTOPMENU_WINDOW_CLOSE_OVER" );
	CreateBody ( "PTOPMENU_WINDOW_BODY", "PTOPMENU_WINDOW_BODY_LEFT", "PTOPMENU_WINDOW_BODY_UP", "PTOPMENU_WINDOW_BODY_MAIN", "PTOPMENU_WINDOW_BODY_DOWN", "PTOPMENU_WINDOW_BODY_RIGHT" );
	ResizeControl ( "PTOPMENU_WINDOW" );

    WORD wFlag = GetAlignFlag ();
	wFlag &= ~UI_FLAG_XSIZE;
	wFlag &= ~UI_FLAG_YSIZE;
	SetAlignFlag ( wFlag );

	CreateFlipButton ( "PTOPMENU_TRADE", "PTOPMENU_TRADE_F", "PTOPMENU_TRADE_OVER",	PTOPMENU_TRADE );
	CreateFlipButton ( "PTOPMENU_WHISPER", "PTOPMENU_WHISPER_F", "PTOPMENU_WHISPER_OVER", PTOPMENU_WHISPER );
	CreateFlipButton ( "PTOPMENU_PARTY", "PTOPMENU_PARTY_F", "PTOPMENU_PARTY_OVER",	PTOPMENU_PARTY );
	CreateFlipButton ( "PTOPMENU_CONFLICT", "PTOPMENU_CONFLICT_F", "PTOPMENU_CONFLICT_OVER", PTOPMENU_CONFLICT );
	CreateFlipButton ( "PTOPMENU_FRIEND", "PTOPMENU_FRIEND_F", "PTOPMENU_FRIEND_OVER", PTOPMENU_FRIEND );

	/*charinfoview , Juver, 2017/11/11 */
	CreateFlipButton ( "PTOPMENU_REFER_CHAR", "PTOPMENU_REFER_CHAR_F", "PTOPMENU_REFER_CHAR_OVER", PTOPMENU_REFER_CHAR );
	
	m_pGuildButton = CreateFlipButton( "PTOPMENU_GUILD", "PTOPMENU_GUILD_F", "PTOPMENU_GUILD_OVER", PTOPMENU_GUILD );
	m_pGuildButton->SetVisibleSingle( FALSE );
	m_pUnionButton = CreateFlipButton( "PTOPMENU_UNION", "PTOPMENU_UNION_F", "PTOPMENU_UNION_OVER", PTOPMENU_UNION );
	m_pUnionButton->SetVisibleSingle( FALSE );
	m_pGuildBattleButton = CreateFlipButton( "PTOPMENU_GUILD_BATTLE", "PTOPMENU_GUILD_BATTLE_F", "PTOPMENU_GUILD_BATTLE_OVER", PTOPMENU_GUILD_BATTLE );
	m_pGuildBattleButton->SetVisibleSingle( FALSE );
	m_pUnionBattleButton = CreateFlipButton( "PTOPMENU_UNION_BATTLE", "PTOPMENU_UNION_BATTLE_F", "PTOPMENU_UNION_BATTLE_OVER", PTOPMENU_UNION_BATTLE );
	m_pUnionBattleButton->SetVisibleSingle( FALSE );

	m_pRideButton = CreateFlipButton( "PTOPMENU_BIKE_BUTTON", "PTOPMENU_BIKE_BUTTON_F", "PTOPMENU_BIKE_BUTTON_OVER", PTOPMENU_BIKE );
	m_pRideButton->SetVisibleSingle( FALSE );

	CBasicVarTextBox* pInfo = new CBasicVarTextBox;
	pInfo->CreateSub ( this, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pInfo->CreateSubControl ();
	pInfo->SetVisibleSingle ( FALSE );
	RegisterControl ( pInfo );
	m_pInfo = pInfo;
}

CBasicButton* CPtoPWindow::CreateFlipButton ( char* szButton, char* szButtonFlip, char* szButtonOver, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::CLICK_FLIP );
	pButton->CreateMouseOver ( szButtonOver );
	RegisterControl ( pButton );
	return pButton;
}

void CPtoPWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case PTOPMENU_TRADE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( ID2GAMEWORD ("P2PMENU",0), NS_UITEXTCOLOR::SILVER );							
				}
				m_pInfo->SetVisibleSingle ( TRUE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( GLGaeaClient::GetInstance().GetCharacter()->m_sPMarket.IsOpen() )
					{
						CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("NO_TRADE_PRIVATE_MARKET_OPEN") );
					}
					else
					{
						GLMSG::SNET_TRADE NetMsg;
						NetMsg.dwTargetID = m_dwGaeaID;
						NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
					}
				}
			}
		}
		break;

	case PTOPMENU_WHISPER:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( ID2GAMEWORD ("P2PMENU",1), NS_UITEXTCOLOR::SILVER );	
				}
				m_pInfo->SetVisibleSingle ( TRUE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					CInnerInterface::GetInstance().GetChat ()->BEGIN_PRIVATE_CHAT ( m_szPlayerName );
					CInnerInterface::GetInstance().ADD_FRIEND_NAME_TO_EDITBOX ( m_szPlayerName );
				}
			}
		}
		break;

	case PTOPMENU_PARTY:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( ID2GAMEWORD ("P2PMENU",2), NS_UITEXTCOLOR::SILVER );	
				}
				m_pInfo->SetVisibleSingle ( TRUE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{			
					CString strTemp = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("PARTY_QUESTION"), m_szPlayerName.GetString() );
					DoPartyModal ( strTemp, MODAL_PARTY_REQ );
				}
			}
		}
		break;

	case PTOPMENU_CONFLICT:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( ID2GAMEWORD ("P2PMENU",3), NS_UITEXTCOLOR::SILVER );	
				}
				m_pInfo->SetVisibleSingle ( TRUE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter ();
					CString strMyName = pCharacter->GetName ();
					EMCONFT_TYPE emTYPE = pCharacter->GetConftType ( m_dwGaeaID );
					UIGUID MODAL_CONFT_TYPE = NO_ID;

					CString strCombine;
					switch ( emTYPE )
					{
					case EMCONFT_NONE:
						{
//							GASSERT ( 0 && "오류." );
							return ;
						}
						break;

					case EMCONFT_ONE:
						{
							strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("CONFLICT_QUESTION"), m_szPlayerName.GetString() );
							MODAL_CONFT_TYPE = MODAL_CONFLICT_ONE_REQ;
						}
						break;

					case EMCONFT_PARTY:
						{
							strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("CONFLICT_QUESTION_PARTY"), m_szPlayerName.GetString() );
							MODAL_CONFT_TYPE = MODAL_CONFLICT_PARTY_REQ;
						}
						break;

					case EMCONFT_GUILD:
						{
							strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("CONFLICT_QUESTION_GUILD"), m_szPlayerName.GetString() );
							MODAL_CONFT_TYPE = MODAL_CONFLICT_GUILD_REQ;
						}
						break;
					}

					DoConftModal ( strCombine, MODAL_CONFT_TYPE, strMyName, m_szPlayerName );
				}
			}
		}
		break;

	case PTOPMENU_FRIEND:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( ID2GAMEWORD ("P2PMENU",4), NS_UITEXTCOLOR::SILVER );	
				}
				m_pInfo->SetVisibleSingle ( TRUE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					CInnerInterface::GetInstance().ADD_FRIEND ( m_szPlayerName );
				}
			}
		}
		break;

	case PTOPMENU_GUILD:
		{
			if ( !m_pRideButton->IsVisible() )
			{
				if ( CHECK_MOUSE_IN ( dwMsg ) )
				{
					if ( m_nOverMenu != ControlID )
					{
						m_pInfo->SetTextNoSplit ( ID2GAMEWORD ("P2PMENU",5), NS_UITEXTCOLOR::SILVER );	
					}
					m_pInfo->SetVisibleSingle ( TRUE );
					m_nOverMenu = ControlID;

					if ( dwMsg & UIMSG_LB_UP )
					{
						//	모달띄우기
						CString strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("CLUB_JOIN"), m_szPlayerName );
						DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_CLUB_JOIN );					
					}
				}
			}
		}
		break;

	case PTOPMENU_UNION:
		{
			if ( !m_pRideButton->IsVisible() )
			{
				if ( CHECK_MOUSE_IN ( dwMsg ) )
				{
					if ( m_nOverMenu != ControlID )
					{
						m_pInfo->SetTextNoSplit ( ID2GAMEWORD ("P2PMENU",6), NS_UITEXTCOLOR::SILVER );	
					}
					m_pInfo->SetVisibleSingle ( TRUE );
					m_nOverMenu = ControlID;

					if ( dwMsg & UIMSG_LB_UP )
					{
						//	모달띄우기
						CString strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("UNION_JOIN"), m_szPlayerName );
						DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_UNION_JOIN );					
					}
				}
			}
		}
		break;
	case PTOPMENU_GUILD_BATTLE:
		{
			if ( !m_pRideButton->IsVisible() )
			{
				if ( CHECK_MOUSE_IN ( dwMsg ) )
				{
					if ( m_nOverMenu != ControlID )
					{
						m_pInfo->SetTextNoSplit ( ID2GAMEWORD ("P2PMENU",7), NS_UITEXTCOLOR::SILVER );	
					}
					m_pInfo->SetVisibleSingle ( TRUE );
					m_nOverMenu = ControlID;

					if ( dwMsg & UIMSG_LB_UP )
					{
						PGLCHARCLIENT pCharClient = GLGaeaClient::GetInstance().GetChar( m_dwGaeaID );
						if ( !pCharClient )	return;
						//	모달띄우기
						CInnerInterface::GetInstance().DoBattleModal( pCharClient->GetClubName() );
					}
				}
			}
		}
		break;
	case PTOPMENU_UNION_BATTLE:
		{
			if ( !m_pRideButton->IsVisible() )
			{
				if ( CHECK_MOUSE_IN ( dwMsg ) )
				{
					if ( m_nOverMenu != ControlID )
					{
						m_pInfo->SetTextNoSplit ( ID2GAMEWORD ("P2PMENU",8), NS_UITEXTCOLOR::SILVER );	
					}
					m_pInfo->SetVisibleSingle ( TRUE );
					m_nOverMenu = ControlID;

					if ( dwMsg & UIMSG_LB_UP )
					{
						PGLCHARCLIENT pCharClient = GLGaeaClient::GetInstance().GetChar( m_dwGaeaID );
						if ( !pCharClient )	return;
						//	모달띄우기
						CInnerInterface::GetInstance().DoBattleModal( pCharClient->GetClubName(), true );
					}
				}
			}
		}
		break;

		/*charinfoview , Juver, 2017/11/11 */
	case PTOPMENU_REFER_CHAR:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( ID2GAMEWORD ("P2PMENU",10), NS_UITEXTCOLOR::SILVER );	
				}
				m_pInfo->SetVisibleSingle ( TRUE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					GLGaeaClient::GetInstance().GetCharacter()->RequestCharacterInfo( m_dwGaeaID );
				}
			}
		}
		break;
	case PTOPMENU_BIKE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_nOverMenu != ControlID )
				{
					m_pInfo->SetTextNoSplit ( ID2GAMEWORD ("P2PMENU",11), NS_UITEXTCOLOR::SILVER );	
				}
				m_pInfo->SetVisibleSingle ( TRUE );
				m_nOverMenu = ControlID;

				if ( dwMsg & UIMSG_LB_UP )
				{
					PGLCHARCLIENT pCharClient = GLGaeaClient::GetInstance().GetChar( m_dwGaeaID );
					if ( !pCharClient )	return;
					{
						GLMSG::SNET_PASSENGER NetMsg;
						NetMsg.dwTargetID = m_dwGaeaID;
						NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
					}
				}
			}
		}
		break;
	}
}

void CPtoPWindow::SetPtoPMenu( PGLCHARCLIENT pCharClient, DWORD dwGaeaID, GLCLUB & sClub )
{
	GASSERT( pCharClient );

	m_szPlayerName = pCharClient->GetName();
	m_dwGaeaID = dwGaeaID;

	SetTitleName ( m_szPlayerName ); 

	m_bSetThisFrame = FALSE;
	m_nOverMenu = NO_ID;
	
	DWORD dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;


	if( sClub.IsMemberFlgJoin(dwCharID) )
	{
		FLOAT fSizeX(0.0);
		
		if( GLGaeaClient::GetInstance().GetCharacter()->m_bVehicle )
		{
			if ( GLGaeaClient::GetInstance().GetCharacter()->m_sVehicle.m_emTYPE != VEHICLE_TYPE_BOARD )
			{
				if ( !m_pRideButton->IsVisible () )
				{
					m_pRideButton->SetVisibleSingle ( TRUE );
				}
			}
		}else m_pRideButton->SetVisibleSingle ( FALSE );

		if ( !m_pGuildButton->IsVisible () )
		{
			m_pGuildButton->SetVisibleSingle ( TRUE );
			const UIRECT& rcButtonGlobalPos = m_pGuildButton->GetGlobalPos ();
			fSizeX += rcButtonGlobalPos.sizeX;
		}

//		if( GLCONST_CHAR::bTESTSERVER )
//		동맹 메뉴
		{
			if( sClub.IsMaster(dwCharID) && pCharClient->IsClubMaster() )
			{
				if ( !sClub.IsAlliance() || sClub.IsChief() )
				{
					if( !m_pUnionButton->IsVisible() )
					{
						m_pUnionButton->SetVisibleSingle( TRUE );
						const UIRECT& rcButtonGlobalPos = m_pGuildButton->GetGlobalPos ();
						fSizeX += rcButtonGlobalPos.sizeX;
					}
				}
			}
			else
			{
				if( m_pUnionButton->IsVisible() )
				{
					m_pUnionButton->SetVisibleSingle( FALSE );
					const UIRECT& rcButtonGlobalPos = m_pGuildButton->GetGlobalPos ();
//					GASSERT( fSizeX == 0 );
					fSizeX -= rcButtonGlobalPos.sizeX;
				}
			}
		}

		// 클럽 배틀 메뉴
		{
			if( GLCONST_CHAR::bCLUB_BATTLE && sClub.IsMaster(dwCharID) && pCharClient->IsClubMaster() )
			{
				if( !m_pGuildBattleButton->IsVisible() )
				{
					m_pGuildBattleButton->SetVisibleSingle( TRUE );
					
					const UIRECT& rcButtonGlobalPos = m_pGuildBattleButton->GetGlobalPos ();
					fSizeX += rcButtonGlobalPos.sizeX;					
				}

				
				const UIRECT& rcUnionLocalPos = m_pUnionButton->GetLocalPos ();
				UIRECT rcBattleLocalPos = m_pGuildBattleButton->GetLocalPos();					

				if ( !m_pUnionButton->IsVisible() )
				{
					rcBattleLocalPos.left = rcUnionLocalPos.left;
					rcBattleLocalPos.right = rcUnionLocalPos.right;
					m_pGuildBattleButton->SetLocalPos( rcBattleLocalPos );
				}
				else
				{
					rcBattleLocalPos.left = rcUnionLocalPos.left + rcUnionLocalPos.sizeX;
					rcBattleLocalPos.right = rcBattleLocalPos.left + rcUnionLocalPos.sizeX;
					m_pGuildBattleButton->SetLocalPos( rcBattleLocalPos );
				}
			}
			else
			{
				if( m_pGuildBattleButton->IsVisible() )
				{
					m_pGuildBattleButton->SetVisibleSingle( FALSE );
					
					const UIRECT& rcButtonGlobalPos = m_pUnionButton->GetGlobalPos ();
//					GASSERT( fSizeX == 0 );
					fSizeX -= rcButtonGlobalPos.sizeX;					
				}
			}
		}
		
		//	동맹 배틀 메뉴
		{
			if ( GLCONST_CHAR::bCLUB_BATTLE_ALLIANCE 
				&& sClub.IsChief() && pCharClient->IsAllianceMaster() ) 
			{
				if( !m_pUnionBattleButton->IsVisible() )
				{
					m_pUnionBattleButton->SetVisibleSingle( TRUE );
					
					const UIRECT& rcButtonGlobalPos = m_pUnionBattleButton->GetGlobalPos ();
					fSizeX += rcButtonGlobalPos.sizeX;					
				}

				
				const UIRECT& rcLeftLocalPos = m_pGuildBattleButton->GetLocalPos ();
				UIRECT rcBattleLocalPos = m_pUnionBattleButton->GetLocalPos();					

				rcBattleLocalPos.left = rcLeftLocalPos.left + rcLeftLocalPos.sizeX;
				rcBattleLocalPos.right = rcBattleLocalPos.left + rcLeftLocalPos.sizeX;
				m_pUnionBattleButton->SetLocalPos( rcBattleLocalPos );

			}
			else
			{
				if( m_pUnionBattleButton->IsVisible() )
				{
					m_pUnionBattleButton->SetVisibleSingle( FALSE );
					
					const UIRECT& rcButtonGlobalPos = m_pUnionButton->GetGlobalPos ();
//					GASSERT( fSizeX == 0 );
					fSizeX -= rcButtonGlobalPos.sizeX;					
				}
			}
		}

		WORD wFlagBack = GetAlignFlag ();
		SetAlignFlag ( UI_FLAG_XSIZE );

		const UIRECT& rcGlobalPosOld = GetGlobalPos ();
		const UIRECT& rcLocalPosOld = GetLocalPos ();

		UIRECT rcGlobalPosNew = UIRECT( rcGlobalPosOld.left, rcGlobalPosOld.top,
										rcGlobalPosOld.sizeX + fSizeX, rcGlobalPosOld.sizeY );

		AlignSubControl ( rcGlobalPosOld, rcGlobalPosNew );

		SetLocalPos ( D3DXVECTOR2 ( rcLocalPosOld.left, rcLocalPosOld.top ) );
		SetGlobalPos ( rcGlobalPosNew);

		SetAlignFlag ( wFlagBack );
	}
	else
	{
		FLOAT fSizeX(0.0);
		FLOAT fSizeXRide(0.0);

		if ( m_pRideButton->IsVisible () )
		{
			m_pRideButton->SetVisibleSingle ( FALSE );
			const UIRECT& rcButtonGlobalPos = m_pRideButton->GetGlobalPos();
			fSizeX += rcButtonGlobalPos.sizeX;
		}

		if ( m_pGuildButton->IsVisible () )
		{
			m_pGuildButton->SetVisibleSingle ( FALSE );
			const UIRECT& rcButtonGlobalPos = m_pGuildButton->GetGlobalPos();
			fSizeX += rcButtonGlobalPos.sizeX;
		}

		if ( m_pUnionButton->IsVisible () )
		{
			m_pUnionButton->SetVisibleSingle ( FALSE );
			const UIRECT& rcButtonGlobalPos = m_pUnionButton->GetGlobalPos ();
			fSizeX += rcButtonGlobalPos.sizeX;
		}

		if ( m_pGuildBattleButton->IsVisible () )
		{
			m_pGuildBattleButton->SetVisibleSingle ( FALSE );
			const UIRECT& rcButtonGlobalPos = m_pGuildBattleButton->GetGlobalPos ();
			fSizeX += rcButtonGlobalPos.sizeX;
		}

		if ( m_pUnionBattleButton->IsVisible () )
		{
			m_pUnionBattleButton->SetVisibleSingle ( FALSE );
			const UIRECT& rcButtonGlobalPos = m_pUnionBattleButton->GetGlobalPos ();
			fSizeX += rcButtonGlobalPos.sizeX;
		}
		

		if( GLGaeaClient::GetInstance().GetCharacter()->m_bVehicle )
		{
			if ( GLGaeaClient::GetInstance().GetCharacter()->m_sVehicle.m_emTYPE != VEHICLE_TYPE_BOARD )
			{
				if ( !m_pRideButton->IsVisible () )
				{
					m_pRideButton->SetVisibleSingle ( TRUE );
					const UIRECT& rcButtonGlobalPos = m_pRideButton->GetGlobalPos ();
					fSizeXRide += rcButtonGlobalPos.sizeX;
				}
			}
		}

		fSizeX -= fSizeXRide;

		WORD wFlagBack = GetAlignFlag ();
		SetAlignFlag ( UI_FLAG_XSIZE );

		const UIRECT& rcGlobalPosOld = GetGlobalPos ();
		const UIRECT& rcLocalPosOld = GetLocalPos ();

		UIRECT rcGlobalPosNew = UIRECT( rcGlobalPosOld.left, rcGlobalPosOld.top,
										rcGlobalPosOld.sizeX - fSizeX, rcGlobalPosOld.sizeY );

		AlignSubControl ( rcGlobalPosOld, rcGlobalPosNew );

		SetLocalPos ( D3DXVECTOR2 ( rcLocalPosOld.left, rcLocalPosOld.top ) );
		SetGlobalPos ( rcGlobalPosNew);

		SetAlignFlag ( wFlagBack );
	}
}

void CPtoPWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_pInfo->SetVisibleSingle ( FALSE );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	DWORD dwMsg = GetMessageEx ();
	if ( CHECK_LB_UP_LIKE ( dwMsg ) )
	{
//		if ( m_bSetThisFrame )
		{
			CInnerInterface::GetInstance().HideGroup ( GetWndID () );
		}
		m_bSetThisFrame = TRUE;
	}
}

CBasicTextBox* CPtoPWindow::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );
	RegisterControl ( pStaticText );
	return pStaticText;
}