/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoSystemWindow.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "../UIWindowEx.h"

class CBasicLineBox;
class CBasicTextBox;
class CBasicTextButton;

class CAutoSkillPage;
class CAutoPotionPage;
class CAutoFollowPage;

struct SAUTO_FOLLOW;
class CAutoSystemWindow : public CUIWindowEx
{
protected:
	enum
	{
		nMAXLBOX	= 6,
		nMAXTEXT	= 3,
		nMAXBUTTON	= 6,
		nMAXICON	= 3
	};

	enum
	{
		BUTTON_0 = ET_CONTROL_NEXT,
		BUTTON_1,
		BUTTON_2,
		BUTTON_3,
		BUTTON_4,
		BUTTON_5,

		PAGE_0,
		PAGE_1,
		PAGE_2
	};

public:
	CAutoSystemWindow();
	void CreateSubControl();
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual	void SetVisibleSingle(BOOL bVisible);

private:
	CUIControl*			m_pWhiteBG;
	CUIControl*			m_pIconBG_[nMAXICON];
	CBasicLineBox*		m_pLBox_[nMAXLBOX];
	CBasicTextBox*		m_pText_[nMAXTEXT];
	CBasicTextButton*	m_pButton_[nMAXBUTTON];

	int					m_nCurPage;

	CAutoSkillPage*		m_pAutoSkillPage;
	CAutoPotionPage*	m_pAutoPotionPage;
	CAutoFollowPage*	m_pAutoFollowPage;

	BOOL				m_bAutoSKILL;
	BOOL				m_bAutoPOTION;
	BOOL				m_bAutoFOLLOW;

private:
	CBasicTextBox* CreateStaticControl(char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID);
	void ShowPage(int nIndex);
	void ResetPos();
	void SetAutoSystem(int nIndex);

public:
	CAutoFollowPage*	GetAutoFollowPage()	{ return m_pAutoFollowPage; }

	void SetAutoSKILL(BOOL bValue);
	void SetAutoPOTION(BOOL bValue);
	void SetAutoFOLLOW(BOOL bValue);

	void StartAutoFOLLOW(SAUTO_FOLLOW sAutoFollow);
	void StopAutoFollow();

	void ReRunAutoSkill();
	void ReRunAutoPotion();
};