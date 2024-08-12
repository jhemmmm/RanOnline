#pragma	once

#include "../Lib -- Engine/GUInterface/UIGroup.h"

class CBasicTextBox;
class CBasicTextButton;
struct SCHARINFO_LOBBY;

class CSelectCharacterButton : public CUIGroup
{
private:
	enum
	{
		SELECT_CHARACTER_NEW = NO_ID + 1,
		SELECT_CHARACTER_DELETE,
		SELECT_CHARACTER_CHANGESERVER,
		SELECT_CHARACTER_GAMESTART,
	};

	
public:
	CSelectCharacterButton ();
	virtual	~CSelectCharacterButton ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void	ResetData();

public:
	SCHARINFO_LOBBY * GetDelCharInfo()				{ return m_pDelCharInfo; }

public:
	int					m_nCharRemainLast;
	int					m_nServerGroup;
	int					m_nServerChannel;

private:
	SCHARINFO_LOBBY*	m_pDelCharInfo;
	CBasicTextButton*	m_pNewButton;
	CBasicTextButton*	m_pDeleteButton;
	CBasicTextButton*	m_pChangeServerButton;
	CBasicTextButton*	m_pGameStartButton;
	CBasicTextBox*		m_pServerName;

};