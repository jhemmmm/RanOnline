/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoFollowPage.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "../../UIWindowEx.h"

class CBasicLineBox;
class CBasicTextButton;
class CBasicTextBox;
class CAutoFollowPageUser;
class CBasicProgressBar;
class CAutoFollowPage : public CUIWindowEx
{
public:
	static const int nCONTROLRANGE;

protected:
	enum
	{
		nMAXLBOX		= 8,
		nMAXBUTTON		= 2,
		nMAXBG			= 2,
		nMAXPAGE		= 2,
		nMAXTEXT_MID	= 4,
		nMAXTEXT_LEFT	= 2,
	};

	enum
	{
		BUTTON_0	= ET_CONTROL_NEXT,
		BUTTON_1,

		PAGE_0,
		PAGE_1,

		AUTOFOLLOW_PAGE_RANGEBAR,

		BUTTON_SELECT
	};

public:
	CAutoFollowPage();
	void CreateSubControl();
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

private:
	CBasicLineBox*				m_pLBox_[nMAXLBOX];
	CBasicTextButton*			m_pButton_[nMAXBUTTON];
	CUIControl*					m_pBG_[nMAXBG];
	CBasicTextBox*				m_pTextMid_[nMAXTEXT_MID];
	CBasicTextBox*				m_pTextLeft_[nMAXTEXT_LEFT];
	CAutoFollowPageUser*		m_pAutoFollowPageUser_[nMAXPAGE];

	CBasicProgressBar*			m_pRangeBar;
	CUIControl*					m_pRangeBarDummy;

	CBasicTextButton*			m_pButtonSelect;

	int			m_nCurPage;
	int			m_nRange;
	CString		m_strName;

	static int nLIMIT_0;

private:
	void SetPage(int nPage);
	CBasicTextBox* CreateStaticControl(char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID);

	void SetSelectUser();

public:
	void RefreshFriendList();
	CString GetFollowName();
	int GetFollowRange();
};