#pragma once

#include "../Lib -- Engine/GUInterface/UIGroup.h"
#include "./BasicButton.h"

class CBasicButton;

class	CEquipmentLockButton : public CUIGroup
{
protected:
	enum
	{
		EQUIPMENT_LOCK_OPEN_BUTTON = NO_ID + 1,
	};

public:
	CEquipmentLockButton ();
	virtual	~CEquipmentLockButton ();

public:
	void	CreateSubControl ();
	CBasicButton* m_pEquipmentLockButton;
	CUIControl*	  m_pEquipmentLockButtonLock;

public:
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

};