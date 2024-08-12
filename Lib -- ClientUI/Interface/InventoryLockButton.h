#pragma once

#include "../Lib -- Engine/GUInterface/UIGroup.h"
#include "./BasicButton.h"

class CBasicButton;

class	CInventoryLockButton : public CUIGroup
{
protected:
	enum
	{
		INVENTORY_LOCK_OPEN_BUTTON = NO_ID + 1,
	};

public:
	CInventoryLockButton ();
	virtual	~CInventoryLockButton ();

public:
	void	CreateSubControl ();
	CBasicButton* m_pInventoryLockButton;
	CUIControl*	  m_pInventoryLockButtonLock;

public:
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

};