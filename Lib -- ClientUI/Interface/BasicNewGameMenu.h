#pragma once

#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "../Lib_ClientUI/Interface/BasicButton.h"
#include "UIWindowEx.h"

class 	CBasicTextButton;
class 	CBasicButton;
class 	CBasicVarTextBox;
class	CBasicLineBoxEx;
class	CBasicButtonText;
class	CBasicLineBoxEx;
#define BASIC_MENU_NUM	12

class	CBasicNewGameMenu : public CUIGroup
{
protected:
	enum
	{
		MENU_INVENTORY = NO_ID + 1,
		MENU_CHARACTER,
		MENU_SKILL,
		MENU_PARTY,
		MENU_CLUB,
		MENU_QUEST,
		MENU_FRIEND,
		MENU_MAP,
		MENU_CHATMACRO,
		MENU_COMBINATION,
		MENU_TYRANNY,
		MENU_ESC,
		MENU_INVENTORY_BUTTON,
		MENU_CHARACTER_BUTTON,
		MENU_SKILL_BUTTON,
		MENU_PARTY_BUTTON,
		MENU_CLUB_BUTTON,
		MENU_QUEST_BUTTON,
		MENU_FRIEND_BUTTON,
		MENU_MAP_BUTTON,
		MENU_CHATMACRO_BUTTON,
		MENU_COMBINATION_BUTTON,
		MENU_TYRANNY_BUTTON,
		MENU_ESC_BUTTON,

	};

private:
	int	m_nOverMenu;
	CBasicVarTextBox* 		m_pInfo;
	CString m_ShotcutText	[BASIC_MENU_NUM];
	CBasicTextButton* 		pButton;

private:
	CBasicTextButton*			CreateTextButton18( char* szButton, UIGUID ControlID, char* szText, BOOL bFlip );
//add by indra
public:
	CBasicButtonText*			CreateButtonBlue ( char* szButton, char* szTextBox, CD3DFontPar* pFont, int nAlign, UIGUID ControlID, CString strText );
	CBasicButtonText*			m_pInvenButton;
	CBasicButtonText* 			m_pCharButton;
	CBasicButtonText*			m_pSkillButton;
	CBasicButtonText*			m_pPartyButton;
	CBasicButtonText*			m_pClubButton;
	CBasicButtonText*			m_pQuestButton;
	CBasicButtonText*			m_pFriendButton;
	CBasicButtonText*			m_pMapButton;
	CBasicButtonText*			m_pMacroButton;
	CBasicButtonText*			m_pCompunButton;
	CBasicButtonText*			m_pTyrannyButton;
	CBasicButtonText*			m_pESCButton;
public:
    CBasicNewGameMenu ();
	virtual	~CBasicNewGameMenu ();

public:
	void CreateSubControl ();
	void SetShotcutText ( DWORD nID, CString& strTemp );
	void InitShotCutString();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
};