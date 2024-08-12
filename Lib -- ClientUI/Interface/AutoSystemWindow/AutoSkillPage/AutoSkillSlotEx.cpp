/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoSkillSlotEx.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "AutoSkillSlotEx.h"
#include "AutoSkillSlot.h"
#include "AutoSkillTray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAutoSkillSlotEx::CAutoSkillSlotEx()
	: m_nTrayIndex(-1)
	, m_nSlotIndex(-1)
	, m_pAutoSkillSlot(NULL)
{
}

void CAutoSkillSlotEx::CreateSubControl(int nTrayIndex, int nSlotIndex)
{
	m_nTrayIndex = nTrayIndex;
	m_nSlotIndex = nSlotIndex;

	m_pAutoSkillSlot = new CAutoSkillSlot;
	m_pAutoSkillSlot->CreateSub(this, "AUTOSKILL_SLOT", UI_FLAG_DEFAULT);
	m_pAutoSkillSlot->CreateSubControl();
	m_pAutoSkillSlot->SetUseSkillImageTwinkle(false);
	m_pAutoSkillSlot->m_pMouseOver->SetVisibleSingle(FALSE);
	RegisterControl(m_pAutoSkillSlot);
}

void CAutoSkillSlotEx::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	int nG_Index	= (m_nTrayIndex + 3) * nMAXSKILL_SLOT;
	nG_Index		+= m_nSlotIndex;

	SNATIVEID sNativeID = GLGaeaClient::GetInstance().GetCharacter()->m_sSKILLQUICK[nG_Index];
	m_pAutoSkillSlot->UpdateSlot(sNativeID);

	BOOL bSlotEmpty	= (sNativeID == NATIVEID_NULL());
	DWORD dwMsg		= GetMessageEx();
	if (m_pAutoSkillSlot->m_pMouseOver)
	{
		if (!bSlotEmpty && CHECK_MOUSE_IN(dwMsg) && bFirstControl)
		{
			m_pAutoSkillSlot->m_pMouseOver->SetVisibleSingle(TRUE);
		}
		else
		{
			m_pAutoSkillSlot->m_pMouseOver->SetVisibleSingle(FALSE);
		}
	}
}
