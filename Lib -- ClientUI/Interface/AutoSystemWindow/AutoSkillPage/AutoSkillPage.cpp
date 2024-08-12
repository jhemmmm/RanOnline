/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoSkillPage.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "AutoSkillPage.h"
#include "AutoSkillTray.h"
#include "AutoSkillPageToTray.h"
#include "../AutoSystemWindow.h"

#include "../../BasicLineBox.h"
#include "../../BasicTextButton.h"
#include "../../BasicTextBoxEx.h"
#include "../../GameTextControl.h"
#include "../../UITextControl.h"
#include "../../InnerInterface.h"

#include "../Lib -- Engine/Common/DxInputString.h"
#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "../Lib -- ClientUI/Interface/BasicProgressBar.h"
#include "../Lib -- ClientUI/Interface/UIEditBoxMan.h"
#include "../Lib -- Engine/GUInterface/UIKeyCheck.h"
#include "../Lib -- Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const	int CAutoSkillPage::nCONTROLRANGE	= MAX_VIEWRANGE;
		int CAutoSkillPage::nLIMIT_0		= 3;

CAutoSkillPage::CAutoSkillPage()
	: m_nCurPage1(-1)
	, m_nCurPage2(-1)
	, m_pEditBoxMan(NULL)

	, m_pRangeBar(NULL)
	, m_pRangeBarDummy(NULL)

	, m_nRange(-1)
{
	memset(m_pLBox_,			NULL, sizeof(m_pLBox_));
	memset(m_pButton_,			NULL, sizeof(m_pButton_));
	memset(m_pBG_,				NULL, sizeof(m_pBG_));
	memset(m_pTextMid_,			NULL, sizeof(m_pTextMid_));
	memset(m_pTextLeft_,		NULL, sizeof(m_pTextLeft_));

	memset(m_pAutoSkillTray_,	NULL, sizeof(m_pAutoSkillTray_));
}

void CAutoSkillPage::CreateSubControl()
{
	char strTemp[50];

	for (int i = 0; i < nMAXLBOX - 3; i++)
	{
		sprintf(strTemp, "AUTOSKILL_PAGE_LBOX_%d", i);

		m_pLBox_[i] = new CBasicLineBox;
		m_pLBox_[i]->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pLBox_[i]->CreateBaseBoxOuter(strTemp);
		RegisterControl(m_pLBox_[i]);
	}

	for (int i = 0; i < nMAXBUTTON; i++)
	{
		sprintf(strTemp, "AUTOSKILL_PAGE_BUTTON_%d", i);

		m_pButton_[i] = new CBasicTextButton;
		m_pButton_[i]->CreateSub(this, "BASIC_TEXT_BUTTON23", UI_FLAG_XSIZE, BUTTON_0 + i);
		m_pButton_[i]->CreateBaseButton(strTemp, CBasicTextButton::SIZE23, CBasicButton::RADIO_FLIP, (char*)ID2GAMEWORD("AUTOSKILL_PAGE_BUTTON", i), _DEFAULT_FONT_SHADOW_FLAG);
		RegisterControl(m_pButton_[i]);
	}

	for (int i = 0; i < nMAXBG; i++)
	{
		sprintf(strTemp, "AUTOSKILL_PAGE_BG_%d", i);

		m_pBG_[i] = new CUIControl;
		m_pBG_[i]->CreateSub(this, strTemp, UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		RegisterControl(m_pBG_[i]);
	}

	for (int i = 2; i < nMAXLBOX; i++)
	{
		sprintf(strTemp, "AUTOSKILL_PAGE_LBOX_%d", i);

		m_pLBox_[i] = new CBasicLineBox;
		m_pLBox_[i]->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pLBox_[i]->CreateBaseBoxOuter(strTemp);
		RegisterControl(m_pLBox_[i]);
	}

	CD3DFontPar* pFont9		= DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG);
	const int nAlignCenter	= TEXT_ALIGN_CENTER_X	| TEXT_ALIGN_CENTER_Y;
	const int nAlignLeft	= TEXT_ALIGN_LEFT		| TEXT_ALIGN_CENTER_Y;
	const DWORD& dwWhite	= NS_UITEXTCOLOR::WHITESMOKE;

	for (int i = 0; i < nMAXTEXT_MID; i++)
	{
		sprintf(strTemp, "AUTOSKILL_PAGE_TEXTMID_%d", i);

		m_pTextMid_[i] = CreateStaticControl(strTemp, pFont9, nAlignCenter);
		m_pTextMid_[i]->SetOneLineText((char*)ID2GAMEWORD("AUTOSKILL_PAGE_TEXTMID", i), dwWhite);
	}

	for (int i = 0; i < nMAXTEXT_LEFT; i++)
	{
		sprintf(strTemp, "AUTOSKILL_PAGE_TEXTLEFT_%d", i);

		m_pTextLeft_[i] = CreateStaticControl(strTemp, pFont9, nAlignLeft);
		m_pTextLeft_[i]->SetOneLineText((char*)ID2GAMEWORD("AUTOSKILL_PAGE_TEXTLEFT", i), dwWhite);
	}

	m_pRangeBar = new CBasicProgressBar;
	m_pRangeBar->CreateSub(this, "AUTOSKILL_PAGE_RANGEBAR");
	m_pRangeBar->CreateOverImage("AUTOSKILL_PAGE_RANGEBAR_OVERIMAGE");
	RegisterControl(m_pRangeBar);

	m_pRangeBarDummy = new CUIControl;
	m_pRangeBarDummy->CreateSub(this, "AUTOSKILL_PAGE_RANGEBAR_DUMMY", UI_FLAG_DEFAULT, AUTOSKILL_PAGE_RANGEBAR);
	RegisterControl(m_pRangeBarDummy);

	m_pEditBoxMan = new CUIEditBoxMan;
	m_pEditBoxMan->CreateSub(this, "AUTOSKILL_PAGE_EDITMAN", UI_FLAG_DEFAULT, AUTOSKILL_PAGE_EDITMAN);
	m_pEditBoxMan->CreateEditBox(AUTOSKILL_PAGE_EDIT_0, "AUTOSKILL_PAGE_EDIT_0", "AUTOSKILL_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_0);
	m_pEditBoxMan->CreateEditBox(AUTOSKILL_PAGE_EDIT_1, "AUTOSKILL_PAGE_EDIT_1", "AUTOSKILL_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_0);
	RegisterControl(m_pEditBoxMan);

	for (int i = 0; i < nMAXPAGE; i++)
	{
		m_pAutoSkillTray_[i] = new CAutoSkillTray;
		m_pAutoSkillTray_[i]->CreateSub(this, "AUTOSKILL_TRAY", UI_FLAG_DEFAULT, PAGE_0 + i);
		m_pAutoSkillTray_[i]->CreateSubControl(i);
		m_pAutoSkillTray_[i]->SetVisibleSingle(FALSE);
		RegisterControl(m_pAutoSkillTray_[i]);
	}

	SetTab1(0);
	SetTab2(0);
}

void CAutoSkillPage::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case BUTTON_0:
	case BUTTON_1:
	case BUTTON_2:
	{
		int nIndex = ControlID - BUTTON_0;

		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetTab1(nIndex);
		}
	}
	break;

	case BUTTON_3:
	case BUTTON_4:
	{
		int nIndex = ControlID - BUTTON_3;

		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetTab2(nIndex);
		}
	}
	break;

	case AUTOSKILL_PAGE_RANGEBAR:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			if (CHECK_LB_DOWN_LIKE(dwMsg))
			{
				m_pRangeBar->SetExclusiveControl();
			}
			else if (CHECK_LB_UP_LIKE(dwMsg))
			{
				m_pRangeBar->ResetExclusiveControl();
			}
		}
		else if (CHECK_LB_UP_LIKE(dwMsg))
		{
			m_pRangeBar->ResetExclusiveControl();
		}

		const DWORD& dwWhite	= NS_UITEXTCOLOR::WHITESMOKE;
		m_nRange				= int(nCONTROLRANGE * m_pRangeBar->GetPercent());

		CString strRange;
		strRange.Format("%ld", m_nRange);

		m_pTextMid_[2]->SetOneLineText(strRange, dwWhite);
	}
	break;
	}

	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);

	if (ControlID == AUTOSKILL_PAGE_EDITMAN)
	{
		if (!CHECK_MOUSE_IN(dwMsg))
		{
			if (UIMSG_LB_DOWN & dwMsg)
			{
				m_pEditBoxMan->EndEdit();
			}
		}
	}
}

void CAutoSkillPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	if (bFirstControl)
	{
		if (m_pRangeBar && m_pRangeBar->IsExclusiveSelfControl())
		{
			const float fSize		= m_pRangeBar->GetGlobalPos().sizeX;
			const float fPos		= float(x) - m_pRangeBar->GetGlobalPos().left;
			const float fPercent	= fPos / fSize;
			m_pRangeBar->SetPercent(fPercent);
		}
	}

	if (m_pEditBoxMan)
	{
		CUIControl* pParent = m_pEditBoxMan->GetTopParent();
		if (!pParent)	pParent = this;
		BOOL bFocus = (pParent->IsFocusControl());

		if (bFocus)
		{
			if (DXInputString::GetInstance().IsOn())
			{
				if (UIKeyCheck::GetInstance()->Check(DIK_TAB, DXKEY_DOWN))
				{
					m_pEditBoxMan->GoNextTab();
				}
			}
			else
			{
				if (UIKeyCheck::GetInstance()->Check(DIK_TAB, DXKEY_DOWN))
				{
					switch (m_nCurPage1)
					{
					case 0: SetTab1(1); break;
					case 1: SetTab1(2); break;
					case 2: SetTab1(0); break;
					}
				}
			}
		}
	}
}

CBasicTextBox* CAutoSkillPage::CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID)
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub(this, szControlKeyword, UI_FLAG_DEFAULT, cID);
	pStaticText->SetFont(pFont);
	pStaticText->SetTextAlign(nAlign);
	RegisterControl(pStaticText);

	return pStaticText;
}

void CAutoSkillPage::SetTab1(int nIndex)
{
	m_nCurPage1 = nIndex;

	for (int i = 0; i < nMAXPAGE; i++)
	{
		m_pButton_[i]->SetFlip(FALSE);
		m_pAutoSkillTray_[i]->SetVisibleSingle(FALSE);
	}

	m_pButton_[nIndex]->SetFlip(TRUE);
	m_pAutoSkillTray_[nIndex]->SetVisibleSingle(TRUE);

	CInnerInterface::GetInstance().GetAutoSystemWindow()->ReRunAutoSkill();
}

void CAutoSkillPage::SetTab2(int nIndex)
{
	m_nCurPage2 = nIndex;

	m_pButton_[3]->SetFlip(FALSE);
	m_pButton_[4]->SetFlip(FALSE);

	for (int i = 0; i < nMAXTEXT_MID; i++)
	{
		m_pTextMid_[i]->SetVisibleSingle(FALSE);
	}

	for (int i = 0; i < nMAXTEXT_LEFT; i++)
	{
		m_pTextLeft_[i]->SetVisibleSingle(FALSE);
	}

	m_pLBox_[5]->SetVisibleSingle(FALSE);

	m_pEditBoxMan->SetVisibleSingle(FALSE);

	m_pRangeBar->SetVisibleSingle(FALSE);
	m_pRangeBarDummy->SetVisibleSingle(FALSE);

	switch (nIndex)
	{
	case 0:
	{
		m_pButton_[3]->SetFlip(TRUE);

		m_pTextMid_[0]->SetVisibleSingle(TRUE);

		m_pTextLeft_[0]->SetVisibleSingle(TRUE);
		m_pTextLeft_[1]->SetVisibleSingle(TRUE);
		m_pTextLeft_[2]->SetVisibleSingle(TRUE);
		m_pTextLeft_[3]->SetVisibleSingle(TRUE);

		m_pLBox_[5]->SetVisibleSingle(TRUE);

		m_pEditBoxMan->SetVisibleSingle(TRUE);
	}
	break;

	case 1:
	{
		m_pButton_[4]->SetFlip(TRUE);

		m_pTextMid_[1]->SetVisibleSingle(TRUE);
		m_pTextMid_[2]->SetVisibleSingle(TRUE);

		m_pTextLeft_[3]->SetVisibleSingle(TRUE);
		m_pTextLeft_[4]->SetVisibleSingle(TRUE);

		m_pRangeBar->SetVisibleSingle(TRUE);
		m_pRangeBarDummy->SetVisibleSingle(TRUE);
	}
	break;
	}
}

int CAutoSkillPage::GetCurSkillTab()
{
	int nReturn = -1;

	switch (m_nCurPage1)
	{
	case 0: nReturn = 30; return nReturn;
	case 1: nReturn = 40; return nReturn;
	case 2: nReturn = 50; return nReturn;
	}

	return nReturn;
}

int CAutoSkillPage::GetLevMin()
{
	m_pEditBoxMan->EndEdit();

	CString strLevMin = m_pEditBoxMan->GetEditString(AUTOSKILL_PAGE_EDIT_0);
	int nLevMin = atoi(strLevMin);

	if (nLevMin < 1 || nLevMin > 999) return -1;

	return nLevMin;
}

int CAutoSkillPage::GetLevMax()
{
	m_pEditBoxMan->EndEdit();

	CString strLevMax = m_pEditBoxMan->GetEditString(AUTOSKILL_PAGE_EDIT_1);
	int nLevMax = atoi(strLevMax);

	if (nLevMax < 1 || nLevMax > 999) return -1;

	return nLevMax;
}

BOOL CAutoSkillPage::CheckSkillTab(int nTab)
{
	switch (nTab)
	{
	case 30:
	{
		for (int i = 0; i < nMAXSKILL_SLOT; i++)
		{
			SNATIVEID sNativeID = GLGaeaClient::GetInstance().GetCharacter()->GetCharData().m_sSKILLQUICK[i + 30];
			if (sNativeID != NATIVEID_NULL())
			{
				return TRUE;
				break;
			}
		}
	}
	break;

	case 40:
	{
		for (int i = 0; i < nMAXSKILL_SLOT; i++)
		{
			SNATIVEID sNativeID = GLGaeaClient::GetInstance().GetCharacter()->GetCharData().m_sSKILLQUICK[i + 40];
			if (sNativeID != NATIVEID_NULL())
			{
				return TRUE;
				break;
			}
		}
	}
	break;

	case 50:
	{
		for (int i = 0; i < nMAXSKILL_SLOT; i++)
		{
			SNATIVEID sNativeID = GLGaeaClient::GetInstance().GetCharacter()->GetCharData().m_sSKILLQUICK[i + 50];
			if (sNativeID != NATIVEID_NULL())
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