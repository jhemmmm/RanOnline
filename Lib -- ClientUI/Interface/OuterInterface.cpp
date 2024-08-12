#include "StdAfx.h"
#include "OuterInterface.h"
#include "GameTextControl.h"
#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "UITextControl.h"
#include "./BasicTextBox.h"

#include "RANPARAM.h"
#include "SelectServerPage.h"
#include "SelectChinaAreaPage.h"
#include "LoginPage.h"
/*security key on charselect, DevArt22*/
#include "CharSelectPassPage.h"
#include "PassKeyboard.h"
#include "SelectCharacterPage.h"
#include "DxGlobalStage.h"
#include "ModalWindow.h"
#include "WaitDialogue.h"
#include "CommonWeb.h"
#include "SecPassSetPage.h"
#include "SecPassCheckPage.h"
#include "MessageWindow.h"
#include "CharacterCreatePage.h"
#include "CharacterCreateInfo.h"
#include "CharacterCreateRotate.h"
#include "LobbyTitle.h"
#include "RegisterPage.h" /*register page, Juver, 2017/11/17 */
#include "../Lib -- Engine/Common/SubPath.h"

//userpanel
#include "../Lib -- ClientUI/Interface/IngameUserPanel/ChangePinPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/ChangePasswordPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/ResetPasswordPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/ResetPasswordPage2.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/ResetPinPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/ChangeEmailPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/TopUpPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/GameTimeCVTPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/PremiumPointCVTPage.h"
#include "../Lib -- ClientUI/Interface/IngameUserPanel/SelectCharacterUserPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COuterInterface& COuterInterface::GetInstance()
{
	static COuterInterface Instance;
	return Instance;
}

const	float	COuterInterface::fLIST_LINE_INTERVAL = 5.0f;

CBasicScrollBarEx* g_pScrollBar = NULL;

COuterInterface::COuterInterface ()	:
	m_bUseWaitTime ( FALSE ),
	m_bBlockProgramFound ( false ),
	m_nServerGroup ( 0 ),
	m_nServerChannel ( 0 ),
	m_bCHANNEL ( false ),
	m_bLoginCancel( FALSE ),
	m_lResolutionBack( 0 ),
	m_uidMoalCallWindow(NO_ID),
	
	/*security key on charselect, DevArt22*/
	m_nCharPassAttempt(0)
{
	ResetControl();
}

COuterInterface::~COuterInterface ()
{
}

void COuterInterface::ResetControl()
{
	m_pModalWindow = NULL;
	m_pSelectServerPage = NULL;
	m_pLoginPage = NULL;
	//userpanel
	m_pGameTimeCVTPage = NULL;
	m_pPremiumPointCVTPage = NULL;
	m_pChangePasswordPage = NULL;
	m_pResetPasswordPage = NULL;
	m_pResetPasswordPage2 = NULL;
	m_pResetPinPage = NULL;
	m_pChangePinPage = NULL;
	m_pChangeEmailPage = NULL;
	m_pTopUpPage = NULL;
	
	/*security key on charselect, DevArt22*/
	m_pCharSelectPassPage = NULL;
	m_pSecPassSetPage = NULL;
	m_pSecPassCheckPage = NULL;
	m_pMessageWindow = NULL;
	m_pMessageWindowDummy = NULL;
	m_pPassKeyboard = NULL;
	m_pSelectCharacterPage = NULL;
	m_pSelectCharacterUserPanel = NULL;
	m_pFullScreenButton = NULL;
	m_pFullScreenButtonDummy = NULL;
	m_pUpImage = NULL;
	m_pDownImage = NULL;
	m_pLoginMark = NULL;
	m_pCharacterCreatePage = NULL;
	m_pCharacterCreateInfo = NULL;
	m_pCharacterCreateRotate = NULL;
	m_pLobbyTitle = NULL;
	m_pRegisterPage = NULL; /*register page, Juver, 2017/11/17 */

	m_pWaitDisplay = NULL;
	m_pDxLobyStage = NULL;
}

HRESULT COuterInterface::OneTimeSceneInit ()
{
	switch ( RANPARAM::emSERVICE_TYPE )
	{
	case EMSERVICE_INDONESIA:
	case EMSERVICE_MALAYSIA_CN:
	case EMSERVICE_MALAYSIA_EN:
	case EMSERVICE_THAILAND:
		CLoginPage::nLIMIT_ID = 20;
		CLoginPage::nLIMIT_PW = 20;
		m_bCHANNEL = true;
		break;
	case EMSERVICE_PHILIPPINES:
	case EMSERVICE_VIETNAM:
	case EMSERVICE_DEFAULT:
		CLoginPage::nLIMIT_ID = 14;
		CLoginPage::nLIMIT_PW = 14;
		m_bCHANNEL = true;
		break;
	case EMSERVICE_FEYA:
		CLoginPage::nLIMIT_ID = 20;
		CLoginPage::nLIMIT_PW = 20;
		m_bCHANNEL = true;
		break;
	case EMSERVICE_KOREA:
		CLoginPage::nLIMIT_ID = 12;
		CLoginPage::nLIMIT_PW = 12;
		m_bCHANNEL = true;
		break;
	case EMSERVICE_CHINA:
		CLoginPage::nLIMIT_ID = 12;
		CLoginPage::nLIMIT_PW = 19;
		m_bCHANNEL = true;
		break;

	case EMSERVICE_JAPAN:
		CLoginPage::nLIMIT_ID = 16;
		CLoginPage::nLIMIT_PW = 16;
		m_bCHANNEL = true;
		break;

	case EMSERVICE_GLOBAL:
		CLoginPage::nLIMIT_ID = 20;
		CLoginPage::nLIMIT_PW = 20;
		m_bCHANNEL = true;
		break;

	default:
		CLoginPage::nLIMIT_ID = 12;
		CLoginPage::nLIMIT_PW = 12;
		m_bCHANNEL = false;
		break;
	};

	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

#ifdef CH_PARAM
	{
		CSelectChinaAreaPage* pSelectChinaAreaPage = new CSelectChinaAreaPage;
		pSelectChinaAreaPage->CreateEx( SELECT_CHINA_AREA_PAGE, "OUTER_WINDOW" );
		pSelectChinaAreaPage->CreateBaseWidnow( "SELECT_AREA_PAGE", (char*)ID2GAMEWORD ( "SELECT_CHINA_AREA" ) );
		pSelectChinaAreaPage->CreateSubControl();
		pSelectChinaAreaPage->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( pSelectChinaAreaPage );	
		ShowGroupFocus ( SELECT_CHINA_AREA_PAGE );
	}
#endif

	{
		m_pSelectServerPage = new CSelectServerPage;
		m_pSelectServerPage->CreateEx ( SELECT_SERVER_PAGE, "OUTER_WINDOW" );
		m_pSelectServerPage->CreateBaseWidnow ( "SELECT_SERVER_PAGE", (char*) ID2GAMEWORD ( "SELECT_SERVER" ) );
		m_pSelectServerPage->CreateSubControl ();
		m_pSelectServerPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pSelectServerPage );	
		ShowGroupFocus ( SELECT_SERVER_PAGE );
	}

	{
		m_pLoginPage = new CLoginPage;
		m_pLoginPage->CreateEx ( LOGIN_PAGE, "OUTER_WINDOW" );
		m_pLoginPage->CreateBaseWidnow ( "LOGIN_PAGE", (char*) ID2GAMEWORD ( "LOGIN_PAGE" ) );
		m_pLoginPage->CreateSubControl ();
		m_pLoginPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pLoginPage );
		ShowGroupFocus ( LOGIN_PAGE );
	}

	//userpanel
	{
		m_pChangePinPage = new CChangePinPage;
		m_pChangePinPage->CreateEx ( CHANGEPIN_PAGE, "OUTER_WINDOW" );
		m_pChangePinPage->CreateBaseWidnow ( "CHANGEPIN_PAGE", (char*) ID2GAMEWORD ( "CHANGEPIN_PAGE" ) );
		m_pChangePinPage->CreateSubControl ();
		m_pChangePinPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pChangePinPage );
		ShowGroupFocus ( CHANGEPIN_PAGE );

	}

	{
		m_pChangePasswordPage = new CChangePasswordPage;
		m_pChangePasswordPage->CreateEx ( CHANGEPASS_PAGE, "OUTER_WINDOW" );
		m_pChangePasswordPage->CreateBaseWidnow ( "CHANGEPASS_PAGE", (char*) ID2GAMEWORD ( "CHANGEPASS_PAGE" ) );
		m_pChangePasswordPage->CreateSubControl ();
		m_pChangePasswordPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pChangePasswordPage );
		ShowGroupFocus ( CHANGEPASS_PAGE );

	}

	{
		m_pResetPasswordPage = new CResetPasswordPage;
		m_pResetPasswordPage->CreateEx ( RESETPASS_PAGE, "OUTER_WINDOW" );
		m_pResetPasswordPage->CreateBaseWidnow ( "RESETPASS_PAGE", (char*) ID2GAMEWORD ( "RESETPASS_PAGE" ) );
		m_pResetPasswordPage->CreateSubControl ();
		m_pResetPasswordPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pResetPasswordPage );
		ShowGroupFocus ( RESETPASS_PAGE );

	}
	
	{
		m_pResetPasswordPage2 = new CResetPasswordPage2;
		m_pResetPasswordPage2->CreateEx ( RESETPASS_PAGE2, "OUTER_WINDOW" );
		m_pResetPasswordPage2->CreateBaseWidnow ( "RESETPASS_PAGE2", (char*) ID2GAMEWORD ( "RESETPASS_PAGE2" ) );
		m_pResetPasswordPage2->CreateSubControl ();
		m_pResetPasswordPage2->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pResetPasswordPage2 );
		ShowGroupFocus ( RESETPASS_PAGE2 );

	}
	
	{
		m_pResetPinPage = new CResetPinPage;
		m_pResetPinPage->CreateEx ( RESETPIN_PAGE, "OUTER_WINDOW" );
		m_pResetPinPage->CreateBaseWidnow ( "RESETPIN_PAGE", (char*) ID2GAMEWORD ( "RESETPIN_PAGE" ) );
		m_pResetPinPage->CreateSubControl ();
		m_pResetPinPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pResetPinPage );
		ShowGroupFocus ( RESETPIN_PAGE );

	}

	{
		m_pChangeEmailPage = new CChangeEmailPage;
		m_pChangeEmailPage->CreateEx ( CHANGEEMAIL_PAGE, "OUTER_WINDOW" );
		m_pChangeEmailPage->CreateBaseWidnow ( "CHANGEEMAIL_PAGE", (char*) ID2GAMEWORD ( "CHANGEEMAIL_PAGE" ) );
		m_pChangeEmailPage->CreateSubControl ();
		m_pChangeEmailPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pChangeEmailPage );
		ShowGroupFocus ( CHANGEEMAIL_PAGE );

	}

	{
		m_pTopUpPage = new CTopUpPage;
		m_pTopUpPage->CreateEx ( TOPUP_PAGE, "OUTER_WINDOW" );
		m_pTopUpPage->CreateBaseWidnow ( "TOPUP_PAGE", (char*) ID2GAMEWORD ( "TOPUP_PAGE" ) );
		m_pTopUpPage->CreateSubControl ();
		m_pTopUpPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pTopUpPage );
		ShowGroupFocus ( TOPUP_PAGE );

	}
	
	/*security key on charselect, DevArt22*/
	{
		m_pCharSelectPassPage = new CCharSelectPassPage;
		m_pCharSelectPassPage->CreateEx ( CHARSELECT_PASS_PAGE, "OUTER_WINDOW" );
		m_pCharSelectPassPage->CreateBaseWidnow ( "CHARSELECT_PASS_PAGE", (char*) ID2GAMEWORD ( "CHARSELECT_PASS_PAGE" ) );
		m_pCharSelectPassPage->CreateSubControl ();
		m_pCharSelectPassPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_TOP );
		RegisterControl ( m_pCharSelectPassPage );
		ShowGroupFocus ( CHARSELECT_PASS_PAGE );

	}

	{
		m_pGameTimeCVTPage = new CGameTimeCVTPage;
		m_pGameTimeCVTPage->CreateEx ( GAMETIMECVT_PAGE, "OUTER_WINDOW" );
		m_pGameTimeCVTPage->CreateBaseWidnow ( "GAMETIMECVT_PAGE", (char*) ID2GAMEWORD ( "GAMETIMECVT_PAGE" ) );
		m_pGameTimeCVTPage->CreateSubControl ();
		m_pGameTimeCVTPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pGameTimeCVTPage );
		ShowGroupFocus ( GAMETIMECVT_PAGE );

	}

	{
		m_pPremiumPointCVTPage = new CPremiumPointCVTPage;
		m_pPremiumPointCVTPage->CreateEx ( PREMIUMPOINTCVT_PAGE, "OUTER_WINDOW" );
		m_pPremiumPointCVTPage->CreateBaseWidnow ( "PREMIUMPOINT_PAGE", (char*) ID2GAMEWORD ( "PREMIUMPOINTCVT_PAGE" ) );
		m_pPremiumPointCVTPage->CreateSubControl ();
		m_pPremiumPointCVTPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPremiumPointCVTPage );
		ShowGroupFocus ( PREMIUMPOINTCVT_PAGE );
	}
	//userpanel end

/*
//#if defined(KRT_PARAM) || defined(KR_PARAM)
#if defined ( JP_PARAM )	// 2차 패스워드
	{
		m_pSecPassSetPage = new CSecPassSetPage;
		m_pSecPassSetPage->CreateEx ( SECPASS_SETPAGE, "OUTER_WINDOW" );
		m_pSecPassSetPage->CreateBaseWidnow ( "SECPASS_SETPAGE", (char*) ID2GAMEWORD ( "SECPASS_SETPAGE" ) );
		m_pSecPassSetPage->CreateSubControl ();
		m_pSecPassSetPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pSecPassSetPage );
		ShowGroupFocus ( SECPASS_SETPAGE );
	}

	{
		m_pSecPassCheckPage = new CSecPassCheckPage;
		m_pSecPassCheckPage->CreateEx ( SECPASS_CHECKPAGE, "OUTER_WINDOW" );
		m_pSecPassCheckPage->CreateBaseWidnow ( "SECPASS_CHECKPAGE", (char*) ID2GAMEWORD ( "SECPASS_CHECKPAGE" ) );
		m_pSecPassCheckPage->CreateSubControl ();
		m_pSecPassCheckPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pSecPassCheckPage );
		ShowGroupFocus ( SECPASS_CHECKPAGE );
	}
#endif
*/

	{

		m_pMessageWindow = new CMessageWindow;
		m_pMessageWindow->Create ( MESSAGE_WINDOW, "BASIC_MESSAGE_DISPLAY", UI_FLAG_CENTER_X );
		m_pMessageWindow->CreateSubControl ();
		RegisterControl ( m_pMessageWindow );
		ShowGroupTop ( MESSAGE_WINDOW );
		HideGroup ( MESSAGE_WINDOW );

		m_pMessageWindowDummy = new CUIControl;
		m_pMessageWindowDummy->Create ( MESSAGE_WINDOW_DUMMY, "BASIC_MESSAGE_DISPLAY", UI_FLAG_CENTER_X );
        RegisterControl ( m_pMessageWindowDummy );


	}

	{
		m_pPassKeyboard = new CPassKeyboard;
		m_pPassKeyboard->Create( PASS_KEYBOARD, "PASS_KEYBOARD", UI_FLAG_CENTER_X );
		m_pPassKeyboard->CreateSubControl();
		RegisterControl( m_pPassKeyboard );
		ShowGroupFocus( PASS_KEYBOARD );
	}

	{
		m_pSelectCharacterPage = new CSelectCharacterPage;
		m_pSelectCharacterPage->Create ( SELECT_CHARACTER_PAGE, "SELECT_CHARACTER_PAGE" );
		m_pSelectCharacterPage->CreateSubControl ();
		m_pSelectCharacterPage->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		RegisterControl ( m_pSelectCharacterPage );
		ShowGroupFocus ( SELECT_CHARACTER_PAGE );

		m_pSelectCharacterUserPanel = new CSelectCharacterUserPanel;
		m_pSelectCharacterUserPanel->Create ( SELECT_CHARACTER_USERPANEL, "SELECT_CHARACTER_USERPANEL" );
		m_pSelectCharacterUserPanel->CreateSubControl ();
		m_pSelectCharacterUserPanel->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pSelectCharacterUserPanel );
		ShowGroupFocus ( SELECT_CHARACTER_USERPANEL );
	}

	{
		m_pCharacterCreatePage = new CCharacterCreatePage;
		m_pCharacterCreatePage->Create ( CHARACTER_CREATE_PAGE, "CREATE_CHAR_WINDOW" );
		m_pCharacterCreatePage->CreateSubControl ();
		m_pCharacterCreatePage->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		RegisterControl ( m_pCharacterCreatePage );
		ShowGroupFocus ( CHARACTER_CREATE_PAGE );

		m_pCharacterCreateInfo = new CCharacterCreateInfo;
		m_pCharacterCreateInfo->Create ( CHARACTER_CREATE_INFO, "CREATE_CHAR_INFO" );
		m_pCharacterCreateInfo->CreateSubControl ();
		m_pCharacterCreateInfo->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		RegisterControl ( m_pCharacterCreateInfo );
		ShowGroupFocus ( CHARACTER_CREATE_INFO );

		m_pCharacterCreateRotate = new CCharacterCreateRotate;
		m_pCharacterCreateRotate->Create ( CHARACTER_CREATE_ROTATE, "ROTATE_CHAR_WINDOW" );
		m_pCharacterCreateRotate->CreateSubControl ();
		m_pCharacterCreateRotate->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		RegisterControl ( m_pCharacterCreateRotate );
		ShowGroupFocus ( CHARACTER_CREATE_ROTATE );
		
	}

	{	//	모달
		{
			m_pModalWindow = new CModalWindow;
			m_pModalWindow->Create ( MODAL_WINDOW_OUTER, "BASIC_MODAL_WINDOW", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
			m_pModalWindow->CreateSubControl ();
			RegisterControl ( m_pModalWindow );
			ShowGroupFocus ( MODAL_WINDOW_OUTER );
		}
		//else
		//{
		//	CModalWindow* pModalWindow = new CModalWindow;
		//	pModalWindow->Create ( MODAL_WINDOW_OUTER, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		//	pModalWindow->CreateBaseModal ( "MODAL_WINDOW");
		//	pModalWindow->CreateSubControl ();
		//	pModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		//	RegisterControl ( pModalWindow );
		//	m_pModalWindow = pModalWindow;
		//	ShowGroupFocus ( MODAL_WINDOW_OUTER );		
		//}
	}

	{
		m_pUpImage = new CUIControl;
		m_pUpImage->Create ( OUTER_UP_IMAGE, "OUTER_UP_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );		
		RegisterControl ( m_pUpImage );
		ShowGroupBottom ( OUTER_UP_IMAGE );

#ifdef CH_PARAM
		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->Create ( CHINA_TITLE, "CHINA_TITLE", UI_FLAG_XSIZE );
			pTextBox->SetFont ( pFont9 );
			pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
			pTextBox->SetLineInterval ( fLIST_LINE_INTERVAL );
			RegisterControl ( pTextBox );		
			pTextBox->AddText ( ID2GAMEEXTEXT ( "CHINA_OUTER_UP_TITLE" ) );
			ShowGroupBottom ( CHINA_TITLE );
		}
#endif

		m_pDownImage = new CUIControl;
		m_pDownImage ->Create ( OUTER_DN_IMAGE, "OUTER_DN_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		RegisterControl ( m_pDownImage  );
		ShowGroupBottom ( OUTER_DN_IMAGE );

		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->Create ( COPYRIGHT, "COPYRIGHT", UI_FLAG_XSIZE | UI_FLAG_BOTTOM );
			pTextBox->SetFont ( pFont9 );
			pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
			pTextBox->SetLineInterval ( fLIST_LINE_INTERVAL );
			RegisterControl ( pTextBox );		
			pTextBox->AddText ( ID2GAMEWORD ( "COPYRIGHT_TITLE" ) );
			pTextBox->AddText ( ID2GAMEWORD ( "COPYRIGHT_COMPANY" ) );
			ShowGroupBottom ( COPYRIGHT );
		}
	}

	{
		m_pFullScreenButton = new CUIControl;
		m_pFullScreenButton->Create ( FULLSCREEN_OUTER, "FULLSCREEN_OUTER", UI_FLAG_RIGHT );
		RegisterControl ( m_pFullScreenButton );
		ShowGroupTop ( FULLSCREEN_OUTER );

		m_pFullScreenButtonDummy = new CUIControl;
		m_pFullScreenButtonDummy->Create ( FULLSCREEN_OUTER_DUMMY, "FULLSCREEN_OUTER", UI_FLAG_RIGHT );
		m_pFullScreenButtonDummy->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pFullScreenButtonDummy );
	}

	{		
		m_pWaitDisplay = new CWaitDialogue;
		m_pWaitDisplay->Create ( WAIT_DISPLAY, "WAITSERVER_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pWaitDisplay->CreateSubControl ();
		RegisterControl ( m_pWaitDisplay );
		ShowGroupFocus ( WAIT_DISPLAY );
	}

	{
		m_pLoginMark = new CUIControl;
		m_pLoginMark->Create ( LOGIN_MARK, "LOGIN_MARK", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pLoginMark );
		ShowGroupBottom ( LOGIN_MARK );
	}

	
	{
		m_pLobbyTitle = new CLobbyTitle;
		m_pLobbyTitle->Create ( LOBBY_TITLE, "OUTER_LOBBY_TITLE" );
		m_pLobbyTitle->CreateSubControl ();
		m_pLobbyTitle->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		RegisterControl ( m_pLobbyTitle );
		ShowGroupBottom ( LOBBY_TITLE );
	}

	{	/*register page, Juver, 2017/11/17 */
		m_pRegisterPage = new CRegisterPage;
		m_pRegisterPage->CreateEx ( REGISTER_PAGE, "OUTER_WINDOW" );
		m_pRegisterPage->CreateBaseWidnow ( "RAN_REGISTER_PAGE", (char*) ID2GAMEWORD ( "RAN_REGISTER_WINDOW", 0  ) );
		m_pRegisterPage->CreateSubControl ();
		m_pRegisterPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pRegisterPage );
		ShowGroupFocus ( REGISTER_PAGE );
		HideGroup ( REGISTER_PAGE );
	}

	CloseAllWindow ();

#ifdef CH_PARAM
	ShowGroupFocus( SELECT_CHINA_AREA_PAGE );
	SetLobbyTitle( SELECT_CHINA_AREA_PAGE );
	
#else
	ShowGroupFocus( SELECT_SERVER_PAGE );	
	SetLobbyTitle( SELECT_SERVER_PAGE );
#endif

	return S_OK;
}

HRESULT COuterInterface::FrameMove ( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime )
{
	ResetOnTheInterface ();

	bool bFULLSCREEN = RANPARAM::bScrWindowed && RANPARAM::bScrWndHalfSize;
	if ( bFULLSCREEN )	ShowGroupTop ( FULLSCREEN_OUTER );
	else				HideGroup ( FULLSCREEN_OUTER );

	if ( IsVisibleGroup ( FULLSCREEN_OUTER ) )
	{
		const UIRECT& rcGlobalPos = m_pFullScreenButtonDummy->GetGlobalPos ();
		m_pFullScreenButton->SetGlobalPos ( D3DXVECTOR2(rcGlobalPos.left,rcGlobalPos.top) );
	}

	/*security key on charselect, DevArt22*/
	if ( m_nCharPassAttempt > 3 )
	{
		DoModalOuter ( ID2GAMEEXTEXT ("CHARSELECT_PASS_TOOMANY_ATTEMPT"), MODAL_INFOMATION, OK, OUTER_MODAL_LOGOUT );
		m_nCharPassAttempt = 0;
	}

	HRESULT hr;
	hr = CUIMan::FrameMove ( pd3dDevice, fTime, fElapsedTime );
	if ( FAILED ( hr ) ) return hr;

	//	TIME UPDATE
	UPDATE_WAIT_TIME ( fElapsedTime );

	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
	if ( !pNetClient->IsOnline() )		//네트워크 연결이 끊어진 경우
	{
		if ( !COuterInterface::GetInstance().IsVisibleGroup ( MODAL_WINDOW_OUTER ) )
		{
			if( !IsLoginCancel() )
			{
				DoModalOuter ( ID2GAMEEXTEXT ("CHARACTERSTAGE_2"), MODAL_INFOMATION, OK, OUTER_MODAL_CONNECTCLOSED );
			}
			else
			{
				/*login port, Juver, 2017/11/16 */
				if ( DxGlobalStage::GetInstance().GetNetClient()->ConnectLoginServer(RANPARAM::LoginAddress, RANPARAM::nLoginPort) > NET_ERROR )
				{
					DxGlobalStage::GetInstance().GetNetClient()->SndReqServerInfo();				
				}
				else
				{
					DoModalOuter( ID2GAMEEXTEXT ( "SERVERSTAGE_3" ), MODAL_INFOMATION, OKCANCEL, OUTER_MODAL_CLOSEGAME );
				}
			}
		}
	}

	if ( IsBlockProgramFound() )
	{
		DoModalOuter ( ID2GAMEINTEXT("BLOCK_PROGRAM_FOUND"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
	}

	if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU ) & DXKEY_PRESSED )
	{
		if ( DxInputDevice::GetInstance().GetKeyState ( DIK_F4 ) & DXKEY_DOWN )
		{
			DoModalOuter ( ID2GAMEINTEXT("CLOSE_GAME"), MODAL_QUESTION, OKCANCEL, OUTER_MODAL_CLOSEGAME );
		}
	}

	if ( IsVisibleGroup ( CHARACTER_CREATE_PAGE ) )
	{
		CUIFocusContainer::UICONTROL_FOCUSLIST focus_list = GetFocusList()->GetFocusList();
		CUIFocusContainer::UICONTROL_FOCUSLIST_RITER riter = focus_list.rbegin ();
		CUIFocusContainer::UICONTROL_FOCUSLIST_RITER riter_end = focus_list.rend ();

		bool bCharacterCreateInfoShow = false;
		bool bCharacterCreateButtonShow = false;
		bool bCharacterCreateRotateShow = false;
		for ( ; riter != riter_end; ++riter )
		{
			UIGUID cID = riter->cID;
			if ( CHARACTER_CREATE_PAGE == cID )
			{
				if ( bCharacterCreateInfoShow ){
					GetFocusList()->InsertAfter ( CHARACTER_CREATE_INFO, CHARACTER_CREATE_PAGE, riter->pUIControl );
				}break;

				if ( bCharacterCreateRotateShow ){
					GetFocusList()->InsertAfter ( CHARACTER_CREATE_ROTATE, CHARACTER_CREATE_PAGE, riter->pUIControl );
				}break;
			}
			else if ( CHARACTER_CREATE_INFO == cID ){
				bCharacterCreateInfoShow = true;
			}
			else if ( CHARACTER_CREATE_ROTATE == cID ){
				bCharacterCreateRotateShow = true;
			}
		}
	}

	{
		// Note : 위아래 바의 크기를 조절한다. 
		LONG lResolution = CUIMan::GetResolution ();

		//if( m_lResolutionBack != lResolution )
		{
			WORD X_RES = HIWORD( lResolution );
			WORD Y_RES = LOWORD( lResolution );

			INT nHeight = static_cast<INT>(Y_RES * 0.1f);
			INT nTop = Y_RES - nHeight;

			if ( m_pUpImage && m_pDownImage )
			{
				m_pUpImage->SetGlobalPos( UIRECT( 0, 0, (float)X_RES, (float)nHeight ) );
				m_pDownImage->SetGlobalPos( UIRECT( 0, (float)nTop, (float)X_RES, (float)nHeight ) );
			}

			// Note : 캐릭터 선택, 생성 윈도우의 위치를 변경한다.
			UIRECT rcLocalPos;


			if( m_pLobbyTitle->IsVisible() && m_pUpImage->IsVisible() )
			{
				rcLocalPos = m_pLobbyTitle->GetLocalPos();
				rcLocalPos.top = m_pUpImage->GetGlobalPos().bottom - 40.0f;
				rcLocalPos.left = float( X_RES / 2 ) - ( rcLocalPos.sizeX / 2.0f );
				m_pLobbyTitle->SetGlobalPos( rcLocalPos );
			}

			if( m_pSelectCharacterPage->IsVisible() && m_pUpImage->IsVisible() )
			{
				rcLocalPos = m_pSelectCharacterPage->GetLocalPos();
				rcLocalPos.top = m_pUpImage->GetGlobalPos().bottom + 2.0f;
				rcLocalPos.left = X_RES - rcLocalPos.sizeX - 2.0f;
				m_pSelectCharacterPage->SetGlobalPos( rcLocalPos );
			}

			if( m_pSelectCharacterUserPanel->IsVisible() && m_pUpImage->IsVisible() )
			{
				rcLocalPos = m_pSelectCharacterUserPanel->GetLocalPos();
				rcLocalPos.top = m_pUpImage->GetGlobalPos().bottom + 2.0f;
				//rcLocalPos.left = X_RES - rcLocalPos.sizeX - 2.0f;
				m_pSelectCharacterUserPanel->SetGlobalPos( rcLocalPos );
			}

			if( m_pCharacterCreatePage->IsVisible() && m_pUpImage->IsVisible() )
			{
				rcLocalPos = m_pCharacterCreatePage->GetLocalPos();
				rcLocalPos.top = m_pUpImage->GetGlobalPos().bottom + 2.0f;
				rcLocalPos.left = X_RES - rcLocalPos.sizeX - 2.0f;
				m_pCharacterCreatePage->SetGlobalPos( rcLocalPos );
			}

			if( m_pCharacterCreateInfo->IsVisible() && m_pUpImage->IsVisible() )
			{
				rcLocalPos = m_pCharacterCreateInfo->GetLocalPos();
				rcLocalPos.top = m_pUpImage->GetGlobalPos().bottom + 2.0f;
				rcLocalPos.left = 2.0f;
				m_pCharacterCreateInfo->SetGlobalPos( rcLocalPos );
			}

			if( m_pCharacterCreateRotate->IsVisible() && m_pDownImage->IsVisible() )
			{
				rcLocalPos = m_pCharacterCreateRotate->GetLocalPos();
				rcLocalPos.top = m_pDownImage->GetGlobalPos().top - ( rcLocalPos.sizeY + 10 );
				rcLocalPos.left = float( X_RES / 2 ) - ( rcLocalPos.sizeX / 2.0f ) + 20;
				m_pCharacterCreateRotate->SetGlobalPos( rcLocalPos );
			}

			
			// Note : 패스워드 키보드의 위치를 조정한다.
			if( m_pPassKeyboard->IsVisible() )
			{
				rcLocalPos = m_pPassKeyboard->GetGlobalPos();
				if ( m_pLoginPage->IsVisible() )
					rcLocalPos.top = m_pLoginPage->GetGlobalPos().bottom + 13.0f;
/*
//#if defined(KRT_PARAM) || defined(KR_PARAM)
#if defined ( JP_PARAM )	// 2차 패스워드
				else if ( m_pSecPassSetPage->IsVisible() ) 
					rcLocalPos.top = m_pSecPassSetPage->GetGlobalPos().bottom + 13.0f;
				else if ( m_pSecPassCheckPage->IsVisible() )
					rcLocalPos.top = m_pSecPassCheckPage->GetGlobalPos().bottom + 13.0f;
#endif
*/
				m_pPassKeyboard->SetGlobalPos( rcLocalPos );
			}

			if ( m_pLoginMark && m_pUpImage )
			{
				rcLocalPos = m_pLoginMark->GetGlobalPos();
				if ( m_pUpImage->IsVisible() )
					rcLocalPos.top = m_pUpImage->GetGlobalPos().bottom + 10.0f;

				m_pLoginMark->SetGlobalPos( rcLocalPos );
			}

			//m_lResolutionBack = lResolution;
		}
	}

	return S_OK;
}

HRESULT COuterInterface::DeleteDeviceObjects()
{
	ResetControl();
	return CUIMan::DeleteDeviceObjects ();
}

// 중국 MMOSPEED
// 중국 네트워크 체크 프로그램 실행
void COuterInterface::RunMMOSpeed()
{
	CString str;
	str.Format ( "%s\\%s", SUBPATH::APP_ROOT, "mmospeed.exe");
	
	CString strCmd;
	std::string strUserID = RANPARAM::GETUSERID_DEC();

	// UserID, 게임 식별번호, 지역번호
	strCmd.Format( "%s %d %d", strUserID.c_str(), 1, RANPARAM::nChinaRegion+1 );

	int result = (int)ShellExecute( NULL , "open", str.GetString() ,strCmd.GetString(), NULL, SW_SHOW );

	if( result < 32  )
	{
		// 에러처리
		return;
	}
	
	return;
}

void COuterInterface::CreateCharacterSetUseCancel( BOOL bSet )
{
	if( m_pCharacterCreatePage ){
		m_pCharacterCreatePage->SetUseCancel( bSet );
	}
}

void COuterInterface::SetLobbyTitle( int nCID )
{
	CString strText = "";
	switch( nCID )
	{
	case SELECT_SERVER_PAGE:
		strText = ID2GAMEWORD ("OUTER_LOBBY_TITLE_TEXT", 0 );
		break;

	case SELECT_CHINA_AREA_PAGE:
		strText = ID2GAMEWORD ("OUTER_LOBBY_TITLE_TEXT", 0 );
		break;

	case LOGIN_PAGE:
		strText = ID2GAMEWORD ("OUTER_LOBBY_TITLE_TEXT", 1 );
		break;

	case SECPASS_SETPAGE:
		strText = ID2GAMEWORD ("OUTER_LOBBY_TITLE_TEXT", 2 );
		break;

	case SECPASS_CHECKPAGE:
		strText = ID2GAMEWORD ("OUTER_LOBBY_TITLE_TEXT", 3 );
		break;

	case SELECT_CHARACTER_PAGE:
		strText = ID2GAMEWORD ("OUTER_LOBBY_TITLE_TEXT", 4 );
		break;

	case CHARACTER_CREATE_PAGE:
		strText = ID2GAMEWORD ("OUTER_LOBBY_TITLE_TEXT", 5 );
		break;

		/*register page, Juver, 2017/11/17 */
	case REGISTER_PAGE:
		strText = ID2GAMEWORD ("OUTER_LOBBY_TITLE_TEXT", 6 );
		break;
	};

	if( m_pLobbyTitle ){
		ShowGroupBottom( LOBBY_TITLE );
		m_pLobbyTitle->SetLobbyText( strText.GetString() );
	}
}