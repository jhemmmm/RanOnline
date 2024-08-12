/////////////////////////////////////////////////////////////////////////////
//	FileName	: AutoFollowPage.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "AutoFollowPage.h"
#include "AutoFollowPageUser.h"

#include "../../BasicLineBox.h"
#include "../../BasicTextButton.h"
#include "../../BasicTextBoxEx.h"
#include "../../GameTextControl.h"
#include "../../UITextControl.h"
#include "../../InnerInterface.h"
#include "../../ModalWindow.h"

#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "../Lib -- ClientUI/Interface/BasicProgressBar.h"
#include "../Lib -- ClientUI/Interface/UIEditBoxMan.h"
#include "../Lib -- Engine/GUInterface/UIKeyCheck.h"
#include "../Lib -- Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const	int CAutoFollowPage::nCONTROLRANGE	= MAX_VIEWRANGE;
		int CAutoFollowPage::nLIMIT_0		= 3;

CAutoFollowPage::CAutoFollowPage()
	: m_nCurPage(-1)
	, m_nRange(-1)
	, m_pRangeBar(NULL)
	, m_pRangeBarDummy(NULL)
	, m_pButtonSelect(NULL)
{
	memset(m_pLBox_,				NULL, sizeof(m_pLBox_));
	memset(m_pButton_,				NULL, sizeof(m_pButton_));
	memset(m_pBG_,					NULL, sizeof(m_pBG_));
	memset(m_pTextMid_,				NULL, sizeof(m_pTextMid_));
	memset(m_pTextLeft_,			NULL, sizeof(m_pTextLeft_));
	memset(m_pAutoFollowPageUser_,	NULL, sizeof(m_pAutoFollowPageUser_));

	m_strName.Empty();
}

void CAutoFollowPage::CreateSubControl()
{
	char strTemp[50];

	for (int i = 0; i < nMAXLBOX - 2; i++)
	{
		sprintf(strTemp, "AUTOFOLLOW_PAGE_LBOX_%d", i);

		m_pLBox_[i] = new CBasicLineBox;
		m_pLBox_[i]->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pLBox_[i]->CreateBaseBoxOuter(strTemp);
		RegisterControl(m_pLBox_[i]);
	}

	for (int i = 0; i < nMAXBUTTON; i++)
	{
		sprintf(strTemp, "AUTOFOLLOW_PAGE_BUTTON_%d", i);

		m_pButton_[i] = new CBasicTextButton;
		m_pButton_[i]->CreateSub(this, "BASIC_TEXT_BUTTON23", UI_FLAG_XSIZE, BUTTON_0 + i);
		m_pButton_[i]->CreateBaseButton(strTemp, CBasicTextButton::SIZE23, CBasicButton::RADIO_FLIP, (char*)ID2GAMEWORD("AUTOFOLLOW_PAGE_BUTTON", i), _DEFAULT_FONT_SHADOW_FLAG);
		RegisterControl(m_pButton_[i]);
	}

	for (int i = 0; i < nMAXBG; i++)
	{
		sprintf(strTemp, "AUTOFOLLOW_PAGE_BG_%d", i);

		m_pBG_[i] = new CUIControl;
		m_pBG_[i]->CreateSub(this, strTemp, UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		RegisterControl(m_pBG_[i]);
	}

	for (int i = 2; i < nMAXLBOX; i++)
	{
		sprintf(strTemp, "AUTOFOLLOW_PAGE_LBOX_%d", i);

		m_pLBox_[i] = new CBasicLineBox;
		m_pLBox_[i]->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pLBox_[i]->CreateBaseBoxOuter(strTemp);
		RegisterControl(m_pLBox_[i]);
	}

	for (int i = 0; i < nMAXPAGE; i++)
	{
		m_pAutoFollowPageUser_[i] = new CAutoFollowPageUser;
		m_pAutoFollowPageUser_[i]->CreateSub(this, "AUTOFOLLOW_PAGE_USER", UI_FLAG_DEFAULT, PAGE_0 + i);
		m_pAutoFollowPageUser_[i]->CreateSubControl(i);
		m_pAutoFollowPageUser_[i]->SetVisibleSingle(FALSE);
		RegisterControl(m_pAutoFollowPageUser_[i]);
	}

	CD3DFontPar* pFont9		= DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG);
	const int nAlignCenter	= TEXT_ALIGN_CENTER_X	| TEXT_ALIGN_CENTER_Y;
	const int nAlignLeft	= TEXT_ALIGN_LEFT		| TEXT_ALIGN_CENTER_Y;
	const DWORD& dwWhite	= NS_UITEXTCOLOR::WHITESMOKE;

	for (int i = 0; i < nMAXTEXT_MID; i++)
	{
		sprintf(strTemp, "AUTOFOLLOW_PAGE_TEXTMID_%d", i);

		m_pTextMid_[i] = CreateStaticControl(strTemp, pFont9, nAlignCenter);
		m_pTextMid_[i]->SetOneLineText((char*)ID2GAMEWORD("AUTOFOLLOW_PAGE_TEXTMID", i), dwWhite);
	}

	m_pTextMid_[1]->ClearText();
	m_pTextMid_[3]->ClearText();

	for (int i = 0; i < nMAXTEXT_LEFT; i++)
	{
		sprintf(strTemp, "AUTOFOLLOW_PAGE_TEXTLEFT_%d", i);

		m_pTextLeft_[i] = CreateStaticControl(strTemp, pFont9, nAlignLeft);
		m_pTextLeft_[i]->SetOneLineText((char*)ID2GAMEWORD("AUTOFOLLOW_PAGE_TEXTLEFT", i), dwWhite);
	}

	m_pRangeBar = new CBasicProgressBar;
	m_pRangeBar->CreateSub(this, "AUTOFOLLOW_PAGE_RANGEBAR");
	m_pRangeBar->CreateOverImage("AUTOFOLLOW_PAGE_RANGEBAR_OVERIMAGE");
	RegisterControl(m_pRangeBar);

	m_pRangeBarDummy = new CUIControl;
	m_pRangeBarDummy->CreateSub(this, "AUTOFOLLOW_PAGE_RANGEBAR_DUMMY", UI_FLAG_DEFAULT, AUTOFOLLOW_PAGE_RANGEBAR);
	RegisterControl(m_pRangeBarDummy);

	m_pButtonSelect = new CBasicTextButton;
	m_pButtonSelect->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE,	BUTTON_SELECT);
	m_pButtonSelect->CreateBaseButton("AUTOFOLLOW_PAGE_BUTTON_SELECT", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, "Select", _DEFAULT_FONT_SHADOW_FLAG);
	RegisterControl(m_pButtonSelect);

	SetPage(0);
}

void CAutoFollowPage::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case BUTTON_0:
	case BUTTON_1:
	{
		int nIndex = ControlID - BUTTON_0;

		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetPage(nIndex);
		}
	}
	break;

	case AUTOFOLLOW_PAGE_RANGEBAR:
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

		const DWORD& dwWhite = NS_UITEXTCOLOR::WHITESMOKE;
		m_nRange = int(nCONTROLRANGE * m_pRangeBar->GetPercent());

		CString strRange;
		strRange.Format("%d", m_nRange);

		m_pTextMid_[3]->SetOneLineText(strRange, dwWhite);
	}
	break;

	case BUTTON_SELECT:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetSelectUser();
		}
	}
	break;
	}

	if (UIKeyCheck::GetInstance()->Check(DIK_TAB, DXKEY_DOWN))
	{
		switch (m_nCurPage)
		{
		case 0: SetPage(1); break;
		case 1: SetPage(0); break;
		}
	}

	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);
}

void CAutoFollowPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	if (bFirstControl)
	{
		if (m_pRangeBar && m_pRangeBar->IsExclusiveSelfControl())
		{
			const float fSize = m_pRangeBar->GetGlobalPos().sizeX;
			const float fPos = float(x) - m_pRangeBar->GetGlobalPos().left;
			const float fPercent = fPos / fSize;
			m_pRangeBar->SetPercent(fPercent);
		}
	}
}

void CAutoFollowPage::SetPage(int nPage)
{
	for (int i = 0; i < nMAXPAGE; i++)
	{
		m_pButton_[i]->SetFlip(FALSE);
		m_pAutoFollowPageUser_[i]->SetVisibleSingle(FALSE);
	}

	m_pButton_[nPage]->SetFlip(TRUE);
	m_pAutoFollowPageUser_[nPage]->SetVisibleSingle(TRUE);
	m_nCurPage = nPage;
}

void CAutoFollowPage::SetSelectUser()
{
	const DWORD& dwWhite = NS_UITEXTCOLOR::WHITESMOKE;

	m_pTextMid_[1]->ClearText();
	m_pTextMid_[1]->SetOneLineText(m_pAutoFollowPageUser_[m_nCurPage]->m_strSelectName, dwWhite);

	m_strName = m_pAutoFollowPageUser_[m_nCurPage]->m_strSelectName;
}

CBasicTextBox* CAutoFollowPage::CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID)
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub(this, szControlKeyword, UI_FLAG_DEFAULT, cID);
	pStaticText->SetFont(pFont);
	pStaticText->SetTextAlign(nAlign);
	RegisterControl(pStaticText);

	return pStaticText;
}

void CAutoFollowPage::RefreshFriendList()
{
	m_pAutoFollowPageUser_[0]->LoadFriendList();
}

CString CAutoFollowPage::GetFollowName()
{
	return m_strName;
}

int CAutoFollowPage::GetFollowRange()
{
	if (m_nRange < 1 || m_nRange > 300)
	{
		return -1;
	}

	return m_nRange;
}