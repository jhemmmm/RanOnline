//	기본 정보 표시
//
//	최초 작성자 : 성기엽
//	이후 수정자 : 
//	로그
//		[2003.11.21]
//			@ 작성
//

#pragma	once

#include "../Lib -- Engine/GUInterface/UIGroup.h"

class	CBasicTextBox;
class	CBasicTextButton;
class	CSelectCharacterUserPanelPage;
class	CSelectCharacterUserPanelButton;

const DWORD UIMSG_MOUSEIN_SELECT_USERPANEL = UIMSG_USER1;

class CSelectCharacterUserPanel : public CUIGroup
{
private:
	enum
	{
		SELECT_CHARACTER_USERPANEL_BTN = NO_ID + 1,
	};
public:
	CSelectCharacterUserPanel ();
	virtual	~CSelectCharacterUserPanel ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	CSelectCharacterUserPanelPage*	m_pSelectCharacterUserPanelPage;
	CSelectCharacterUserPanelButton*	m_pSelectCharacterUserPanelButton;
};