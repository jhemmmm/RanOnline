#include "StdAfx.h"
#include "PlayerRankingDisplay.h"
#include "PlayerRankingDisplayPage.h"
#include "BasicTextBox.h"
#include "d3dfont.h"
#include "GLGaeaClient.h"
#include "../UITextControl.h"
#include "../GameTextControl.h"
//#include "InnerInterface.h"
#include "../BasicTextButton.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


PlayerRankingDisplay::PlayerRankingDisplay()
	:	m_pSwPRankingText ( NULL )
	,	m_pPage_whole ( NULL )
	,	m_pCTFBaseLineBoxHead	( NULL )
	,	m_pCTFBaseLineBoxBody	( NULL )
	,	m_pCtfMyRankingPage ( NULL )
	//,	m_pClubTimeText			( NULL )
{
	memset ( m_pCtfRankingPage, 0, sizeof ( m_pCtfRankingPage ) );
}

PlayerRankingDisplay::~PlayerRankingDisplay()
{
}

void PlayerRankingDisplay::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	//m_pPage_whole = CreateTextButton23 ( "CTF_RESULT_SORTBUTTON_TOTAL", PAGE_WHOLE, (char*)ID2GAMEWORD ( "PLAYER_RESULT_BUTTON", 0 ) );

	m_pCTFBaseLineBoxHead = new CBasicLineBoxEx;
	m_pCTFBaseLineBoxHead->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pCTFBaseLineBoxHead->CreateBaseBoxWhiteOuter ( "PLAYER_RESULT_RANKSLOT_BASE" );
	m_pCTFBaseLineBoxHead->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pCTFBaseLineBoxHead );

	m_pCTFBaseLineBoxBody = new CBasicLineBoxEx;
	m_pCTFBaseLineBoxBody->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pCTFBaseLineBoxBody->CreateBaseBoxWhiteOuter ( "PLAYER_RESULT_RANKSLOT_BODY" );
	m_pCTFBaseLineBoxBody->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pCTFBaseLineBoxBody );

	for ( int i = 0; i < nMAXPAGE; i++ )
	{			
		m_pCtfRankingPage[i] = CreateRankPage ( PAGE_RANKING_0 + i, false );
		if(i==3)m_pCtfRankingPage[i]->SetVisibleSingle(TRUE);
		else m_pCtfRankingPage[i]->SetVisibleSingle(FALSE);
	}	

	m_pCtfMyRankingPage = CreateRankPage(PAGE_MYRANK,true);

	m_pSwPRankingText = new CBasicTextBox;
	m_pSwPRankingText->CreateSub ( this, "PLAYER_RANKING_TEXT" );
	m_pSwPRankingText->SetFont ( pFont );
	m_pSwPRankingText->SetTextAlign ( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pSwPRankingText );	
}

PlayerRankingDisplayPage* PlayerRankingDisplay::CreateRankPage( UIGUID ControlID, bool bMyRank )
{
	PlayerRankingDisplayPage* pRankPage = new PlayerRankingDisplayPage;
	if(!bMyRank) 
	{
		pRankPage->CreateSub ( this, "PLAYER_RESULT_RANKING", UI_FLAG_DEFAULT, ControlID );
		pRankPage->CreateSubControl ( 0, true );
	}
	else
	{
		pRankPage->CreateSub ( this, "PLAYER_RESULT_MYRANKING", UI_FLAG_DEFAULT, ControlID );
		pRankPage->CreateSubControl ( 0, false );
	}
	
	pRankPage->Init();
	RegisterControl ( pRankPage );
	return pRankPage;
}

CBasicTextButton*  PlayerRankingDisplay::CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE40;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON40", UI_FLAG_XSIZE|UI_FLAG_YSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText ,_DEFAULT_FONT_SHADOW_FLAG );
	RegisterControl ( pTextButton );
	return pTextButton;
}

CBasicTextButton*  PlayerRankingDisplay::CreateTextButton ( const char* szButton, UIGUID ControlID, const char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE19;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText );
	pTextButton->SetFlip(FALSE);
	RegisterControl ( pTextButton );
	return pTextButton;
}

void PlayerRankingDisplay::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

/*
void PlayerRankingDisplay::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
		//case PAGE_WHOLE:
		//	{
		//		if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	
		//		{
		//			SetVisiblePage(3);
		//		}
		//	}
		//	break;
	}
}*/

void PlayerRankingDisplay::SetVisiblePage( int page )
{
	m_pPage_whole->SetFlip(false);

	
	for ( int i = 0; i < nMAXPAGE; i++ )
		m_pCtfRankingPage[i]->SetVisibleSingle( FALSE );

	switch( page )
	{
	case 3:
		{
			m_pPage_whole->SetFlip(true);
			RefreshSwPRanking(3);
		}
		break;
	}
}

void PlayerRankingDisplay::RefreshSwPRanking()
{
	for ( int i = 0; i < nMAXPAGE; i++ )
		m_pCtfRankingPage[i]->RefreshRankSlot( false,i );
	m_pCtfMyRankingPage->RefreshRankSlot( true,0 );
}

void PlayerRankingDisplay::RefreshSwPRanking( int page )
{
	m_pCtfRankingPage[page]->RefreshRankSlot( false,page );
	m_pCtfRankingPage[page]->SetVisibleSingle(TRUE);
}