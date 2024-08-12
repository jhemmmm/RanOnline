#include "StdAfx.h"

#include "PersonalLockWindow.h"
#include "EquipmentLockButton.h"
#include "InventoryLockButton.h"
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

CPersonalLockWindow::CPersonalLockWindow ()
	: m_pEquipmentLockButton ( NULL )
	, m_pInventoryLockButton ( NULL )
	, m_pStorageLockButton ( NULL )
{
}

CPersonalLockWindow::~CPersonalLockWindow ()
{
}

void CPersonalLockWindow::CreateSubControl ()
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, "PERSONAL_LOCK_BG", UI_FLAG_DEFAULT, PERSONAL_LOCK_BOX );
	RegisterControl ( pControl );

	m_pEquipmentLockButton = new CEquipmentLockButton;
	m_pEquipmentLockButton->CreateSub ( this, "EQUIPMENT_LOCK_BUTTON", UI_FLAG_DEFAULT, EQUIPMENT_LOCK_BUTTON );
	m_pEquipmentLockButton->CreateSubControl ();
	RegisterControl ( m_pEquipmentLockButton );

	m_pInventoryLockButton = new CInventoryLockButton;
	m_pInventoryLockButton->CreateSub ( this, "INVENTORY_LOCK_BUTTON", UI_FLAG_DEFAULT, INVENTORY_LOCK_BUTTON );
	m_pInventoryLockButton->CreateSubControl ();
	RegisterControl ( m_pInventoryLockButton );

	m_pStorageLockButton = new CStorageLockButton;
	m_pStorageLockButton->CreateSub ( this, "STORAGE_LOCK_BUTTON", UI_FLAG_DEFAULT, STORAGE_LOCK_BUTTON );
	m_pStorageLockButton->CreateSubControl ();
	RegisterControl ( m_pStorageLockButton );
}
void CPersonalLockWindow::CheckMouseState ()
{
	const DWORD dwMsg = GetMessageEx ();
	if ( CHECK_MOUSE_IN ( dwMsg ) )
	{
		if( UIMSG_LB_DUP & dwMsg )
		{
			AddMessageEx( UIMSG_PERSONAL_LOCK_WINDOW_LBDUP );
			SetDiffuse ( D3DCOLOR_ARGB ( 0xFF, 0xFF, 0xFF, 0xFF ) );
			return ;
		}

		if ( dwMsg & UIMSG_LB_DOWN )
		{
			SetExclusiveControl();	

			if ( !m_bFirstGap )
			{
				UIRECT rcPos = GetGlobalPos ();
				m_vGap.x = m_nPosX - rcPos.left;
				m_vGap.y = m_nPosY - rcPos.top;
				m_bFirstGap = TRUE;

				SetDiffuse ( D3DCOLOR_ARGB ( 0xB4, 0xFF, 0xFF, 0xFF ) );
			}
		}
		else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
		{
			ResetExclusiveControl();
			m_bFirstGap = FALSE;				

			SetDiffuse ( D3DCOLOR_ARGB ( 0xFF, 0xFF, 0xFF, 0xFF ) );
		}
	}
	else if ( CHECK_LB_UP_LIKE ( dwMsg ) )		
	{								
		ResetExclusiveControl();
		m_bFirstGap = FALSE;					

		SetDiffuse ( D3DCOLOR_ARGB ( 0xFF, 0xFF, 0xFF, 0xFF ) );
	}
}
void CPersonalLockWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_bCHECK_MOUSE_STATE = false;
	m_nPosX = x;
	m_nPosY = y;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( IsExclusiveSelfControl() ){
		SetGlobalPos ( D3DXVECTOR2 ( x - m_vGap.x, y - m_vGap.y ) );
	}
}
void CPersonalLockWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	if ( !m_bCHECK_MOUSE_STATE )
	{
		CheckMouseState ();
		m_bCHECK_MOUSE_STATE = true;
	}
}
