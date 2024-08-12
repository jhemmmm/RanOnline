#pragma once

#include "../Lib -- Engine/GUInterface/UIGroup.h"
#include "./BasicButton.h"

class CBasicButton;
class CEquipmentLockButton;
class CInventoryLockButton;
class CStorageLockButton;

const DWORD UIMSG_PERSONAL_LOCK_WINDOW_LBDUP = UIMSG_USER1;

class	CPersonalLockWindow : public CUIGroup
{
protected:
	enum
	{
		PERSONAL_LOCK_BOX = NO_ID + 1,
		EQUIPMENT_LOCK_BUTTON,
		INVENTORY_LOCK_BUTTON,
		STORAGE_LOCK_BUTTON,
	};

public:
	CPersonalLockWindow ();
	virtual	~CPersonalLockWindow ();

public:
	void	CheckMouseState();
	void	CreateSubControl ();
	
	CEquipmentLockButton*		m_pEquipmentLockButton;
	CInventoryLockButton*		m_pInventoryLockButton;
	CStorageLockButton*			m_pStorageLockButton;

private:
	BOOL		m_bCHECK_MOUSE_STATE;
	BOOL		m_bFirstGap;
	D3DXVECTOR2	m_vGap;
	int			m_nPosX;
	int			m_nPosY;

public:
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

};