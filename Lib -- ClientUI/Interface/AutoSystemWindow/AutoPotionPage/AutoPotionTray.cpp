/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoPotionTray.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "AutoPotionTray.h"
#include "AutoPotionSlotEx.h"

#include "../../BasicLineBox.h"
#include "../../InnerInterface.h"
#include "../../ItemMove.h"

#include "../Lib -- Client/G-Logic/GLGaeaClient.h"
#include "../Lib -- Client/G-Logic/GLItemMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CAutoPotionTray::nOUTOFRANGE = -1;

CAutoPotionTray::CAutoPotionTray()
	: m_pLBox(NULL)
{
	memset(m_pAutoPotionEx_,	NULL, sizeof(m_pAutoPotionEx_));
}

void CAutoPotionTray::CreateSubControl(int nTrayIndex)
{
	m_pLBox = new CBasicLineBox;
	m_pLBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pLBox->CreateBaseBoxOuter("AUTOPOTION_TRAY_LBOX");
	RegisterControl(m_pLBox);

	char strTemp[50];

	for (int i = 0; i < nMAXPOTION_SLOT; i++)
	{
		sprintf(strTemp, "AUTOPOTION_TRAY_SLOT_%d", i);

		m_pAutoPotionEx_[i] = new CAutoPotionSlotEx;
		m_pAutoPotionEx_[i]->CreateSub(this, strTemp, UI_FLAG_DEFAULT, POTION_SLOT_0 + i);
		m_pAutoPotionEx_[i]->CreateSubControl(nTrayIndex, i);
		RegisterControl(m_pAutoPotionEx_[i]);
	}
}

void CAutoPotionTray::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	if (!IsVisible()) return;
	SetSlotIndex(nOUTOFRANGE);
	CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
}

void CAutoPotionTray::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
		case POTION_SLOT_0:
		case POTION_SLOT_1:
		case POTION_SLOT_2:
		{
			if (CHECK_MOUSE_IN(dwMsg))
			{
				int nG_Index	= (m_pAutoPotionEx_[0]->GetTrayIndex() * nMAXPOTION_SLOT) + 6;
				int nIndex		= ControlID - POTION_SLOT_0;
				nG_Index += nIndex;

				SetSlotIndex(nG_Index);

				if (!CInnerInterface::GetInstance().IsFirstItemSlot()) return;

				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove();
				if (!pItemMove)
				{
					GASSERT(0 && "CItemMove가 널입니다.");
					return;
				}

				if (pItemMove->GetItem() != NATIVEID_NULL())
				{
					const UIRECT& rcSlotPotionImagePos = m_pAutoPotionEx_[nIndex]->GetAbsPosPotionImage();
					pItemMove->SetGlobalPos(rcSlotPotionImagePos);

					AddMessageEx(UIMSG_MOUSEIN_AUTOPOTIONSLOT);
				}

				if (dwMsg & UIMSG_LB_UP)
				{
					const SITEMCUSTOM& sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_HOLD_ITEM();
					if (sItemCustom.sNativeID != NATIVEID_NULL())
					{
						GLGaeaClient::GetInstance().GetCharacter()->ReqItemQuickSet((WORD)nG_Index);
					}
					else
					{
						GLGaeaClient::GetInstance().GetCharacter()->ReqActionQ((WORD)nG_Index);
					}
				}

				if (dwMsg & UIMSG_RB_UP)
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReqItemQuickReSet((WORD)nG_Index);
				}

				SACTION_SLOT sSlot = GLGaeaClient::GetInstance().GetCharacter()->m_sACTIONQUICK[nG_Index];
				if (sSlot.sNID != NATIVEID_NULL())
				{
					SITEM* pItem = GLItemMan::GetInstance().GetItem(sSlot.sNID);
					if (pItem)
					{
						DWORD dwLevel = pItem->sBasicOp.emLevel;
						CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(pItem->GetName(), COMMENT::ITEMCOLOR[dwLevel]);
					}
				}
			}
		}
		break;
	}
}