/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoSkillPage.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "../../UIWindowEx.h"

class CAutoSkillSlot;

class CAutoSkillSlotEx : public CUIWindowEx
{
public:
	CAutoSkillSlotEx();
	void CreateSubControl(int nTrayIndex, int nSlotIndex);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

private:
	int				m_nTrayIndex;
	int				m_nSlotIndex;

	CAutoSkillSlot*	m_pAutoSkillSlot;

public:
	int GetTrayIndex() { return m_nTrayIndex; }

	CAutoSkillSlot* GetAutoSkillSlot() { return m_pAutoSkillSlot; }
};