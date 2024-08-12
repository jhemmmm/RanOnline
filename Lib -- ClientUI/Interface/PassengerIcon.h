#pragma	once

#include "../Lib -- Engine/GUInterface/UIGroup.h"

#include "../Lib -- Engine/GUInterface/UIMan.h"
#include "GLCrowData.h"
class CPassenger : public CUIGroup
{
public:
	enum
	{
		MouseOpen = NO_ID + 1,
		MENU_CLOSE_BUTTON,
	};

public:
	CPassenger();
	virtual ~CPassenger();

	void	CreateSubControl ();
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	void	SetPassenger();
	void	ReSetPassenger();

private:
	CBasicButton* m_pImage;
};