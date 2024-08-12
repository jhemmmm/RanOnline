#pragma	once

#include "../Lib -- Engine/GUInterface/UIGroup.h"

class	CBasicTextBoxEx;
class	CBasicScrollBarEx;

const DWORD UIMSG_LISTCHAR_LB_DUP = UIMSG_USER1;

class	CProcessListChar : public CUIGroup
{
private:
static	const	int		nOUTOFRANGE;
static	const	int		nLimitList;

private:
	enum
	{
		PROCESS_LIST_TEXTBOX_CHAR = NO_ID + 1,
		PROCESS_LIST_SCROLLBAR_CHAR,
	};

public:
	CProcessListChar ();
	virtual	~CProcessListChar ();

public:
	void	CreateSubControl ();

private:
	CBasicTextBoxEx*	m_pListText;
	CBasicScrollBarEx*	m_pListScrollBar;


public:
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	int			m_nSelectIndex;
	std::string strSelectedText;

public:
	void	ResetList();
	void	LoadList();
	std::string GetText(){ return strSelectedText.c_str(); }
};