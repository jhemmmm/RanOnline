/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoPotionPage.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "../../UIWindowEx.h"

class CBasicLineBox;
class CBasicTextButton;
class CAutoPotionTray;
class CBasicProgressBar;
class CBasicTextBox;
class CAutoPotionPage : public CUIWindowEx
{
public:
	static const int nCONTROLPOTION;

protected:
	enum
	{
		nMAXLBOX		= 9,
		nMAXBUTTON		= 2,
		nMAXBG			= 1,
		nMAXPAGE		= 2,
		nMAXBAR			= 3,
		nMAXTEXT_MID	= 4,
		nMAXTEXT_LEFT	= 12,
		nMAXCONTROL		= 3
	};

	enum
	{
		BUTTON_0 = ET_CONTROL_NEXT,
		BUTTON_1,

		PAGE_0,
		PAGE_1,

		CONTROL_0,
		CONTROL_1,
		CONTROL_2
	};

public:
	CAutoPotionPage();
	void CreateSubControl();
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

private:
	CBasicLineBox*		m_pLBox_[nMAXLBOX];
	CBasicTextButton*	m_pButton_[nMAXBUTTON];
	CUIControl*			m_pBG_[nMAXBG];
	CAutoPotionTray*	m_pAutoPotionTray_[nMAXPAGE];
	CBasicProgressBar*	m_pAutoPotionBar_[nMAXBAR];
	CBasicTextBox*		m_pTextMid_[nMAXTEXT_MID];
	CBasicTextBox*		m_pTextLeft_[nMAXTEXT_LEFT];
	CUIControl*			m_pControl_[nMAXCONTROL];

	int m_nCurPage;
	int m_nHPSet;
	int m_nMPSet;
	int m_nSPSet;

private:
	CBasicTextBox* CreateStaticControl(char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID);
	void SetLBox(int nIndex);

public:
	void	SetTab(int nIndex);
	int		GetCurPotionTab()	{ return m_nCurPage; }
	BOOL	CheckPotionTab(int nTab);
	int		GetPotionSettings(int nIndex);
};