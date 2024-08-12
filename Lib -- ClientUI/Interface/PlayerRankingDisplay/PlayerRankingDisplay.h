#pragma	once

#include "../UIWindowEx.h"
#include "../BasicLineBoxEx.h"

class	CBasicTextBox;
class	CBasicTextButton;
class	PlayerRankingDisplayPage;

class PlayerRankingDisplay : public CUIWindowEx
{
protected:
	enum
	{
		PAGE_WHOLE = ET_CONTROL_NEXT,
		PAGE_SG,
		PAGE_MP,
		PAGE_PHX,
		BUTTON_CLOSE,
		PAGE_RANKING_0,
		PAGE_RANKING_1,
		PAGE_RANKING_2,
		PAGE_RANKING_3,
		PAGE_MYRANK
	};

	enum
	{
		nMAXPAGE = 4,
	};

private:
	CBasicTextBox*			m_pSwPRankingText;
	CBasicTextButton*		m_pPage_whole;

	CBasicLineBoxEx*		m_pCTFBaseLineBoxHead;
	CBasicLineBoxEx*		m_pCTFBaseLineBoxBody;

	PlayerRankingDisplayPage*		m_pCtfRankingPage[nMAXPAGE];
	PlayerRankingDisplayPage*		m_pCtfMyRankingPage;

	PlayerRankingDisplayPage*		CreateRankPage ( UIGUID ControlID, bool bMyRank );

public:
	PlayerRankingDisplay();
	~PlayerRankingDisplay();

	void	CreateSubControl ();
	CBasicTextButton*	CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText );
	CBasicTextButton*	CreateTextButton ( const char* szButton, UIGUID ControlID, const char* szText );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	//virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:	
	void	RefreshSwPRanking();
	void	RefreshSwPRanking(int page);
	void	SetVisiblePage(int page);
};