#include "StdAfx.h"
#include "./SelectCharacterUserPanel.h"
#include "./SelectCharacterUserPanelPage.h"
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

CSelectCharacterUserPanel::CSelectCharacterUserPanel ()
	//m_uCharRemain ( USHRT_MAX )
	: m_pSelectCharacterUserPanelPage (NULL)
{
}

CSelectCharacterUserPanel::~CSelectCharacterUserPanel ()
{
}

void CSelectCharacterUserPanel::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	m_pSelectCharacterUserPanelPage = new CSelectCharacterUserPanelPage;
	m_pSelectCharacterUserPanelPage->CreateSub ( this, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pSelectCharacterUserPanelPage->CreateBaseWidnow ( "SELECT_CHARACTER_USERPANELWINDOW", (char*)ID2GAMEWORD("SELECT_CHARACTER_USERPANEL") );
	m_pSelectCharacterUserPanelPage->CreateSubControl ();
	RegisterControl ( m_pSelectCharacterUserPanelPage );

	m_pSelectCharacterUserPanelButton = new CSelectCharacterUserPanelButton;
	m_pSelectCharacterUserPanelButton->CreateSub( this, "SELECT_CHARACTER_USERPANEL_BUTTON", UI_FLAG_DEFAULT, SELECT_CHARACTER_USERPANEL_BTN );
	m_pSelectCharacterUserPanelButton->CreateSubControl ();
	RegisterControl ( m_pSelectCharacterUserPanelButton );
}

void CSelectCharacterUserPanel::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}
void CSelectCharacterUserPanel::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
	
	if ( CHECK_MOUSE_IN ( dwMsg ) )
	{
		AddMessageEx ( UIMSG_MOUSEIN_SELECT_USERPANEL );
	}
}