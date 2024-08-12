#pragma once

#include "../Lib -- Engine/GUInterface/UIGroup.h"
#include "./BasicButton.h"

class CBasicButton;

class	CStorageLockButton : public CUIGroup
{
protected:
	enum
	{
		STORAGE_LOCK_OPEN_BUTTON = NO_ID + 1,
	};

public:
	CStorageLockButton ();
	virtual	~CStorageLockButton ();

public:
	void	CreateSubControl ();
	CBasicButton* m_pStorageLockButton;
	CUIControl*	  m_pStorageLockButtonLock;

public:
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

};