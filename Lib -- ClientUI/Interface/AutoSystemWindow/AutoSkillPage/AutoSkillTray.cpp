/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoSkillTray.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "AutoSkillTray.h"
#include "AutoSkillSlotEx.h"
#include "AutoSkillSlot.h"
#include "AutoSkillPageToTray.h"

#include "../../BasicLineBox.h"
#include "../../InnerInterface.h"
#include "../../BasicVarTextBox.h"
#include "../../UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CAutoSkillTray::nLBUPSKIP = 1;

CAutoSkillTray::CAutoSkillTray()
	: m_pLBox(NULL)
	, m_nLBUPSKIP(0)
	, m_pSkillInfo(NULL)
{
	memset(m_pAutoSkillSlotEx_, NULL, sizeof(m_pAutoSkillSlotEx_));
}

void CAutoSkillTray::CreateSubControl(int nTrayIndex)
{
	m_pLBox = new CBasicLineBox;
	m_pLBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pLBox->CreateBaseBoxOuter("AUTOSKILL_TRAY_LBOX");
	RegisterControl(m_pLBox);

	char strTemp[50];

	for (int i = 0; i < nMAXSKILL_SLOT; i++)
	{
		sprintf(strTemp, "AUTOSKILL_TRAY_SLOT_%d", i);

		m_pAutoSkillSlotEx_[i] = new CAutoSkillSlotEx;
		m_pAutoSkillSlotEx_[i]->CreateSub(this, strTemp, UI_FLAG_DEFAULT, SKILL_SLOT_0 + i);
		m_pAutoSkillSlotEx_[i]->CreateSubControl(nTrayIndex, i);
		RegisterControl(m_pAutoSkillSlotEx_[i]);
	}

	m_pSkillInfo = new CBasicVarTextBox;
	m_pSkillInfo->CreateSub(this, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pSkillInfo->CreateSubControl();
	m_pSkillInfo->SetVisibleSingle(FALSE);
	RegisterControl(m_pSkillInfo);
}

void CAutoSkillTray::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case SKILL_SLOT_0:
	case SKILL_SLOT_1:
	case SKILL_SLOT_2:
	case SKILL_SLOT_3:
	case SKILL_SLOT_4:
	case SKILL_SLOT_5:
	case SKILL_SLOT_6:
	case SKILL_SLOT_7:
	case SKILL_SLOT_8:
	case SKILL_SLOT_9:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			int nG_Index	= (m_pAutoSkillSlotEx_[0]->GetTrayIndex() + 3) * nMAXSKILL_SLOT;
			int nIndex		= ControlID - SKILL_SLOT_0;
			nG_Index		+= nIndex;

			SNATIVEID sOverSkill = GLGaeaClient::GetInstance().GetCharacter()->GetCharData().m_sSKILLQUICK[nG_Index];
			if (sOverSkill != NATIVEID_NULL())
			{
				UpdateSkillInfo(sOverSkill);
				m_pSkillInfo->SetVisibleSingle(TRUE);
			}

			CAutoSkillPageToTray* pAutoSkillPageToTray = CInnerInterface::GetInstance().GetAutoSkillPageToTray();
			if (!pAutoSkillPageToTray)
			{
				GASSERT(0 && "CAutoSkillPageToTray가 널입니다.");

				return;
			}

			if (pAutoSkillPageToTray->GetSkill() != NATIVEID_NULL())
			{
				const UIRECT& rcSlotPos = m_pAutoSkillSlotEx_[nIndex]->GetAutoSkillSlot()->GetAbsPosSkillImage();
				pAutoSkillPageToTray->SetGlobalPos(rcSlotPos);
			}

			if (dwMsg & UIMSG_LB_UP)
			{
				SNATIVEID sNativeID = pAutoSkillPageToTray->GetSkill();

				if (sNativeID != NATIVEID_NULL())
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReqSkillQuickSet(sNativeID, nG_Index);
					pAutoSkillPageToTray->ResetSkill();
					m_nLBUPSKIP = 0;
				}
				else
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReqSkillRunSet(nG_Index);
				}
			}

			if (dwMsg & UIMSG_RB_UP)
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqSkillQuickReSet(nG_Index);
			}
		}
	}
	break;
	}
}

void CAutoSkillTray::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	if (!IsVisible()) return;

	m_pSkillInfo->SetVisibleSingle(FALSE);

	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	DWORD dwMsg = GetMessageEx();
	if (dwMsg & UIMSG_LB_UP)
	{
		CAutoSkillPageToTray* pAutoSkillPageToTray = CInnerInterface::GetInstance().GetAutoSkillPageToTray();
		if (!pAutoSkillPageToTray)	return;

		BOOL bExist = (pAutoSkillPageToTray->GetSkill() != NATIVEID_NULL());
		if (bExist)
		{
			if (nLBUPSKIP <= m_nLBUPSKIP)
			{
				pAutoSkillPageToTray->ResetSkill();
				m_nLBUPSKIP = 0;
			}
			else
			{
				m_nLBUPSKIP++;
			}
		}
	}
}

void CAutoSkillTray::UpdateSkillInfo(SNATIVEID sNativeID)
{
	if (sNativeID != NATIVEID_NULL())
	{
		static SNATIVEID sNativeIDBack = NATIVEID_NULL();

		BOOL bUPDATE = FALSE;
		if (sNativeID != sNativeIDBack)
		{
			bUPDATE = TRUE;
		}

		if (bUPDATE)
		{
			PGLSKILL pSkill = GLSkillMan::GetInstance().GetData(sNativeID.wMainID, sNativeID.wSubID);
			if (pSkill)
			{
				SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL(sNativeID);
				LoadSkillInfo(pSkill, pCharSkill);
			}

			sNativeIDBack = sNativeID;
		}
	}
}

void CAutoSkillTray::LoadSkillInfo(PGLSKILL const pSkill, SCHARSKILL* const pCharSkill)
{
	m_pSkillInfo->SetTextNoSplit(pSkill->GetName(), NS_UITEXTCOLOR::PALEGREEN);
}