#include "StdAfx.h"
#include "./SelectCharacterUserPanelPage.h"
#include "BasicTextBox.h"
#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "BasicLineBox.h"
#include "BasicTextButton.h"
#include "OuterInterface.h"
#include "ModalWindow.h"
#include "DxGlobalStage.h"
#include "s_NetClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSelectCharacterUserPanelPage::CSelectCharacterUserPanelPage () :
	//m_uCharRemain ( USHRT_MAX )
	 m_uUserPoint(0),
	 m_dwGameTime(0),
	 m_uBattlePoint(0)
{
}

CSelectCharacterUserPanelPage::~CSelectCharacterUserPanelPage ()
{
}

void CSelectCharacterUserPanelPage::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	{	
	
		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_EMAIL_TEXTBOX_STATIC" );
			pTextBox->SetFont ( pFont );
			pTextBox->AddText ( ID2GAMEWORD("SELECT_CHARACTER_USER_STATIC",0), NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_PASSWORD_TEXTBOX_STATIC" );
			pTextBox->SetFont ( pFont );
			pTextBox->AddText ( ID2GAMEWORD("SELECT_CHARACTER_USER_STATIC",1), NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_PIN_TEXTBOX_STATIC" );
			pTextBox->SetFont ( pFont );
			pTextBox->AddText ( ID2GAMEWORD("SELECT_CHARACTER_USER_STATIC",2), NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_GAMETIME_TEXTBOX_STATIC" );
			pTextBox->SetFont ( pFont );
			pTextBox->AddText ( ID2GAMEWORD("SELECT_CHARACTER_USER_STATIC",3), NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_USERPOINT_TEXTBOX_STATIC" );
			pTextBox->SetFont ( pFont );
			pTextBox->AddText ( ID2GAMEWORD("SELECT_CHARACTER_USER_STATIC",4), NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_BATTLEPOINT_TEXTBOX_STATIC" );
			pTextBox->SetFont ( pFont );
			pTextBox->AddText ( ID2GAMEWORD("SELECT_CHARACTER_USER_STATIC",5), NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_EMAIL_TEXTBOX" );
			pTextBox->SetFont ( pFont );
			pTextBox->SetText ( "D***y@yahoo.com", NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
			m_pEmailTextBox = pTextBox;
			
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_GAMETIME_TEXTBOX" );
			pTextBox->SetFont ( pFont );
			RegisterControl ( pTextBox );
			m_pGameTimeTextBox = pTextBox;
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_USERPOINT_TEXTBOX" );
			pTextBox->SetFont ( pFont );
			RegisterControl ( pTextBox );
			m_pUserPointTextBox = pTextBox;
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_BATTLEPOINT_TEXTBOX" );
			pTextBox->SetFont ( pFont );
			RegisterControl ( pTextBox );
			m_pBattlePointTextBox = pTextBox;
		}
	}
}

void CSelectCharacterUserPanelPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindow::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	const USHORT uUserPoint = COuterInterface::GetInstance().GetUserPoint();
	const USHORT uBattlePoint = COuterInterface::GetInstance().GetBattlePoint();
	const DWORD dwGameTime = COuterInterface::GetInstance().GetGameTime();
	const CString strEmail =  COuterInterface::GetInstance().GetEmail();
	{
			m_pUserPointTextBox->ClearText();
			CString strTemp;
			strTemp.Format ( "%u", uUserPoint );
			m_pUserPointTextBox->SetText ( strTemp, NS_UITEXTCOLOR::PALEGOLDENROD );
			
			m_uUserPoint = uUserPoint;
	}
	{
			m_pBattlePointTextBox->ClearText();
			CString strTemp;
			strTemp.Format ( "%u", uBattlePoint );
			m_pBattlePointTextBox->SetText ( strTemp, NS_UITEXTCOLOR::WHITE );

			m_uBattlePoint = uBattlePoint;
	}
	{
			m_pGameTimeTextBox->ClearText();
			CString strTemp;
			strTemp.Format ( "%d Minute(s)", dwGameTime );
			m_pGameTimeTextBox->SetText ( strTemp, NS_UITEXTCOLOR::WHITE );

			m_dwGameTime = dwGameTime;
	}
	{
			m_pEmailTextBox->ClearText();
			CString strTemp;
			strTemp.Format ( "%s", strEmail.GetString() );
			m_pEmailTextBox->SetText ( strEmail, NS_UITEXTCOLOR::WHITE );

			m_strEmail = strEmail;
	}

}