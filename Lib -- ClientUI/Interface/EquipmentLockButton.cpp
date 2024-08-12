#include "StdAfx.h"
#include "EquipmentLockButton.h"
#include "./BasicButton.h"
#include "InnerInterface.h"
#include "RANPARAM.h"
#include "GLGaeaClient.h"
#include "GLCharacter.h"
#include "UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEquipmentLockButton::CEquipmentLockButton ()
	: m_pEquipmentLockButton(NULL)
	, m_pEquipmentLockButtonLock(NULL)
{
}

CEquipmentLockButton::~CEquipmentLockButton ()
{
}

void CEquipmentLockButton::CreateSubControl ()
{
	m_pEquipmentLockButton = new CBasicButton;
	m_pEquipmentLockButton->CreateSub ( this, "EQUIPMENT_LOCK_OPEN_BUTTON", UI_FLAG_DEFAULT, EQUIPMENT_LOCK_OPEN_BUTTON );
	m_pEquipmentLockButton->CreateMouseOver( "MENU_OVER_IMAGE_EQUIPMENTLOCK" );
	m_pEquipmentLockButton->SetUseGlobalAction ( TRUE );
	RegisterControl ( m_pEquipmentLockButton );

	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, "EQUIPMENT_LOCK_OPEN_BUTTON_F" );
	pControl->SetVisibleSingle ( FALSE );
	RegisterControl ( pControl );
	m_pEquipmentLockButtonLock = pControl;
}
void CEquipmentLockButton::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	BOOL bEnabled = GLGaeaClient::GetInstance().GetCharacter()->m_bEnableEquipmentLock;
	BOOL bLocked = GLGaeaClient::GetInstance().GetCharacter()->m_bEquipmentLockStatus;
	if ( bEnabled && bLocked ) m_pEquipmentLockButtonLock->SetVisibleSingle ( TRUE );
	if ( bEnabled && !bLocked ) m_pEquipmentLockButtonLock->SetVisibleSingle ( FALSE );
}
void CEquipmentLockButton::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case EQUIPMENT_LOCK_OPEN_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				BOOL bEnabled = GLGaeaClient::GetInstance().GetCharacter()->m_bEnableEquipmentLock;
				BOOL bLocked = GLGaeaClient::GetInstance().GetCharacter()->m_bEquipmentLockStatus;

				if ( !bEnabled ) CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Enable Equipment Lock", NS_UITEXTCOLOR::WHITE  );
				if ( bEnabled && bLocked ) CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Unlock Equipment", NS_UITEXTCOLOR::WHITE  );
				if ( bEnabled && !bLocked ) CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Lock Equipment", NS_UITEXTCOLOR::WHITE  );
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				BOOL bEnabled = GLGaeaClient::GetInstance().GetCharacter()->m_bEnableEquipmentLock;
				BOOL bLocked = GLGaeaClient::GetInstance().GetCharacter()->m_bEquipmentLockStatus;

				if ( !bEnabled ) CInnerInterface::GetInstance().ShowGroupFocus( EQUIPMENT_LOCK_ENABLE_WINDOW );
				if ( bEnabled && bLocked ) CInnerInterface::GetInstance().OpenEquipmentLockInput();
				if ( bEnabled && !bLocked ) CInnerInterface::GetInstance().OpenEquipmentLockInput();
			}
		}
		break;
	}
}
