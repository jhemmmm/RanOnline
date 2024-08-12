/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoPotionTray.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "../../UIWindowEx.h"
#include "../Lib -- Client/G-Logic/GLAutoSystem/GLAutoSystem.h"

class CBasicLineBox;
class CAutoPotionSlotEx;

const DWORD UIMSG_MOUSEIN_AUTOPOTIONSLOT = UIMSG_USER1;

class CAutoPotionTray : public CUIWindowEx
{
private:
	static const int nOUTOFRANGE;

protected:
	enum
	{
		POTION_SLOT_0 = NO_ID + 1,
		POTION_SLOT_1,
		POTION_SLOT_2,
	};

public:
	CAutoPotionTray();
	void CreateSubControl(int nIndex);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);

private:
	CBasicLineBox*		m_pLBox;
	int					m_nIndex;

public:
	CAutoPotionSlotEx*	m_pAutoPotionEx_[nMAXPOTION_SLOT];

public:
	void SetSlotIndex(int nIndex)	{ m_nIndex = nIndex; }
	int GetSlotIndex()				{ return m_nIndex; }
};