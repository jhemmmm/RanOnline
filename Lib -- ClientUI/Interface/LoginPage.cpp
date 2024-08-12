#include "StdAfx.h"

#include "LoginPage.h"
#include "BasicTextButton.h"
#include "GameTextControl.h"
#include "BasicTextBoxEx.h"
#include "UITextControl.h"
#include "BasicLineBox.h"
#include "OuterInterface.h"
#include "ModalWindow.h"
#include "s_NetClient.h"
#include "RANPARAM.h"
#include "DxGlobalStage.h"
#include "DxInputString.h"
#include "DebugSet.h"

#include "../Lib -- Engine/Common/StringUtils.h"
#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "./UIEditBoxMan.h"
#include "../Lib -- Engine/GUInterface/UIKeyCheck.h"

#include "RANPARAM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int	CLoginPage::nLIMIT_ID = 12;
int	CLoginPage::nLIMIT_PW = 12;
int	CLoginPage::nLIMIT_RP = 6;

CLoginPage::CLoginPage ()
	: m_pIDSaveButton(NULL)
	, m_pRandTextBox(NULL)
	, m_nRandPassNumber(0)
	, m_nRPUpdateCnt(0)
{
}

CLoginPage::~CLoginPage ()
{
}

void CLoginPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	const int nBUTTONSIZE = CBasicTextButton::SIZE18;

	CBasicTextBox* pTextBox = NULL;
	pTextBox = CreateStaticControl ( "LOGIN_PAGE_ID", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT );
	pTextBox->SetOneLineText ( (char*)ID2GAMEWORD ( "LOGIN_PAGE_IDPW", 0 ) );
	pTextBox = CreateStaticControl ( "LOGIN_PAGE_PW", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT );
	pTextBox->SetOneLineText ( (char*)ID2GAMEWORD ( "LOGIN_PAGE_IDPW", 1 ) );

//#ifdef TW_PARAM
//	// Note : 랜덤넘버 안내문
//	m_pRandTextBox = CreateStaticControl ( "LOGIN_PAGE_RP", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT );
//#endif
	
	{
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox->CreateBaseBoxOuter ( "LOGIN_PAGE_ID_BACK" );
		RegisterControl ( pBasicLineBox );

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox->CreateBaseBoxOuter ( "LOGIN_PAGE_PW_BACK" );
		RegisterControl ( pBasicLineBox );

//#ifdef TW_PARAM
//		pBasicLineBox = new CBasicLineBox;
//		pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
//		pBasicLineBox->CreateBaseBoxOuter ( "LOGIN_PAGE_RP_BACK" );
//		RegisterControl ( pBasicLineBox );
//#endif
	}

	{
		CUIEditBoxMan* pEditBoxMan = new CUIEditBoxMan;
		pEditBoxMan->CreateSub ( this, "LOGIN_EDITMAN", UI_FLAG_DEFAULT, LOGIN_EDIT_MAN );
		pEditBoxMan->CreateEditBox ( LOGIN_EDIT_ID, "LOGIN_EDIT_ID", "LOGIN_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_ID );
		pEditBoxMan->CreateEditBox ( LOGIN_EDIT_PW, "LOGIN_EDIT_PW", "LOGIN_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PW );
//#ifdef TW_PARAM
//		pEditBoxMan->CreateEditBox ( LOGIN_EDIT_RP, "LOGIN_EDIT_RP", "LOGIN_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_RP );
//		pEditBoxMan->SetHide ( LOGIN_EDIT_RP, TRUE );
//#endif
		pEditBoxMan->SetHide ( LOGIN_EDIT_PW, TRUE );
		RegisterControl( pEditBoxMan );
		m_pEditBoxMan = pEditBoxMan;
	}

	{
		m_pIDSaveButton = CreateFlipButton ( "LOGIN_PAGE_IDSAVE_BUTTON", "LOGIN_PAGE_IDSAVE_BUTTON_F", LOGIN_PAGE_IDSAVE_BUTTON );
		pTextBox = CreateStaticControl ( "LOGIN_PAGE_IDSAVE_BACK", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT );
		pTextBox->SetOneLineText ( (char*)ID2GAMEWORD ( "LOGIN_PAGE_IDSAVE_BACK" ) );
	}

	CBasicTextButton* pOKButton = new CBasicTextButton;
	pOKButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, LOGIN_OK );
	pOKButton->CreateBaseButton ( "LOGIN_PAGE_OK", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "LOGIN_PAGE_OKCANCEL", 0 ) );
	pOKButton->SetShortcutKey ( DIK_RETURN, DIK_NUMPADENTER );
	RegisterControl ( pOKButton );

	CBasicTextButton* pQuitButton = new CBasicTextButton;
	pQuitButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, LOGIN_CANCEL );
	pQuitButton->CreateBaseButton ( "LOGIN_PAGE_CANCEL", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "LOGIN_PAGE_OKCANCEL", 1 ) );
	pQuitButton->SetShortcutKey ( DIK_ESCAPE );
	RegisterControl ( pQuitButton );

	CBasicTextButton* pRegButton = new CBasicTextButton;
	pRegButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, LOGIN_REGISTER );
	pRegButton->CreateBaseButton ( "LOGIN_PAGE_REG", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "LOGIN_PAGE_OKCANCEL", 2 ) );
	pRegButton->SetVisibleSingle( RANPARAM::bFeatureRegister );
	RegisterControl ( pRegButton );
	
	//CreateControl ( "LOGIN_BOTTOM" );

	LoadIDSaveButton ();
}	

void CLoginPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	CUIWindow::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
	if ( !pNetClient->IsOnline() )		//네트워크 연결이 끊어진 경우
	{
		if ( !COuterInterface::GetInstance().IsVisibleGroup ( MODAL_WINDOW_OUTER ) )
		{
			if( !COuterInterface::GetInstance().IsLoginCancel() )
			{
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_1"), MODAL_INFOMATION, OK, OUTER_MODAL_RECONNECT );
			}
		}
	}
	else
	{
//#ifdef TW_PARAM
//		if( m_nRandPassNumber == 0 )
//		{
//			m_nRandPassNumber = pNetClient->GetRandomPassNumber();
//
//			if( m_nRandPassNumber != 0 )
//			{
//				CString strTemp;
//				strTemp.Format( ID2GAMEEXTEXT( "LOGIN_PAGE_RAND_NUMBER" ), m_nRandPassNumber );
//				if( m_pRandTextBox ) m_pRandTextBox->SetOneLineText( strTemp );
//			}
//			else
//			{
//				if( m_nRPUpdateCnt++ > 10 )
//				{
//					pNetClient->SndRequestRandomKey();
//					m_nRPUpdateCnt = 0;
//				}
//			}
//		}
//#endif
	}

	//	탭 이동
	if ( m_pEditBoxMan )
	{
		CUIControl* pParent = m_pEditBoxMan->GetTopParent ();
		if ( !pParent )	pParent = this;	//	만약 이클래스가 최상위 컨트롤인 경우
		BOOL bFocus = ( pParent->IsFocusControl() );

		if ( bFocus )
		{
			if ( UIKeyCheck::GetInstance()->Check ( DIK_TAB, DXKEY_DOWN ) )
			{
				m_pEditBoxMan->GoNextTab ();
			}
		}
	}
}

void CLoginPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindow::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case LOGIN_OK:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				SetMessageEx( dwMsg &= ~UIMSG_KEY_FOCUSED ); // Focus 문제로 메세지 삭제
				
				CString strID = m_pEditBoxMan->GetEditString ( LOGIN_EDIT_ID );
				CString strPW = m_pEditBoxMan->GetEditString ( LOGIN_EDIT_PW );
				CString strRP;
//#ifdef TW_PARAM
//				strRP = m_pEditBoxMan->GetEditString ( LOGIN_EDIT_RP );
//#endif

				if ( !strID.GetLength () || !strPW.GetLength () )
				{
					DoModalOuter ( ID2GAMEEXTEXT ( "LOGINSTAGE_5" ) );
					return ;
				}

#ifndef CH_PARAM
				if ( !CheckString ( strID ) )
				{
					DoModalOuter ( ID2GAMEEXTEXT ( "LOGIN_PAGE_ID_ERROR" ) );
					return ;
				}

				if ( !CheckString ( strPW ) )
				{
					DoModalOuter ( ID2GAMEEXTEXT ( "LOGIN_PAGE_PW_ERROR" ) );
					return ;
				}
#endif

//#ifdef TW_PARAM
//				if ( !CheckString ( strRP ) )
//				{
//					DoModalOuter ( ID2GAMEEXTEXT ( "LOGIN_PAGE_RP_ERROR" ) );
//					return ;
//				}
//#endif

				//	컨트롤로 부터 아이디와 암호를 가져와 서버에 전송
				CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();

				int nServerGroup, nServerChannel;
				COuterInterface::GetInstance().GetConnectServerInfo( nServerGroup, nServerChannel );

				switch ( RANPARAM::emSERVICE_TYPE )
				{
				case EMSERVICE_THAILAND:
					pNetClient->ThaiSndLogin( strID, strPW, nServerChannel );
					break;

				case EMSERVICE_CHINA:
					pNetClient->ChinaSndLogin( strID, strPW, strRP, nServerChannel );
					break;
//		Japan 로그인 방식 변경
				case EMSERVICE_JAPAN:
					pNetClient->JapanSndLogin( strID, strPW, nServerChannel );
                    break;

				case EMSERVICE_GLOBAL:
					pNetClient->GsSndLogin( strID, strPW, nServerChannel );
					break;

				default:
					pNetClient->SndLogin( strID, strPW, strRP, nServerChannel );		
					break;
				};

				//	Note : 유저 id 설정. ( 게임 설정 저장, id 저장을 위해서 필요함. )
				//
				RANPARAM::SETUSERID ( strID );
				RANPARAM::SAVE_GAMEOPTION();
				
				//	암호 인증 대기 메시지 표시
				DoModalOuter ( ID2GAMEEXTEXT ("LOGINSTAGE_6"), MODAL_INFOMATION, CANCEL, OUTER_MODAL_WAITCONFIRM );
				COuterInterface::GetInstance().SetModalCallWindowID( GetWndID() );

				//COuterInterface::GetInstance().HideGroup ( GetWndID () );
				//COuterInterface::GetInstance().ShowGroupFocus ( SELECT_CHARACTER_PAGE );
			}
		}
		break;

	case LOGIN_CANCEL:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
				if( pNetClient->IsOnline() == true )
				{
					pNetClient->CloseConnect();
					COuterInterface::GetInstance().SetCancelToLogin();
				}

				COuterInterface::GetInstance().ToSelectServerPage ( GetWndID () );
			}
		}
		break;

	case LOGIN_PAGE_IDSAVE_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				if ( m_pIDSaveButton )
				{
					RANPARAM::bSAVE_USERID = !RANPARAM::bSAVE_USERID;
					RANPARAM::SAVE_GAMEOPTION ();
					LoadIDSaveButton ();
				}
			}
		}
		break;

		/*register page, Juver, 2017/11/17 */
	case LOGIN_REGISTER:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				COuterInterface::GetInstance().ToRegisterPage ( GetWndID () );
			}
		}break;

	}
}

BOOL CLoginPage::CheckString( CString strTemp )
{	
	strTemp = strTemp.Trim();

	// 문자열 체크 - 들어가면 안되는 특수문자 : ~!@#$%^&*+|":?><\=`',.;[]{}()
	if( STRUTIL::CheckString( strTemp ) )
	{
		DoModalOuter ( ID2GAMEEXTEXT ( "LOGINSTAGE_4" ), MODAL_INFOMATION, OK );		
		return FALSE;
	}

    return TRUE;
}

void CLoginPage::ResetAll ()
{
	m_pEditBoxMan->EndEdit ();

	m_pEditBoxMan->ClearEdit ( LOGIN_EDIT_ID );
	m_pEditBoxMan->ClearEdit ( LOGIN_EDIT_PW );
//#ifdef TW_PARAM
//	m_pEditBoxMan->ClearEdit ( LOGIN_EDIT_RP );
//
//	m_nRandPassNumber = 0;
//	m_nRPUpdateCnt = 0;
//#endif

	m_pEditBoxMan->Init();
	m_pEditBoxMan->BeginEdit();

	if( m_pEditBoxMan->IsMODE_NATIVE() )
	{
		m_pEditBoxMan->DoMODE_TOGGLE();
	}

	std::string& strSavedUserID = RANPARAM::GETUSERID_DEC();
	if ( strSavedUserID.size() )
	{
		m_pEditBoxMan->SetEditString ( LOGIN_EDIT_ID, CString ( strSavedUserID.c_str() ) );
		m_pEditBoxMan->GoNextTab ();
	}
}

void CLoginPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if ( bVisible )
	{
		if( GLCONST_CHAR::nUI_KEYBOARD == 2 )
		{
			m_pEditBoxMan->DisableKeyInput();
			DXInputString::GetInstance().DisableKeyInput();
		}

		ResetAll();

		if( m_pRandTextBox ) m_pRandTextBox->ClearText();
		COuterInterface::GetInstance().ResetCancelToLogin();
	}
	else
	{
		m_pEditBoxMan->EndEdit ();

		if( GLCONST_CHAR::nUI_KEYBOARD == 2 )
		{
			m_pEditBoxMan->UsableKeyInput();
			DXInputString::GetInstance().UsableKeyInput();
		}

		CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
		pNetClient->ResetRandomPassNumber();
	}
}

CBasicButton* CLoginPage::CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::RADIO_FLIP );
	pButton->SetControlNameEx ( szButton );
	RegisterControl ( pButton );

	return pButton;
}

void CLoginPage::LoadIDSaveButton ()
{
	BOOL bIDSave = RANPARAM::bSAVE_USERID;
	m_pIDSaveButton->SetFlip ( bIDSave );	
}

void CLoginPage::SetCharToEditBox( TCHAR cKey )
{
	if( !m_pEditBoxMan ) return;
	
	CString strTemp;

	UIGUID nID = m_pEditBoxMan->GetBeginEditBox();

	strTemp = m_pEditBoxMan->GetEditString( nID );
	strTemp += cKey;

	m_pEditBoxMan->SetEditString( nID, strTemp );
}

void CLoginPage::DelCharToEditBox()
{
	if( !m_pEditBoxMan ) return;

	CString strTemp;

	UIGUID nID = m_pEditBoxMan->GetBeginEditBox();

	strTemp = m_pEditBoxMan->GetEditString( nID );
	INT nLenth = strTemp.GetLength();
	strTemp = strTemp.Left( nLenth - 1 );

	m_pEditBoxMan->SetEditString( nID, strTemp );
}

void CLoginPage::GoNextTab()
{
	if( m_pEditBoxMan )
		m_pEditBoxMan->GoNextTab();
}