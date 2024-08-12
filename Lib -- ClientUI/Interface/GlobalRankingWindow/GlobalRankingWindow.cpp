/////////////////////////////////////////////////////////////////////////////
//	FileName	: GlobalRankingWindow.cpp
//	Project		: RanClientUILib
//	Coder		: LG-7

#include "stdafx.h"
#include "GlobalRankingWindow.h"
#include "GlobalRankingPage.h"

#include "../BasicTextButton.h"
#include "../InnerInterface.h"
#include "../GameTextControl.h"

#include "BasicButton.h"
#include "UIWindowTitle.h"
#include "UIWindowBody.h"

#include "RANPARAM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGlobalRankingWindow::CGlobalRankingWindow()
	: m_bDelay(FALSE)
	
	, m_pWhiteBG(NULL)
{
	memset(m_pButtonTab_,	NULL, sizeof(m_pButtonTab_));
	memset(m_pPage_,		NULL, sizeof(m_pPage_));
}

CGlobalRankingWindow::~CGlobalRankingWindow()
{
}

void CGlobalRankingWindow::CreateSubControl()
{
	for (int i = 0; i < 10; i++)
	{
		m_pButtonTab_[i] = new CBasicTextButton;
		m_pButtonTab_[i]->CreateSub(this, "BASIC_TEXT_BUTTON23", UI_FLAG_XSIZE, BUTTON_TAB_0 + i);
		m_pButtonTab_[i]->CreateBaseButton("BASIC_TEXT_BUTTON23", CBasicTextButton::SIZE23, CBasicButton::RADIO_FLIP, NULL, _DEFAULT_FONT_SHADOW_FLAG);
		RegisterControl(m_pButtonTab_[i]);
	}

	m_pWhiteBG = new CUIControl;
	m_pWhiteBG->CreateSub(this, "BASIC_WINDOW_WHITE_BG", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	RegisterControl(m_pWhiteBG);

	for (int i = 0; i < 10; i++)
	{
		m_pPage_[i] = new CGlobalRankingPage;
		m_pPage_[i]->CreateSub(this, "BASIC_WINDOW", UI_FLAG_DEFAULT);
		m_pPage_[i]->CreateSubControl(i);
		RegisterControl(m_pPage_[i]);
	}

	RESET();

	DefaultPos();
	SetPage(BUTTON_TAB_0);
}

void CGlobalRankingWindow::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	static float fTimer(0.0f);

	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	if (m_bDelay)
	{
		fTimer += fElapsedTime;

		if (fTimer > 0.0f)
		{
			m_bDelay = FALSE;
			fTimer = 0.0f;
		}
	}
}

void CGlobalRankingWindow::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	if (BUTTON_TAB_0 <= ControlID && ControlID < BUTTON_TAB_END)
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
			SetPage(ControlID);

	switch (ControlID)
	{
	case ET_CONTROL_BUTTON:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
			CInnerInterface::GetInstance().SetGlobalRankingWindowOpen(FALSE);
	}
	break;
	}

	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);
}

void CGlobalRankingWindow::DefaultPos()
{
	UIRECT RcOLD;
	UIRECT RcNEW1;

	/*dmk14 LG-7 button data*/
	UIRECT m_RcButton[10];
	UIRECT m_RcWindow, m_RcBG;

	m_RcWindow = UIRECT(0.00, 0.00, 728.00, 429.00);
	m_RcBG = UIRECT(5.00, 41.00, 718.00, 381.00);

	if ( RANPARAM::dwGlobalRankingClass == 4 )
	{
		m_RcButton[0] = UIRECT(5.00, 22.00, 58.00, 19.00);
		m_RcButton[1] = UIRECT(65.00, 22.00, 58.00, 19.00);
		m_RcButton[2] = UIRECT(125.00, 22.00, 58.00, 19.00);
		m_RcButton[3] = UIRECT(185.00, 22.00, 58.00, 19.00);
		m_RcButton[4] = UIRECT(245.00, 22.00, 58.00, 19.00);
		m_RcButton[5] = UIRECT(305.00, 22.00, 58.00, 19.00);
		m_RcButton[6] = UIRECT(365.00, 22.00, 58.00, 19.00);		//shaman 4class button
	}
	else if ( RANPARAM::dwGlobalRankingClass == 5 )
	{
		m_RcButton[0] = UIRECT(5.00, 22.00, 58.00, 19.00);
		m_RcButton[1] = UIRECT(65.00, 22.00, 58.00, 19.00);
		m_RcButton[2] = UIRECT(125.00, 22.00, 58.00, 19.00);
		m_RcButton[3] = UIRECT(185.00, 22.00, 58.00, 19.00);
		m_RcButton[4] = UIRECT(245.00, 22.00, 58.00, 19.00);
		m_RcButton[5] = UIRECT(305.00, 22.00, 58.00, 19.00);
		m_RcButton[6] = UIRECT(365.00, 22.00, 58.00, 19.00);		//shaman 4class button
		m_RcButton[7] = UIRECT(425.00, 22.00, 58.00, 19.00);		//5 class extreme
	}
	else if ( RANPARAM::dwGlobalRankingClass == 6 )
	{
		m_RcButton[0] = UIRECT(5.00, 22.00, 58.00, 19.00);
		m_RcButton[1] = UIRECT(65.00, 22.00, 58.00, 19.00);
		m_RcButton[2] = UIRECT(125.00, 22.00, 58.00, 19.00);
		m_RcButton[3] = UIRECT(185.00, 22.00, 58.00, 19.00);
		m_RcButton[4] = UIRECT(245.00, 22.00, 58.00, 19.00);
		m_RcButton[5] = UIRECT(305.00, 22.00, 58.00, 19.00);
		m_RcButton[6] = UIRECT(365.00, 22.00, 58.00, 19.00);		//shaman 4class button
		m_RcButton[7] = UIRECT(425.00, 22.00, 58.00, 19.00);		//5 class extreme
		m_RcButton[8] = UIRECT(485.00, 22.00, 58.00, 19.00);		//6 class gunner
	}
	else if ( RANPARAM::dwGlobalRankingClass == 7 )
	{
		m_RcButton[0] = UIRECT(5.00, 22.00, 58.00, 19.00);
		m_RcButton[1] = UIRECT(65.00, 22.00, 58.00, 19.00);
		m_RcButton[2] = UIRECT(125.00, 22.00, 58.00, 19.00);
		m_RcButton[3] = UIRECT(185.00, 22.00, 58.00, 19.00);
		m_RcButton[4] = UIRECT(245.00, 22.00, 58.00, 19.00);
		m_RcButton[5] = UIRECT(305.00, 22.00, 58.00, 19.00);
		m_RcButton[6] = UIRECT(365.00, 22.00, 58.00, 19.00);		//shaman 4class button
		m_RcButton[7] = UIRECT(425.00, 22.00, 58.00, 19.00);		//5 class extreme
		m_RcButton[8] = UIRECT(485.00, 22.00, 58.00, 19.00);		//6 class gunner
		m_RcButton[9] = UIRECT(545.00, 22.00, 58.00, 19.00);		//7 class assassin
	}
	//m_RcButton[7] = UIRECT(425.00, 22.00, 58.00, 19.00);
	//m_RcButton[8] = UIRECT(485.00, 22.00, 58.00, 19.00);
	//m_RcButton[9] = UIRECT(545.00, 22.00, 58.00, 19.00);
	//m_RcButton[10] = UIRECT(605.00, 22.00, 58.00, 19.00);
	//m_RcButton[11] = UIRECT(665.00, 22.00, 58.00, 19.00);

	/*m_RcButton[0] = UIRECT(5.00, 22.00, 58.00, 19.00);
	m_RcButton[1] = UIRECT(65.00, 22.00, 58.00, 19.00);
	m_RcButton[2] = UIRECT(125.00, 22.00, 58.00, 19.00);
	m_RcButton[3] = UIRECT(185.00, 22.00, 58.00, 19.00);
	m_RcButton[4] = UIRECT(245.00, 22.00, 58.00, 19.00);
	m_RcButton[5] = UIRECT(305.00, 22.00, 58.00, 19.00);
	m_RcButton[6] = UIRECT(365.00, 22.00, 58.00, 19.00);		//shaman 4class button
	m_RcButton[7] = UIRECT(425.00, 22.00, 58.00, 19.00);		//5 class extreme
	m_RcButton[8] = UIRECT(485.00, 22.00, 58.00, 19.00);		//6 class gunner
	m_RcButton[9] = UIRECT(545.00, 22.00, 58.00, 19.00);		//7 class assassin*/

	for (int i = 0; i < 10; i++)
	{
		RcNEW1 = m_RcButton[i];

		RcOLD = m_pButtonTab_[i]->GetLocalPos();
		m_pButtonTab_[i]->AlignSubControl(RcOLD, RcNEW1);
		m_pButtonTab_[i]->SetLocalPos(RcNEW1);
		m_pButtonTab_[i]->SetGlobalPos(RcNEW1);
		m_pButtonTab_[i]->SetOneLineText((char*)ID2GAMEWORD("GLOBAL_RANKING_WINDOW_BUTTON", i));
	}

	// m_pWhiteBG
	{
		RcNEW1 = m_RcBG;

		RcOLD = m_pWhiteBG->GetLocalPos();
		m_pWhiteBG->AlignSubControl(RcOLD, RcNEW1);
		m_pWhiteBG->SetLocalPos(RcNEW1);
	}

	{
		D3DXVECTOR2 vPos;
		vPos.x = GetGlobalPos().left;
		vPos.y = GetGlobalPos().top;

		RcNEW1 = m_RcWindow;

		SetLocalPos(RcNEW1);
		SetGlobalPos(RcNEW1);

		{
			RcOLD = GetTitle()->GetLocalPos();
			RcNEW1 = UIRECT(
				GetLocalPos().left,
				GetLocalPos().top,
				GetLocalPos().sizeX,
				RcOLD.sizeY
				);
			GetTitle()->AlignSubControl(RcOLD, RcNEW1);
			GetTitle()->SetLocalPos(RcNEW1);
			GetTitle()->SetGlobalPos(RcNEW1);

#if defined(KRT_PARAM) || defined(KR_PARAM) || defined(TW_PARAM) || defined(JP_PARAM) || defined(CH_PARAM) || defined ( HK_PARAM )
			GetTitleFocus()->AlignSubControl(RcOLD, RcNEW1);
			GetTitleFocus()->SetLocalPos(RcNEW1);
			GetTitleFocus()->SetGlobalPos(RcNEW1);
#endif
		}

		{
			RcOLD = GetBody()->GetLocalPos();
			RcNEW1 = UIRECT(
				GetLocalPos().left,
				GetTitle()->GetLocalPos().top + GetTitle()->GetLocalPos().sizeY,
				GetLocalPos().sizeX,
				GetLocalPos().sizeY - GetTitle()->GetLocalPos().sizeY
				);
			GetBody()->AlignSubControl(RcOLD, RcNEW1);
			GetBody()->SetLocalPos(RcNEW1);
			GetBody()->SetGlobalPos(RcNEW1);
		}

		{
			RcOLD = GetClose()->GetLocalPos();
			RcNEW1 = UIRECT(
				GetTitle()->GetLocalPos().left + GetTitle()->GetLocalPos().sizeX - 20.0f,
				RcOLD.top,
				RcOLD.sizeX,
				RcOLD.sizeY
				);
			GetClose()->AlignSubControl(RcOLD, RcNEW1);
			GetClose()->SetLocalPos(RcNEW1);
			GetClose()->SetGlobalPos(RcNEW1);
		}

		SetGlobalPos(vPos);
	}

	this->SetTitleName((char*)ID2GAMEWORD("GLOBAL_RANKING_WINDOW"));
}

void CGlobalRankingWindow::SetPage(int nIndex)
{
	if (m_bDelay)
		return;

	m_bDelay = TRUE;

	int nPage = nIndex - BUTTON_TAB_0;

	for (int i = 0; i < 10; i++)
	{
		m_pButtonTab_[i]->SetFlip(FALSE);
		m_pPage_[i]->SetVisibleSingle(FALSE);
	}

	m_pButtonTab_[nPage]->SetFlip(TRUE);
	m_pPage_[nPage]->SetVisibleSingle(TRUE);
}

void CGlobalRankingWindow::RESET()
{
	for (int i = 0; i < 10; i++)
		m_pPage_[i]->RESET();
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingWindow::SetRankingKill(WORD wPart, STOP_RANK_KILL sTopKill, int nIndex)
{
	/*switch (emFB)
	{
	case EM_GLOBAL_RANKING_FB_TOP_KILL:		m_pPage_[0]->SetRankingKill(wPart, sTopKill, nIndex);	break;
	case EM_GLOBAL_RANKING_FB_TOP_BRAWLER:	m_pPage_[3]->SetRankingKill(wPart, sTopKill, nIndex);	break;
	case EM_GLOBAL_RANKING_FB_TOP_SWORD:	m_pPage_[4]->SetRankingKill(wPart, sTopKill, nIndex);	break;
	case EM_GLOBAL_RANKING_FB_TOP_ARCHER:	m_pPage_[5]->SetRankingKill(wPart, sTopKill, nIndex);	break;
	case EM_GLOBAL_RANKING_FB_TOP_SHAMAN:	m_pPage_[6]->SetRankingKill(wPart, sTopKill, nIndex);	break;
	case EM_GLOBAL_RANKING_FB_TOP_EXTREME:	m_pPage_[7]->SetRankingKill(wPart, sTopKill, nIndex);	break;
	case EM_GLOBAL_RANKING_FB_TOP_GUNNER:	m_pPage_[8]->SetRankingKill(wPart, sTopKill, nIndex);	break;
	//case EM_GLOBAL_RANKING_FB_TOP_ASSASSIN:	m_pPage_[9]->SetRankingKill(wPart, sTopKill, nIndex);	break;
	//case EM_GLOBAL_RANKING_FB_TOP_MAGICIAN:	m_pPage_[10]->SetRankingKill(wPart, sTopKill, nIndex);	break;
	//case EM_GLOBAL_RANKING_FB_TOP_SHAPER:	m_pPage_[11]->SetRankingKill(wPart, sTopKill, nIndex);	break;
	}*/

	m_pPage_[0]->SetRankingKill(wPart, sTopKill, nIndex);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingWindow::SetRankingKillBr(WORD wPart, STOP_RANK_KILL_BR sTopKill, int nIndex)
{
	m_pPage_[3]->SetRankingKillBr(wPart, sTopKill, nIndex);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingWindow::SetRankingKillSw(WORD wPart, STOP_RANK_KILL_SW sTopKill, int nIndex)
{
	m_pPage_[4]->SetRankingKillSw(wPart, sTopKill, nIndex);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingWindow::SetRankingKillAr(WORD wPart, STOP_RANK_KILL_AR sTopKill, int nIndex)
{
	m_pPage_[5]->SetRankingKillAr(wPart, sTopKill, nIndex);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingWindow::SetRankingKillSh(WORD wPart, STOP_RANK_KILL_SH sTopKill, int nIndex)
{
	m_pPage_[6]->SetRankingKillSh(wPart, sTopKill, nIndex);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingWindow::SetRankingKillEx(WORD wPart, STOP_RANK_KILL_EX sTopKill, int nIndex)
{
	m_pPage_[7]->SetRankingKillEx(wPart, sTopKill, nIndex);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingWindow::SetRankingKillSc(WORD wPart, STOP_RANK_KILL_SC sTopKill, int nIndex)
{
	m_pPage_[8]->SetRankingKillSc(wPart, sTopKill, nIndex);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingWindow::SetRankingKillAs(WORD wPart, STOP_RANK_KILL_AS sTopKill, int nIndex)
{
	m_pPage_[9]->SetRankingKillAs(wPart, sTopKill, nIndex);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingWindow::SetRankingRich(WORD wPart, STOP_RANK_RICH sTopRich, int nIndex)
{
	m_pPage_[1]->SetRankingRich(wPart, sTopRich, nIndex);
}
/*re-coded LG-7 Global Ranking by DevArt22*/
void CGlobalRankingWindow::SetRankingGuild(WORD wPart, STOP_RANK_GUILD sTopGuild, int nIndex)
{
	m_pPage_[2]->SetRankingGuild(wPart, sTopGuild, nIndex);
}