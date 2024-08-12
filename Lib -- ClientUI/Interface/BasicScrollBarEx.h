#pragma	once

#include "./BasicScrollBar.h"

class	CBasicScrollBarEx : public CBasicScrollBar
{
public:
	CBasicScrollBarEx ();
	virtual	~CBasicScrollBarEx ();

public:
    void	CreateBaseScrollBar ( char* szBaseScrollBar, const bool& bVERTICAL = true );
	void	CreateNewBaseScrollBar ( char* szNewBaseScrollBar, const bool& bVERTICAL = true );
	//dmk14 | 10-31-16 | ep10 gui
	void	CreateBaseScrollBarBlack ( char* szBaseScrollBar, const bool& bVERTICAL = true );
};