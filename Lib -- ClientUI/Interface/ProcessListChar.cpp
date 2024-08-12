#include "StdAfx.h"
#include "ProcessListChar.h"
#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "BasicTextBoxEx.h"
#include "BasicScrollBarEx.h"
#include "BasicScrollThumbFrame.h"
#include "BasicLineBox.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "GLGaeaClient.h"
#include "GLProcessInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const	int	CProcessListChar::nOUTOFRANGE = -1;
const	int CProcessListChar::nLimitList = 1000;

CProcessListChar::CProcessListChar () 
	: m_nSelectIndex ( nOUTOFRANGE )	
	, strSelectedText("")
{
}

CProcessListChar::~CProcessListChar ()
{
}

void CProcessListChar::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	
	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "PROCESS_LIST_NAMELIST_BACK" );
	RegisterControl ( pBasicLineBox );

	CBasicTextBoxEx* pTextBox = new CBasicTextBoxEx;
	pTextBox->CreateSub ( this, "PROCESS_LIST_NAMELIST_TEXTBOX", UI_FLAG_DEFAULT, PROCESS_LIST_TEXTBOX_CHAR );
	pTextBox->SetFont ( pFont9 );		
	pTextBox->SetLineInterval ( 3.0f );
	pTextBox->SetSensitive ( true );
	pTextBox->SetLimitLine ( nLimitList );
	RegisterControl ( pTextBox );
	m_pListText = pTextBox;

	int nTotalLine = pTextBox->GetVisibleLine ();
	CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx;
	pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, PROCESS_LIST_SCROLLBAR_CHAR );
	pScrollBar->CreateBaseScrollBar ( "PROCESS_LIST_NAMELIST_SCROLLBAR" );
	pScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( pScrollBar );
	m_pListScrollBar = pScrollBar;
	
}

void CProcessListChar::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_pListText->SetUseOverColor ( FALSE );
	if ( m_pListText->GetCount () && 0 <= m_nSelectIndex )
		m_pListText->SetUseTextColor ( m_nSelectIndex, FALSE );
	
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_pListText->GetCount () && 0 <= m_nSelectIndex )
	{
		m_pListText->SetUseTextColor ( m_nSelectIndex, TRUE );
		m_pListText->SetTextColor ( m_nSelectIndex, NS_UITEXTCOLOR::ORANGE );
	}
	
	CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame ();

	const int nTotalLine = m_pListText->GetTotalLine ();
	const int nLinePerOneView = m_pListText->GetVisibleLine ();	
	pThumbFrame->SetState ( nTotalLine, nLinePerOneView );
	if ( nLinePerOneView < nTotalLine )
	{
		const int nMovableLine = nTotalLine - nLinePerOneView;
		float fPercent = pThumbFrame->GetPercent ();
		int nPos = (int)floor(fPercent * nMovableLine);
		m_pListText->SetCurLine ( nPos );			
	}	
}

void CProcessListChar::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case PROCESS_LIST_TEXTBOX_CHAR:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = m_pListText->GetSelectPos ();
				if ( m_pListText->GetCount () <= nIndex ) return ;

				if ( UIMSG_LB_UP & dwMsg )
				{
					if ( nIndex != -1 )
						strSelectedText = m_pListText->GetText(nIndex).GetString();
					else
						strSelectedText = "";

					m_nSelectIndex = nIndex;
				}

				if ( UIMSG_LB_DUP & dwMsg ) AddMessageEx ( UIMSG_LISTCHAR_LB_DUP );

				m_pListText->SetUseOverColor ( TRUE );
				m_pListText->SetOverColor ( nIndex, NS_UITEXTCOLOR::ORANGE );
			}
		}
		break;
	}
}

void CProcessListChar::ResetList ()
{
	m_nSelectIndex = -1;
	strSelectedText = "";
	m_pListText->ClearText ();
}

void CProcessListChar::LoadList ()
{
	ResetList();

	PROCESS_INFO_DATA_MAP_CLIENT mapClient = GLGaeaClient::GetInstance().m_mapProcessInfo;
	if ( mapClient.size() == 0 )	return;

	PROCESS_INFO_DATA_MAP_CLIENT_ITER iter = mapClient.begin();
	for( ; iter != mapClient.end(); ++iter )
	{
		std::string strNAME = (*iter).first;
		m_pListText->AddText ( strNAME.c_str() );
	}
}
