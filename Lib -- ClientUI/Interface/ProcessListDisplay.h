/*!
 * \file ProcessListDisplay.h
 *
 * \author Juver
 * \date June 2017
 *
 * 
 */

#pragma	once

#include "UIWindowEx.h"

class	CProcessListChar;
class	CProcessListProc;

class CProcessListDisplay : public CUIWindowEx
{
public:
	enum
	{
		PROCESS_LIST_CHAR = ET_CONTROL_NEXT,
		PROCESS_LIST_PROC,
	};

public:
	CProcessListDisplay ();
	virtual	~CProcessListDisplay ();

public:
	void	CreateSubControl ();

private:
	CProcessListChar*		m_pListChar;
	CProcessListProc*		m_pListProc;

public:
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	RefreshList ();

};