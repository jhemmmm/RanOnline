#pragma once

#include "../Lib -- Engine/GUInterface/UIGroup.h"
#include "../Lib -- ClientUI/Interface/BasicButton.h"

class	CEscMenuOpen : public CUIGroup
{
protected:
	enum
	{
		ESCMENU_OPEN_BUTTON = NO_ID + 1,
	};

public:
	CEscMenuOpen ();
	virtual	~CEscMenuOpen ();

public:
	void	CreateSubControl ();

public:
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
};