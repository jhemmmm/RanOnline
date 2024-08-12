#include "StdAfx.h"
#include "ProcessListDisplay.h"
#include "InnerInterface.h"
#include "ProcessListChar.h"
#include "ProcessListProc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CProcessListDisplay::CProcessListDisplay ()
	: m_pListChar(NULL)
	, m_pListProc(NULL)
{
}

CProcessListDisplay::~CProcessListDisplay ()
{
}

void	CProcessListDisplay::CreateSubControl ()
{
	m_pListChar = new CProcessListChar;
	m_pListChar->CreateSub ( this, "PROCESS_LIST_NAMELIST", UI_FLAG_DEFAULT, PROCESS_LIST_CHAR );
	m_pListChar->CreateSubControl ();
	RegisterControl ( m_pListChar );


	m_pListProc = new CProcessListProc;
	m_pListProc->CreateSub ( this, "PROCESS_LIST_PROCLIST", UI_FLAG_DEFAULT, PROCESS_LIST_PROC );
	m_pListProc->CreateSubControl ();
	RegisterControl ( m_pListProc );
}

void	CProcessListDisplay::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( QUEST_WINDOW );
			}
		}
		break;
	case PROCESS_LIST_CHAR:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( UIMSG_LB_UP & dwMsg )
				{
					std::string strTEXT = m_pListChar->GetText();
					if ( strTEXT.size() <= 0 )	return;
					
					if ( m_pListProc )
						m_pListProc->LoadList( strTEXT.c_str() );
				}
			}
		}
		break;
	};
}

void CProcessListDisplay::RefreshList()
{
	if ( m_pListChar )
	{
		m_pListChar->ResetList();
		m_pListChar->LoadList();
	}

	if ( m_pListProc )
		m_pListProc->ResetList();
}
