/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoSkillPage.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "../../UIWindowEx.h"

class CBasicLineBox;
class CBasicTextButton;
class CBasicTextBox;
class CBasicProgressBar;
class CUIEditBoxMan;

class CAutoSkillTray;

class CAutoSkillPage : public CUIWindowEx
{
public:
	static const int nCONTROLRANGE;

protected:
	enum
	{
		nMAXLBOX		= 7,
		nMAXBUTTON		= 5,
		nMAXBG			= 2,
		nMAXTEXT_MID	= 3,
		nMAXTEXT_LEFT	= 5,
		nMAXPAGE		= 3
	};

	enum
	{
		BUTTON_0 = ET_CONTROL_NEXT,
		BUTTON_1,
		BUTTON_2,
		BUTTON_3,
		BUTTON_4,

		PAGE_0,
		PAGE_1,
		PAGE_2,

		AUTOSKILL_PAGE_EDITMAN,
		AUTOSKILL_PAGE_EDIT_0,
		AUTOSKILL_PAGE_EDIT_1,
		AUTOSKILL_PAGE_RANGEBAR
	};

public:
	CAutoSkillPage();
	void CreateSubControl();
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

private:
	CBasicLineBox*		m_pLBox_[nMAXLBOX];
	CBasicTextButton*	m_pButton_[nMAXBUTTON];
	CUIControl*			m_pBG_[nMAXBG];
	CBasicTextBox*		m_pTextMid_[nMAXTEXT_MID];
	CBasicTextBox*		m_pTextLeft_[nMAXTEXT_LEFT];
	CAutoSkillTray*		m_pAutoSkillTray_[nMAXPAGE];
	CUIEditBoxMan*		m_pEditBoxMan;

	CBasicProgressBar*	m_pRangeBar;
	CUIControl*			m_pRangeBarDummy;

	int m_nCurPage1;
	int m_nCurPage2;

	int m_nRange;

	static int nLIMIT_0;

private:
	CBasicTextBox* CreateStaticControl(char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID);

public:
	void	SetTab1(int nIndex);
	void	SetTab2(int nIndex);

	int		GetCurSkillTab();
	int		GetLevMin();
	int		GetLevMax();

	BOOL	CheckSkillTab(int nTab);

	int		GetRange()			{ return m_nRange; }
};