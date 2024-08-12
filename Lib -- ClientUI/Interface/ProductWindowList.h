/*!
 * \file ProductWindowType.h
 *
 * \author Juver
 * \date October 2017
 *
 * 
 */
#pragma	once

#include "../Lib -- Engine/GUInterface/UIGroup.h"

class	CBasicTextBoxEx;
class	CBasicScrollBarEx;

const DWORD UIMSG_PRODUCT_WINDOW_LIST_SELECT = UIMSG_USER1;

class CProductWindowList : public CUIGroup
{
private:
	enum
	{
		PRODUCT_WINDOW_LIST_TEXTBOX = NO_ID + 1,
		PRODUCT_WINDOW_LIST_SCROLLBAR,
	};

public:
	CProductWindowList ();
	virtual	~CProductWindowList ();

public:
	void	CreateSubControl ();

private:
	CBasicTextBoxEx*	m_pListTextBox;
	CBasicScrollBarEx*	m_pListScrollBar;

	int		m_nSelectIndex;
	DWORD	m_dwSelectedData;
	std::string m_strSelectedText;

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	void	LoadData( WORD wCategory, WORD wFilter );
	void	ClearData();
	DWORD	GetSelectData()	{ return m_dwSelectedData; }
	std::string GetSelectText() { return m_strSelectedText; }
};