#include "StdAfx.h"
#include "./SelectCharacterUserPanelButton.h"
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

CSelectCharacterUserPanelButton::CSelectCharacterUserPanelButton () :
	 m_pButtonCE(NULL),
	 m_pButtonCP(NULL),
	 m_pButtonCPN(NULL),
	 m_pButtonGTCVT(NULL),
	 m_pButtonPPCVT(NULL)
{
}

CSelectCharacterUserPanelButton::~CSelectCharacterUserPanelButton ()
{
}

void CSelectCharacterUserPanelButton::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	const int nBUTTONSIZE = CBasicTextButton::SIZE18;
		
	{
			m_pButtonCE = new CBasicTextButton;
			m_pButtonCE->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, SELECT_CHARACTER_USERPANEL_CE );
			m_pButtonCE->CreateBaseButton ( "SELECT_CHARACTER_USERPANEL_CE", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "SELECT_CHARACTER_USERPANEL_BUTTON", 0 ) );
			RegisterControl ( m_pButtonCE );

			m_pButtonCP = new CBasicTextButton;
			m_pButtonCP->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, SELECT_CHARACTER_USERPANEL_CP );
			m_pButtonCP->CreateBaseButton ( "SELECT_CHARACTER_USERPANEL_CP", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "SELECT_CHARACTER_USERPANEL_BUTTON", 1 ) );
			RegisterControl ( m_pButtonCP );

			m_pButtonCPN = new CBasicTextButton;
			m_pButtonCPN->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, SELECT_CHARACTER_USERPANEL_CPN );
			m_pButtonCPN->CreateBaseButton ( "SELECT_CHARACTER_USERPANEL_CPN", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "SELECT_CHARACTER_USERPANEL_BUTTON", 2 ) );
			RegisterControl ( m_pButtonCPN );

			m_pButtonGTCVT = new CBasicTextButton;
			m_pButtonGTCVT->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, SELECT_CHARACTER_USERPANEL_GTCVT );
			m_pButtonGTCVT->CreateBaseButton ( "SELECT_CHARACTER_USERPANEL_GTCVT", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "SELECT_CHARACTER_USERPANEL_BUTTON", 4 ) );
			RegisterControl ( m_pButtonGTCVT );

			//m_pButtonPPCVT = new CBasicTextButton;
			//m_pButtonPPCVT->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, SELECT_CHARACTER_USERPANEL_PPCVT );
			//m_pButtonPPCVT->CreateBaseButton ( "SELECT_CHARACTER_USERPANEL_PPCVT", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "SELECT_CHARACTER_USERPANEL_BUTTON", 6 ) );
			//RegisterControl ( m_pButtonPPCVT );
	}
}

void CSelectCharacterUserPanelButton::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}
void CSelectCharacterUserPanelButton::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case SELECT_CHARACTER_USERPANEL_CP:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
				if( pNetClient->IsOnline() == true )
				{
					COuterInterface::GetInstance().ToCPPage ( GetWndID () );
				}
			}
		}
		break;

	case SELECT_CHARACTER_USERPANEL_CPN:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
				if( pNetClient->IsOnline() == true )
				{
					COuterInterface::GetInstance().ToCPNPage ( GetWndID () );
				}
			}
		}
		break;

	case SELECT_CHARACTER_USERPANEL_CE:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				COuterInterface::GetInstance().ToCEPage ( NULL );
			}
		}
		break;

	case SELECT_CHARACTER_USERPANEL_GTCVT:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
				if( pNetClient->IsOnline() == true )
				{
					COuterInterface::GetInstance().ToGTCVTPage ( GetWndID () );
				}
			}
		}
		break;
	/*case SELECT_CHARACTER_USERPANEL_PPCVT:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
				if( pNetClient->IsOnline() == true )
				{
					COuterInterface::GetInstance().ToPPCVTPage ( GetWndID () );
				}
			}
		}
		break;*/
	}
}