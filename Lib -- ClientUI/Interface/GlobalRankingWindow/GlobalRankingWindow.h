/////////////////////////////////////////////////////////////////////////////
//	FileName	: GlobalRankingWindow.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "../UIWindowEx.h"
#include "../DxGlobalStage.h"

class CBasicTextButton;
class CGlobalRankingPage;
class CGlobalRankingWindow : public CUIWindowEx
{
private:
	enum
	{
		BUTTON_TAB_0	= ET_CONTROL_NEXT,
		BUTTON_TAB_END	= BUTTON_TAB_0 + 10,
	};

private:
	BOOL m_bDelay;

private:
	CBasicTextButton*	m_pButtonTab_[10];
	CUIControl*			m_pWhiteBG;
	CGlobalRankingPage*	m_pPage_[10];

public:
	CGlobalRankingWindow();
	virtual ~CGlobalRankingWindow();

public:
	void CreateSubControl();
	virtual	void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);

private:
	void DefaultPos();
	void SetPage(int nIndex);

public:
	void RESET();
	
	/*re-coded LG-7 Global Ranking by DevArt22*/
	void SetRankingKill(WORD wPart, STOP_RANK_KILL sTopKill, int nIndex);
	void SetRankingKillBr(WORD wPart, STOP_RANK_KILL_BR sTopKill, int nIndex);
	void SetRankingKillSw(WORD wPart, STOP_RANK_KILL_SW sTopKill, int nIndex);
	void SetRankingKillAr(WORD wPart, STOP_RANK_KILL_AR sTopKill, int nIndex);
	void SetRankingKillSh(WORD wPart, STOP_RANK_KILL_SH sTopKill, int nIndex);
	void SetRankingKillEx(WORD wPart, STOP_RANK_KILL_EX sTopKill, int nIndex);
	void SetRankingKillSc(WORD wPart, STOP_RANK_KILL_SC sTopKill, int nIndex);
	void SetRankingKillAs(WORD wPart, STOP_RANK_KILL_AS sTopKill, int nIndex);

	void SetRankingRich(WORD wPart, STOP_RANK_RICH sTopRich, int nIndex);
	void SetRankingGuild(WORD wPart, STOP_RANK_GUILD sTopGuild, int nIndex);

	void SetDelay(BOOL bDelay) { m_bDelay = bDelay; }
};