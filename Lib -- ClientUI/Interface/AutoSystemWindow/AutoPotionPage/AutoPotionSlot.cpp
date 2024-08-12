/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoPotionSlot.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "AutoPotionSlot.h"

#include "../../ItemImage.h"
#include "../../UITextControl.h"

#include "../Lib -- Engine/DxCommon/d3dfont.h"
#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "../Lib -- ClientUI/Interface/BasicTextBox.h"
#include "../Lib -- Client/G-Logic/GLItemMan.h"
#include "../Lib -- Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const D3DCOLOR CAutoPotionSlot::dwDEFAULT_TRANSPARENCY	= D3DCOLOR_ARGB(160, 255, 255, 255);;
const D3DCOLOR CAutoPotionSlot::dwFULL_TRANSPARENCY		= D3DCOLOR_ARGB(255, 255, 255, 255);;

CAutoPotionSlot::CAutoPotionSlot()
	: m_pPotionMouseOver(NULL)
	, m_pMiniText(NULL)
	, m_pd3dDevice(NULL)
	, m_nIndex(0)
	, m_pItemImage(NULL)
	, m_sNativeID(NATIVEID_NULL())
{
}

void CAutoPotionSlot::CreateSubControl()
{
	CreatePotionImage("AUTOPOTION_SLOT_IMAGE");
	CreateMouseOver("AUTOPOTION_SLOT_MOUSE_OVER");

	m_pMiniText = CreateTextBox("AUTOPOTION_SLOT_AMOUNT_SINGLE");
	m_pMiniText->SetTextAlign(TEXT_ALIGN_RIGHT);

	SetSlotIndex(0);
}

void CAutoPotionSlot::CreatePotionImage(char* szImage)
{
	m_pItemImage = new CItemImage;
	m_pItemImage->CreateSub(this, szImage);
	m_pItemImage->CreateSubControl();
	m_pItemImage->SetUseRender(TRUE);
	m_pItemImage->SetVisibleSingle(FALSE);
	RegisterControl(m_pItemImage);
}

HRESULT CAutoPotionSlot::InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects(pd3dDevice);
	if (FAILED(hr)) return hr;

	m_pd3dDevice = pd3dDevice;

	return S_OK;
}

void CAutoPotionSlot::CreateMouseOver(char* szMouseOver)
{
	m_pPotionMouseOver = new CUIControl;
	m_pPotionMouseOver->CreateSub(this, szMouseOver, UI_FLAG_DEFAULT, AUTOPOTION_MOUSE_OVER);
	m_pPotionMouseOver->SetVisibleSingle(TRUE);
	RegisterControl(m_pPotionMouseOver);
}

CBasicTextBox* CAutoPotionSlot::CreateTextBox(char* szTextBox)
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII);

	CBasicTextBox * pTextBox = new CBasicTextBox;
	pTextBox->CreateSub(this, szTextBox);
	pTextBox->SetFont(pFont8);
	RegisterControl(pTextBox);
	return pTextBox;
}

void CAutoPotionSlot::SetItem(SNATIVEID sICONINDEX, const char* szTexture, int nAmount)
{
	if (m_pItemImage)
	{
		m_pItemImage->SetItem(sICONINDEX, szTexture, m_sNativeID);
		m_pItemImage->SetVisibleSingle(TRUE);

		CString strNumber;
		strNumber.Format("%d", nAmount);
		if (m_pMiniText) m_pMiniText->SetOneLineText(strNumber, NS_UITEXTCOLOR::WHITE);

		if (nAmount <= 0)	m_pItemImage->SetDiffuse(dwDEFAULT_TRANSPARENCY);
		else				m_pItemImage->SetDiffuse(dwFULL_TRANSPARENCY);
	}
}

void CAutoPotionSlot::ResetItem()
{
	if (m_pItemImage)
	{
		m_pItemImage->SetVisibleSingle(FALSE);
		m_pItemImage->ResetItem();

		if (m_pMiniText) m_pMiniText->ClearText();
	}
}

void CAutoPotionSlot::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	const int nIndex = GetSlotIndex();
	SACTION_SLOT sSlot = GLGaeaClient::GetInstance().GetCharacter()->m_sACTIONQUICK[nIndex];
	if (sSlot.sNID != NATIVEID_NULL())
	{
		SITEM* pItem = GLItemMan::GetInstance().GetItem(sSlot.sNID);
		if (!pItem)
		{
			GASSERT(0 && "등록되지 않은 ID입니다.");
			return;
		}

		if (GetItemID() != sSlot.sNID) ResetItem();

		SetItemNativeID(sSlot.sNID);

		int nAmountNumber = GLGaeaClient::GetInstance().GetCharacter()->GetAmountActionQ(nIndex);
		SetItem(pItem->sBasicOp.sICONID, pItem->GetInventoryFile(), nAmountNumber);
	}
	else
	{
		ResetItem();
	}
}