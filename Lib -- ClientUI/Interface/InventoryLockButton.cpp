#include "StdAfx.h"
#include "InventoryLockButton.h"
#include "./BasicButton.h"
#include "InnerInterface.h"
#include "RANPARAM.h"
#include "GLGaeaClient.h"
#include "GLCharacter.h"
#include "UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CInventoryLockButton::CInventoryLockButton ()
	: m_pInventoryLockButton(NULL)
	, m_pInventoryLockButtonLock(NULL)
{
}

CInventoryLockButton::~CInventoryLockButton ()
{
}

void CInventoryLockButton::CreateSubControl ()
{
	m_pInventoryLockButton = new CBasicButton;
	m_pInventoryLockButton->CreateSub ( this, "INVENTORY_LOCK_OPEN_BUTTON", UI_FLAG_DEFAULT, INVENTORY_LOCK_OPEN_BUTTON );
	m_pInventoryLockButton->CreateMouseOver( "MENU_OVER_IMAGE_INVENTORYLOCK" );
	m_pInventoryLockButton->SetUseGlobalAction ( TRUE );
	RegisterControl ( m_pInventoryLockButton );

	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, "INVENTORY_LOCK_OPEN_BUTTON_F" );
	pControl->SetVisibleSingle ( FALSE );
	RegisterControl ( pControl );
	m_pInventoryLockButtonLock = pControl;
}
void CInventoryLockButton::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	BOOL bEnabled = GLGaeaClient::GetInstance().GetCharacter()->m_bEnableInventoryLock;
	BOOL bLocked = GLGaeaClient::GetInstance().GetCharacter()->m_bInventoryLockStatus;
	if ( bEnabled && bLocked ) m_pInventoryLockButtonLock->SetVisibleSingle ( TRUE );
	if ( bEnabled && !bLocked ) m_pInventoryLockButtonLock->SetVisibleSingle ( FALSE );
}
void CInventoryLockButton::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case INVENTORY_LOCK_OPEN_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				BOOL bEnabled = GLGaeaClient::GetInstance().GetCharacter()->m_bEnableInventoryLock;
				BOOL bLocked = GLGaeaClient::GetInstance().GetCharacter()->m_bInventoryLockStatus;

				if ( !bEnabled ) CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Enable Inventory Lock", NS_UITEXTCOLOR::WHITE  );
				if ( bEnabled && bLocked ) CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Unlock Inventory", NS_UITEXTCOLOR::WHITE  );
				if ( bEnabled && !bLocked ) CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Lock Inventory", NS_UITEXTCOLOR::WHITE  );
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				BOOL bEnabled = GLGaeaClient::GetInstance().GetCharacter()->m_bEnableInventoryLock;
				BOOL bLocked = GLGaeaClient::GetInstance().GetCharacter()->m_bInventoryLockStatus;

				if ( !bEnabled ) CInnerInterface::GetInstance().ShowGroupFocus( INVENTORY_LOCK_ENABLE_WINDOW );
				if ( bEnabled && bLocked ) CInnerInterface::GetInstance().OpenInventoryLockInput();
				if ( bEnabled && !bLocked ) CInnerInterface::GetInstance().OpenInventoryLockInput();
			}
		}
		break;
	}
}
