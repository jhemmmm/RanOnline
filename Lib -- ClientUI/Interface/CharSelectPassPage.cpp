#include "StdAfx.h"
#include "CharSelectPassPage.h"
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
#include "UIEditBoxMan.h"
#include "../Lib -- Engine/GUInterface/UIKeyCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int	CCharSelectPassPage::nLIMIT_CODE = 20;
int	CCharSelectPassPage::nLIMIT_PIN = 12;
int	CCharSelectPassPage::nLIMIT_CP = 7;

CCharSelectPassPage::CCharSelectPassPage ()
	: m_pRandTextBox(NULL)
	, m_nRandPassNumber(0)
	, m_nRPUpdateCnt(0)
	, m_PrevPageID ( NO_ID )
{
	memset( m_szRandomString, 0, sizeof( m_szRandomString ) );
}

CCharSelectPassPage::~CCharSelectPassPage ()
{
}

void CCharSelectPassPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	const int nBUTTONSIZE = CBasicTextButton::SIZE18;

	CBasicTextBox* pTextBox = NULL;
	pTextBox = CreateStaticControl ( "CHARSELECT_PASS_PAGE_PASS", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT );
	pTextBox->SetOneLineText ( (char*)ID2GAMEWORD ( "CHARSELECT_PASS_PAGE_INFO", 0 ) );;

	//m_pRandTextBox = CreateStaticControl ( "TOPUP_PAGE_CP", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT );

	
	{
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox->CreateBaseBoxOuter ( "CHARSELECT_PASS_PAGE_PASS_BACK" );
		RegisterControl ( pBasicLineBox );
	}

	{
		CUIEditBoxMan* pEditBoxMan = new CUIEditBoxMan;
		pEditBoxMan->CreateSub ( this, "CHARSELECT_PASS_EDITMAN", UI_FLAG_DEFAULT, CHARSELECT_PASS_EDIT_MAN );
		pEditBoxMan->CreateEditBox ( CHARSELECT_PASS_EDIT_PASS, "CHARSELECT_PASS_EDIT_PASS", "CHARSELECT_PASS_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_CODE );

		pEditBoxMan->SetHide ( CHARSELECT_PASS_EDIT_PASS, TRUE );
		RegisterControl( pEditBoxMan );
		m_pEditBoxMan = pEditBoxMan;
	}

	CBasicTextButton* pOKButton = new CBasicTextButton;
	pOKButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, CHARSELECT_PASS_OK );
	pOKButton->CreateBaseButton ( "CHARSELECT_PASS_PAGE_OK", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "CHARSELECT_PASS_PAGE_OKCANCEL", 0 ) );
	pOKButton->SetShortcutKey ( DIK_RETURN, DIK_NUMPADENTER );
	RegisterControl ( pOKButton );

	CBasicTextButton* pQuitButton = new CBasicTextButton;
	pQuitButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, CHARSELECT_PASS_CANCEL );
	pQuitButton->CreateBaseButton ( "CHARSELECT_PASS_PAGE_CANCEL", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "CHARSELECT_PASS_PAGE_OKCANCEL", 1 ) );
	pQuitButton->SetShortcutKey ( DIK_ESCAPE );
	RegisterControl ( pQuitButton );

}	

void CCharSelectPassPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	CUIWindow::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CCharSelectPassPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindow::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case CHARSELECT_PASS_OK:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				SetMessageEx( dwMsg &= ~UIMSG_KEY_FOCUSED ); // Focus 문제로 메세지 삭제
				
				CString strPass = m_pEditBoxMan->GetEditString ( CHARSELECT_PASS_EDIT_PASS );

				
				if ( !CheckString ( strPass ) )
				{
					DoModalOuter ( ID2GAMEEXTEXT ( "TOPUP_PAGE_CD_ERROR" ) );
					return ;
				}

				CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
				
				 pNetClient->SndCharSelectPass( strPass );
			}
		}
		break;

	case CHARSELECT_PASS_CANCEL:
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

				COuterInterface::GetInstance().HideGroup ( CHARSELECT_PASS_PAGE, true );
			}
		}
		break;
	};
}

BOOL CCharSelectPassPage::CheckString( CString strTemp )
{	
	strTemp = strTemp.Trim();

	// 문자열 체크 - 들어가면 안되는 특수문자 : ~!@#$%^&*+|":?><\=`',.;[]{}()
	if( STRUTIL::CheckString( strTemp ) )
	{
		DoModalOuter ( ID2GAMEEXTEXT ( "TOPUPSTAGE_4" ), MODAL_INFOMATION, OK );		
		return FALSE;
	}

    return TRUE;
}

void CCharSelectPassPage::ResetAll ()
{
	m_pEditBoxMan->EndEdit ();

	m_pEditBoxMan->ClearEdit ( CHARSELECT_PASS_EDIT_PASS );

	m_nRandPassNumber = 0;
	m_szRandomString[7] = 0;
	m_nRPUpdateCnt = 0;


	m_pEditBoxMan->Init();
	m_pEditBoxMan->BeginEdit();

	if( m_pEditBoxMan->IsMODE_NATIVE() )
	{
		m_pEditBoxMan->DoMODE_TOGGLE();
	}
}

void CCharSelectPassPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if ( bVisible )
	{

		ResetAll();

		if( m_pRandTextBox ) m_pRandTextBox->ClearText();
	}
	else
	{
		m_pEditBoxMan->EndEdit ();

		CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
		pNetClient->ResetRandomPassNumber();
	}
}

CBasicButton* CCharSelectPassPage::CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::RADIO_FLIP );
	pButton->SetControlNameEx ( szButton );
	RegisterControl ( pButton );

	return pButton;
}

void CCharSelectPassPage::SetCharToEditBox( TCHAR cKey )
{
	if( !m_pEditBoxMan ) return;
	
	CString strTemp;

	UIGUID nID = m_pEditBoxMan->GetBeginEditBox();

	strTemp = m_pEditBoxMan->GetEditString( nID );
	strTemp += cKey;

	m_pEditBoxMan->SetEditString( nID, strTemp );
}

void CCharSelectPassPage::DelCharToEditBox()
{
	if( !m_pEditBoxMan ) return;

	CString strTemp;

	UIGUID nID = m_pEditBoxMan->GetBeginEditBox();

	strTemp = m_pEditBoxMan->GetEditString( nID );
	INT nLenth = strTemp.GetLength();
	strTemp = strTemp.Left( nLenth - 1 );

	m_pEditBoxMan->SetEditString( nID, strTemp );
}

void CCharSelectPassPage::GoNextTab()
{
	if( m_pEditBoxMan )
		m_pEditBoxMan->GoNextTab();
}