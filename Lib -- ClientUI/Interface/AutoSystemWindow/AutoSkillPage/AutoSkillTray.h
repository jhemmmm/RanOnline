/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoSkillTray.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "../../UIWindowEx.h"

#include "../Lib -- Client/G-Logic/GLGaeaClient.h"

class CBasicLineBox;
class CAutoSkillSlotEx;
class CBasicVarTextBox;

class CAutoSkillTray : public CUIWindowEx
{
private:
	static const int nLBUPSKIP;

protected:
	enum
	{
		nMAXLBOX = 1,		
	};

	enum
	{
		SKILL_SLOT_0	= NO_ID + 1,
		SKILL_SLOT_1,
		SKILL_SLOT_2,
		SKILL_SLOT_3,
		SKILL_SLOT_4,
		SKILL_SLOT_5,
		SKILL_SLOT_6,
		SKILL_SLOT_7,
		SKILL_SLOT_8,
		SKILL_SLOT_9
	};

public:
	CAutoSkillTray();
	void CreateSubControl(int nIndex);
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

private:
	int					m_nLBUPSKIP;

	CBasicLineBox*		m_pLBox;
	CAutoSkillSlotEx*	m_pAutoSkillSlotEx_[nMAXSKILL_SLOT];
	CBasicVarTextBox*	m_pSkillInfo;

private:
	void UpdateSkillInfo(SNATIVEID sNativeID);
	void LoadSkillInfo(PGLSKILL const pSkill, SCHARSKILL* const pCharSkill);
};