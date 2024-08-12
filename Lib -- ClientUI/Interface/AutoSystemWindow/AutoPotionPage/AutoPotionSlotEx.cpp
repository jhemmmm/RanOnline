/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoPotionSlotEx.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "AutoPotionSlotEx.h"
#include "AutoPotionTray.h"

#include "../../BasicLineBox.h"
#include "../../GameTextControl.h"
#include "../../UITextControl.h"
#include "../../InnerInterface.h"
#include "../../ItemImage.h"

#include "../Lib -- ClientUI/Interface/BasicTextBox.h"
#include "../Lib -- Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAutoPotionSlotEx::CAutoPotionSlotEx()
	: m_nGSlotIndex(-1)
	, m_nSlotIndex(-1)
	, m_nTrayIndex(-1)
	, m_pLBox(NULL)
{
}

void CAutoPotionSlotEx::CreateSubControl(int nTrayIndex, int nSlotIndex)
{
	m_nTrayIndex = nTrayIndex;
	m_nSlotIndex = nSlotIndex;

	m_nGSlotIndex = (m_nTrayIndex * nMAXPOTION_SLOT) + (m_nSlotIndex + 6);

	m_pLBox = new CBasicLineBox;
	m_pLBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pLBox->CreateBaseBoxOuter("AUTOPOTION_SLOT_LBOX");
	RegisterControl(m_pLBox);

	CreatePotionImage("AUTOPOTION_SLOT_IMAGE");
	CreateMouseOver("AUTOPOTION_SLOT_MOUSE_OVER");
	m_pPotionMouseOver->SetVisibleSingle(FALSE);

	m_pMiniText = CreateTextBox("AUTOPOTION_SLOT_AMOUNT");
	m_pMiniText->SetTextAlign(TEXT_ALIGN_RIGHT);
	const D3DXVECTOR2 vPos(m_pMiniText->GetLocalPos().left, m_pMiniText->GetLocalPos().top);
	m_pMiniText->SetLocalPos(D3DXVECTOR2(vPos.x - 3.0f, vPos.y));

	SetSlotIndex(m_nGSlotIndex);
}

void CAutoPotionSlotEx::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CAutoPotionSlot::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	int nIndex = GetSlotIndex();
	GASSERT(0 <= nIndex && nIndex < EMACTIONQUICK_SIZE && "¹üÀ§¸¦ ¹þ¾î³³´Ï´Ù.");

	SACTION_SLOT sSlot = GLGaeaClient::GetInstance().GetCharacter()->m_sACTIONQUICK[nIndex];
	BOOL bSlotEmpty = (sSlot.sNID == NATIVEID_NULL());
	DWORD dwMsg = GetMessageEx();
	if (m_pPotionMouseOver)
	{
		if (!bSlotEmpty && CHECK_MOUSE_IN(dwMsg) && bFirstControl)
		{
			m_pPotionMouseOver->SetVisibleSingle(TRUE);
		}
		else
		{
			m_pPotionMouseOver->SetVisibleSingle(FALSE);
		}
	}
}

UIRECT CAutoPotionSlotEx::GetAbsPosPotionImage()
{
	return m_pItemImage->GetGlobalPos();
}