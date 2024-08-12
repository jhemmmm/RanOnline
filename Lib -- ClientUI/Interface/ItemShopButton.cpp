#include "StdAfx.h"
#include "ItemShopButton.h"
#include "./InnerInterface.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNPCShopButton::CNPCShopButton ()
	: m_pButtonImage(NULL)
{
}

CNPCShopButton::~CNPCShopButton ()
{
}

void CNPCShopButton::CreateSubControl ()
{
	m_pButtonImage = new CUIControl;
	m_pButtonImage->CreateSub ( this, "NPCSHOP_BUTTON_IMAGE", UI_FLAG_DEFAULT, NPCSHOP_BUTTON );
	m_pButtonImage->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonImage );
}

void CNPCShopButton::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CNPCShopButton::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
		case NPCSHOP_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) ){
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Item Mall" , NS_UITEXTCOLOR::WHITE  );				
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) ){ 
				CInnerInterface::GetInstance().OpenNpcWindow();
			}
		}break;
	}
}
