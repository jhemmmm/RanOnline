/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoPotionSlotEx.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "AutoPotionSlot.h"

class BasicTextBox;
class CBasicLineBox;
class CAutoPotionSlotEx : public CAutoPotionSlot
{
public:
	CAutoPotionSlotEx();
	void CreateSubControl(int ntrayIndex, int nSlotIndex);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

private:
	int m_nTrayIndex;
	int m_nSlotIndex;
	int m_nGSlotIndex;

	CBasicLineBox* m_pLBox;

public:
	UIRECT	GetAbsPosPotionImage();
	int GetTrayIndex() { return m_nTrayIndex; }
};