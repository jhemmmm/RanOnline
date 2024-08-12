#include "StdAfx.h"
#include "StorageLockButton.h"
#include "./BasicButton.h"
#include "InnerInterface.h"
#include "RANPARAM.h"
#include "GLGaeaClient.h"
#include "GLCharacter.h"
#include "UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CStorageLockButton::CStorageLockButton ()
	: m_pStorageLockButton(NULL)
	, m_pStorageLockButtonLock(NULL)
{
}

CStorageLockButton::~CStorageLockButton ()
{
}

void CStorageLockButton::CreateSubControl ()
{
	m_pStorageLockButton = new CBasicButton;
	m_pStorageLockButton->CreateSub ( this, "STORAGE_LOCK_OPEN_BUTTON", UI_FLAG_DEFAULT, STORAGE_LOCK_OPEN_BUTTON );
	m_pStorageLockButton->CreateMouseOver( "MENU_OVER_IMAGE_STORAGELOCK" );
	m_pStorageLockButton->SetUseGlobalAction ( TRUE );
	RegisterControl ( m_pStorageLockButton );

	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, "STORAGE_LOCK_OPEN_BUTTON_F" );
	pControl->SetVisibleSingle ( FALSE );
	RegisterControl ( pControl );
	m_pStorageLockButtonLock = pControl;
}
void CStorageLockButton::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	BOOL bEnabled = GLGaeaClient::GetInstance().GetCharacter()->m_bEnableStorageLock;
	BOOL bLocked = GLGaeaClient::GetInstance().GetCharacter()->m_bStorageLockStatus;
	if ( bEnabled && bLocked ) m_pStorageLockButtonLock->SetVisibleSingle ( TRUE );
	if ( bEnabled && !bLocked ) m_pStorageLockButtonLock->SetVisibleSingle ( FALSE );
}
void CStorageLockButton::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case STORAGE_LOCK_OPEN_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				BOOL bEnabled = GLGaeaClient::GetInstance().GetCharacter()->m_bEnableStorageLock;
				BOOL bLocked = GLGaeaClient::GetInstance().GetCharacter()->m_bStorageLockStatus;

				if ( !bEnabled ) CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Enable Storage Lock", NS_UITEXTCOLOR::WHITE  );
				if ( bEnabled && bLocked ) CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Unlock Storage", NS_UITEXTCOLOR::WHITE  );
				if ( bEnabled && !bLocked ) CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Lock Storage", NS_UITEXTCOLOR::WHITE  );
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				BOOL bEnabled = GLGaeaClient::GetInstance().GetCharacter()->m_bEnableStorageLock;
				BOOL bLocked = GLGaeaClient::GetInstance().GetCharacter()->m_bStorageLockStatus;

				if ( !bEnabled ) CInnerInterface::GetInstance().ShowGroupFocus( STORAGE_LOCK_ENABLE_WINDOW );
				if ( bEnabled && bLocked ) CInnerInterface::GetInstance().OpenStorageLockInput();
				if ( bEnabled && !bLocked ) CInnerInterface::GetInstance().OpenStorageLockInput();
			}
		}
		break;
	}
}
