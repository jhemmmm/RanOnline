/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoPotionPage.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "AutoPotionPage.h"
#include "AutoPotionTray.h"
#include "AutoPotionSlotEx.h"
#include "../AutoSystemWindow.h"

#include "../../BasicLineBox.h"
#include "../../BasicTextButton.h"
#include "../../BasicTextBoxEx.h"
#include "../../GameTextControl.h"
#include "../../UITextControl.h"
#include "../../InnerInterface.h"

#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "../Lib -- ClientUI/Interface/BasicProgressBar.h"
#include "../Lib -- Engine/GUInterface/UIKeyCheck.h"
#include "../Lib -- Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CAutoPotionPage::nCONTROLPOTION = 100;

CAutoPotionPage::CAutoPotionPage()
	: m_nHPSet(100)
	, m_nMPSet(100)
	, m_nSPSet(100)
{
	memset(m_pLBox_,			NULL, sizeof(m_pLBox_));
	memset(m_pButton_,			NULL, sizeof(m_pButton_));
	memset(m_pBG_,				NULL, sizeof(m_pBG_));
	memset(m_pAutoPotionTray_,	NULL, sizeof(m_pAutoPotionTray_));
	memset(m_pAutoPotionBar_,	NULL, sizeof(m_pAutoPotionBar_));
	memset(m_pTextMid_,			NULL, sizeof(m_pTextMid_));
	memset(m_pTextLeft_,		NULL, sizeof(m_pTextLeft_));
	memset(m_pControl_,			NULL, sizeof(m_pControl_));
}

void CAutoPotionPage::CreateSubControl()
{
	char strTemp[50];

	for (int i = 0; i < nMAXLBOX; i++)
	{
		sprintf(strTemp, "AUTOPOTION_PAGE_LBOX_%d", i);

		m_pLBox_[i] = new CBasicLineBox;
		m_pLBox_[i]->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pLBox_[i]->CreateBaseBoxOuter(strTemp);
		RegisterControl(m_pLBox_[i]);
	}

	m_pLBox_[3]->SetVisibleSingle(FALSE);
	m_pLBox_[4]->SetVisibleSingle(FALSE);
	m_pLBox_[5]->SetVisibleSingle(FALSE);

	for (int i = 0; i < nMAXBUTTON; i++)
	{
		sprintf(strTemp, "AUTOPOTION_PAGE_BUTTON_%d", i);

		m_pButton_[i] = new CBasicTextButton;
		m_pButton_[i]->CreateSub(this, "BASIC_TEXT_BUTTON23", UI_FLAG_XSIZE, BUTTON_0 + i);
		m_pButton_[i]->CreateBaseButton(strTemp, CBasicTextButton::SIZE23, CBasicButton::RADIO_FLIP, (char*)ID2GAMEWORD("AUTOPOTION_PAGE_BUTTON", i), _DEFAULT_FONT_SHADOW_FLAG);
		RegisterControl(m_pButton_[i]);
	}

	for (int i = 0; i < nMAXBG; i++)
	{
		sprintf(strTemp, "AUTOPOTION_PAGE_BG_%d", i);

		m_pBG_[i] = new CUIControl;
		m_pBG_[i]->CreateSub(this, strTemp, UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		RegisterControl(m_pBG_[i]);
	}

	for (int i = 0; i < nMAXPAGE; i++)
	{
		m_pAutoPotionTray_[i] = new CAutoPotionTray;
		m_pAutoPotionTray_[i]->CreateSub(this, "AUTOPOTION_TRAY", UI_FLAG_DEFAULT, PAGE_0 + i);
		m_pAutoPotionTray_[i]->CreateSubControl(i);
		m_pAutoPotionTray_[i]->SetVisibleSingle(FALSE);
		RegisterControl(m_pAutoPotionTray_[i]);
	}

	for (int i = 0; i < nMAXBAR; i++)
	{
		sprintf(strTemp, "AUTOPOTION_PAGE_BAR_%d", i);

		m_pAutoPotionBar_[i] = new CBasicProgressBar;
		m_pAutoPotionBar_[i]->CreateSub(this, strTemp);
		m_pAutoPotionBar_[i]->CreateOverImage("AUTOPOTION_PAGE_BAR_OVERIMAGE");
		RegisterControl(m_pAutoPotionBar_[i]);
	}

	CD3DFontPar* pFont9		= DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG);
	const int nAlignCenter	= TEXT_ALIGN_CENTER_X	| TEXT_ALIGN_CENTER_Y;
	const int nAlignLeft	= TEXT_ALIGN_LEFT		| TEXT_ALIGN_CENTER_Y;
	const DWORD& dwWhite	= NS_UITEXTCOLOR::WHITESMOKE;

	for (int i = 0; i < nMAXTEXT_MID; i++)
	{
		sprintf(strTemp, "AUTOPOTION_PAGE_TEXTMID_%d", i);

		m_pTextMid_[i] = CreateStaticControl(strTemp, pFont9, nAlignCenter);
		m_pTextMid_[i]->SetOneLineText((char*)ID2GAMEWORD("AUTOPOTION_PAGE_TEXTMID", i), dwWhite);
	}

	for (int i = 0; i < nMAXTEXT_LEFT; i++)
	{
		sprintf(strTemp, "AUTOPOTION_PAGE_TEXTLEFT_%d", i);

		m_pTextLeft_[i] = CreateStaticControl(strTemp, pFont9, nAlignLeft);
		m_pTextLeft_[i]->SetOneLineText((char*)ID2GAMEWORD("AUTOPOTION_PAGE_TEXTLEFT", i), dwWhite);
	}

	for (int i = 0; i < nMAXCONTROL; i++)
	{
		sprintf(strTemp, "AUTOPOTION_PAGE_CONTROL_%d", i);

		m_pControl_[i] = new CUIControl;
		m_pControl_[i]->CreateSub(this, strTemp, UI_FLAG_DEFAULT, CONTROL_0 + i);
		RegisterControl(m_pControl_[i]);
	}

	for (int i = 0; i < nMAXBAR; i++)
		m_pAutoPotionBar_[i]->SetPercent(1.0f);

	SetTab(0);
}

void CAutoPotionPage::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case BUTTON_0:
	case BUTTON_1:
	{
		int nIndex = ControlID - BUTTON_0;

		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetTab(nIndex);
		}
	}
	break;

	case CONTROL_0:
	case CONTROL_1:
	case CONTROL_2:
	{
		int nIndex = ControlID - CONTROL_0;

		if (CHECK_MOUSE_IN(dwMsg))
		{
			if (CHECK_LB_DOWN_LIKE(dwMsg))
			{
				m_pAutoPotionBar_[nIndex]->SetExclusiveControl();
			}
			else if (CHECK_LB_UP_LIKE(dwMsg))
			{
				m_pAutoPotionBar_[nIndex]->ResetExclusiveControl();
			}
		}
		else if (CHECK_LB_UP_LIKE(dwMsg))
		{
			m_pAutoPotionBar_[nIndex]->ResetExclusiveControl();
		}

		const	DWORD&	dwWhite		= NS_UITEXTCOLOR::WHITESMOKE;
				int		nPotion		= int(nCONTROLPOTION * m_pAutoPotionBar_[nIndex]->GetPercent());

		CString strPercent;
		strPercent.Format("%d", nPotion);

		m_pTextLeft_[nIndex + 6]->SetOneLineText(strPercent, dwWhite);

		if (nIndex == 0)		m_nHPSet = nPotion;
		else if(nIndex == 1)	m_nMPSet = nPotion;
		else if (nIndex == 2)	m_nSPSet = nPotion;
	}
	break;

	case PAGE_0:
	case PAGE_1:
	{
		int nTabIndex = ControlID - PAGE_0;

		BOOL bSnap = (dwMsg & UIMSG_MOUSEIN_AUTOPOTIONSLOT);
		if (bSnap) CInnerInterface::GetInstance().SetSnapItem();

		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			if (m_pAutoPotionTray_[nTabIndex])
			{
				int nSlotIndex = m_pAutoPotionTray_[nTabIndex]->GetSlotIndex();

				m_pAutoPotionTray_[nTabIndex]->SetSlotIndex(nSlotIndex);
			}
		}
	}
	break;
	}

	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);
}

void CAutoPotionPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	if (bFirstControl)
	{
		for (int i = 0; i < nMAXBAR; i++)
		{
			if (m_pAutoPotionBar_[i] && m_pAutoPotionBar_[i]->IsExclusiveSelfControl())
			{
				const float fSize		= m_pAutoPotionBar_[i]->GetGlobalPos().sizeX;
				const float fPos		= float(x) - m_pAutoPotionBar_[i]->GetGlobalPos().left;
				const float fPercent	= fPos / fSize;
				m_pAutoPotionBar_[i]->SetPercent(fPercent);
			}
		}
	}

	if (UIKeyCheck::GetInstance()->Check(DIK_TAB, DXKEY_DOWN))
	{
		switch (m_nCurPage)
		{
		case 0: SetTab(1); break;
		case 1: SetTab(0); break;
		}
	}
}

CBasicTextBox* CAutoPotionPage::CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID)
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub(this, szControlKeyword, UI_FLAG_DEFAULT, cID);
	pStaticText->SetFont(pFont);
	pStaticText->SetTextAlign(nAlign);
	RegisterControl(pStaticText);

	return pStaticText;
}

void CAutoPotionPage::SetLBox(int nIndex)
{
	m_pLBox_[3]->SetVisibleSingle(FALSE);
	m_pLBox_[4]->SetVisibleSingle(FALSE);
	m_pLBox_[5]->SetVisibleSingle(FALSE);

	m_pLBox_[nIndex + 3]->SetVisibleSingle(TRUE);
}

void CAutoPotionPage::SetTab(int nIndex)
{
	m_nCurPage = nIndex;

	for (int i = 0; i < nMAXPAGE; i++)
	{
		m_pButton_[i]->SetFlip(FALSE);
		m_pAutoPotionTray_[i]->SetVisibleSingle(FALSE);
	}

	m_pButton_[nIndex]->SetFlip(TRUE);
	m_pAutoPotionTray_[nIndex]->SetVisibleSingle(TRUE);

	CInnerInterface::GetInstance().GetAutoSystemWindow()->ReRunAutoPotion();
}

BOOL CAutoPotionPage::CheckPotionTab(int nTab)
{
	switch (nTab)
	{
	case 0:
	{
		for (int i = 0; i < nMAXPOTION_SLOT; i++)
		{
			SACTION_SLOT sPotionSlot = GLGaeaClient::GetInstance().GetCharacter()->GetCharData().m_sACTIONQUICK[i + 6];
			if (sPotionSlot.VALID())
			{
				return TRUE;
				break;
			}
		}
	}
	break;
	
	case 1:
	{
		for (int i = 0; i < nMAXPOTION_SLOT; i++)
		{
			SACTION_SLOT sPotionSlot = GLGaeaClient::GetInstance().GetCharacter()->GetCharData().m_sACTIONQUICK[i + 9];
			if (sPotionSlot.VALID())
			{
				return TRUE;
				break;
			}
		}
	}
	break;
	}

	return FALSE;
}

int CAutoPotionPage::GetPotionSettings(int nIndex)
{
	switch (nIndex)
	{
	case 0:
	{
		if (m_nHPSet < 1 || m_nHPSet > 100)
		{
			return -1;
		}
		
		return m_nHPSet;
	}
	break;

	case 1:
	{
		if (m_nMPSet < 1 || m_nMPSet > 100)
		{
			return -1;
		}

		return m_nMPSet;
	}
	break;

	case 2:
	{
		if (m_nSPSet < 1 || m_nSPSet > 100)
		{
			return -1;
		}

		return m_nSPSet;
	}
	break;
	}

	return -1;
}