#include "StdAfx.h"
#include "PlayerRankingDisplayPage.h"
#include "PlayerRankingDisplaySlot.h"
#include "../BasicTextBoxEx.h"
#include "BasicScrollThumbFrame.h"
#include "d3dfont.h"
#include "GLGaeaClient.h"
#include "../UITextControl.h"
#include "../GameTextControl.h"
#include "../BasicScrollBarEx.h"
#include "../BasicTextButton.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int PlayerRankingDisplayPage::nSTARTLINE = 0;
const int PlayerRankingDisplayPage::nOUTOFRANGE = -1;

PlayerRankingDisplayPage::PlayerRankingDisplayPage()
	:	m_pCTFBaseRank			( NULL )
	,	m_pCTFBaseSchool		( NULL )
	,	m_pCTFBaseClass			( NULL )
	,	m_pCTFBaseCharName		( NULL )
	,	m_pCTFBaseKill			( NULL )
	,	m_pCTFBaseDeath			( NULL )
	,	m_pCTFBaseKdr			( NULL )
	,	m_pScrollBar			( NULL )
	,	m_pLineBoxMyRankHead	( NULL )
	,	m_pLineBoxMyRankTail	( NULL )
{
	memset ( m_pRankSlot, 0, sizeof ( PlayerRankingDisplaySlot* ) * nMAX_RANK_SLOT );
	memset ( m_pRankSlotArrayDummy, 0, sizeof ( CUIControl* ) * nMAX_ONE_VIEW_SLOT );
}

PlayerRankingDisplayPage::~PlayerRankingDisplayPage()
{
}

void PlayerRankingDisplayPage::CreateSubControl ( int nRankCount, bool bMyRank )
{				
	//if ( nRankCount < 1 ) return ;

	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CString strSlotKeyword[nMAX_ONE_VIEW_SLOT] =
	{
		"PLAYER_RESULT_SLOT_0",
		"PLAYER_RESULT_SLOT_1",
		"PLAYER_RESULT_SLOT_2",
		"PLAYER_RESULT_SLOT_3",
		"PLAYER_RESULT_SLOT_4",
		"PLAYER_RESULT_SLOT_5",
		"PLAYER_RESULT_SLOT_6",
		"PLAYER_RESULT_SLOT_7",
		"PLAYER_RESULT_SLOT_8",
		"PLAYER_RESULT_SLOT_9"
	};
	
	m_pLineBoxMyRankHead = new CBasicLineBoxEx;
	m_pLineBoxMyRankHead->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_DIALOGUE_WHITE_BACK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxMyRankHead->CreateBaseBoxSkill ( "PLAYER_RESULT_MYRANK_LINE_HEAD" );
	m_pLineBoxMyRankHead->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxMyRankHead );

	m_pLineBoxMyRankTail = new CBasicLineBoxEx;
	m_pLineBoxMyRankTail->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_DIALOGUE_WHITE_BACK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxMyRankTail->CreateBaseBoxSkill ( "PLAYER_RESULT_MYRANK_LINE_TAIL" );
	m_pLineBoxMyRankTail->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxMyRankTail );
	

	m_pCTFBaseRank = new CBasicTextBoxEx;
	m_pCTFBaseRank->CreateSub ( this, "CTF_RESULT_TEXT_BASE_RANK" );
	m_pCTFBaseRank->SetFont ( pFont );
	m_pCTFBaseRank->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	RegisterControl ( m_pCTFBaseRank );	

	m_pCTFBaseSchool = new CBasicTextBoxEx;
	m_pCTFBaseSchool->CreateSub ( this, "CTF_RESULT_TEXT_BASE_SCHOOL" );
	m_pCTFBaseSchool->SetFont ( pFont );
	m_pCTFBaseSchool->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	RegisterControl ( m_pCTFBaseSchool );	

	m_pCTFBaseClass = new CBasicTextBoxEx;
	m_pCTFBaseClass->CreateSub ( this, "CTF_RESULT_TEXT_BASE_CLASS" );
	m_pCTFBaseClass->SetFont ( pFont );
	m_pCTFBaseClass->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	RegisterControl ( m_pCTFBaseClass );	

	m_pCTFBaseCharName = new CBasicTextBoxEx;
	m_pCTFBaseCharName->CreateSub ( this, "CTF_RESULT_TEXT_BASE_NAME" );
	m_pCTFBaseCharName->SetFont ( pFont );
	m_pCTFBaseCharName->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	RegisterControl ( m_pCTFBaseCharName );	

	m_pCTFBaseKill = new CBasicTextBoxEx;
	m_pCTFBaseKill->CreateSub ( this, "PLAYER_RESULT_TEXT_BASE_KILL" );
	m_pCTFBaseKill->SetFont ( pFont );
	m_pCTFBaseKill->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	RegisterControl ( m_pCTFBaseKill );	

	m_pCTFBaseDeath = new CBasicTextBoxEx;
	m_pCTFBaseDeath->CreateSub ( this, "PLAYER_RESULT_TEXT_BASE_DEATH" );
	m_pCTFBaseDeath->SetFont ( pFont );
	m_pCTFBaseDeath->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	RegisterControl ( m_pCTFBaseDeath );	

	m_pCTFBaseKdr = new CBasicTextBoxEx;
	m_pCTFBaseKdr->CreateSub ( this, "PLAYER_RESULT_TEXT_BASE_KDR" );
	m_pCTFBaseKdr->SetFont ( pFont );
	m_pCTFBaseKdr->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	RegisterControl ( m_pCTFBaseKdr );	

	for ( int i = 0; i < nMAX_ONE_VIEW_SLOT; i++ )
	{
		m_pRankSlotArrayDummy[i] = CreateControl ( strSlotKeyword[i].GetString () );
	}
	if(bMyRank){
		for ( int i = 0; i < nMAX_RANK_SLOT; i++ )
		{
			m_pRankSlot[i] = CreateRankSlot ( strSlotKeyword[0], CTF_RESULT_SLOT_0 + i, i+1 ,NS_UITEXTCOLOR::WHITE);
			
		}
	}
	else m_pRankSlot[0] = CreateRankSlot ( "PLAYER_RESULT_MYRANK_SLOT", CTF_RESULT_MYSLOT, 100 ,NS_UITEXTCOLOR::GREENYELLOW);
	

	//m_pMyRankSlot = CreateRankSlot("CTF_RESULT_MYRANK_SLOT", CTF_RESULT_MYSLOT, 1,NS_UITEXTCOLOR::BRIGHTGREEN );
	//m_pMyRankSlot->SetVisibleSingle(TRUE);

	if ( nRankCount < nMAX_ONE_VIEW_SLOT )
	{
		nRankCount = nMAX_ONE_VIEW_SLOT;
	}
	m_nMaxRankCnt = nRankCount;

	m_vecViewRank.clear();

	CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx;
	pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE );
	pScrollBar->CreateNewBaseScrollBar ( "PLAYER_RESULT_RANK_SCROLLBAR" ); //createbasescrollbarblack
	pScrollBar->GetThumbFrame()->SetState ( nRankCount, nMAX_ONE_VIEW_SLOT );
	RegisterControl ( pScrollBar );
	m_pScrollBar = pScrollBar;

	if(!bMyRank)
	{
		m_pCTFBaseRank->SetVisibleSingle(FALSE);
		m_pCTFBaseSchool->SetVisibleSingle(FALSE);
		m_pCTFBaseClass->SetVisibleSingle(FALSE);
		m_pCTFBaseCharName->SetVisibleSingle(FALSE);
		m_pCTFBaseKill->SetVisibleSingle(FALSE);
		m_pCTFBaseDeath->SetVisibleSingle(FALSE);
		m_pLineBoxMyRankHead->SetVisibleSingle(FALSE);
		m_pLineBoxMyRankTail->SetVisibleSingle(FALSE);
		m_pCTFBaseKdr->SetVisibleSingle(FALSE);
		m_pScrollBar->SetVisibleSingle(FALSE);
	}
}

CUIControl*	PlayerRankingDisplayPage::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

PlayerRankingDisplaySlot* PlayerRankingDisplayPage::CreateRankSlot ( CString strKeyword, UIGUID ControlID, int index, D3DCOLOR dwColor )
{
	PlayerRankingDisplaySlot* pRankSlot = new PlayerRankingDisplaySlot;
	pRankSlot->CreateSub ( this, strKeyword.GetString (), UI_FLAG_DEFAULT, ControlID );
	pRankSlot->CreateSubControl ( dwColor );
	RegisterControl ( pRankSlot );
	return pRankSlot;
}

void PlayerRankingDisplayPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	m_nSlotIndex = nOUTOFRANGE;
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_pScrollBar )
	{
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nViewPerPage = pThumbFrame->GetViewPerPage ();
			int nBeforeRankCnt = m_vecViewRank.size();

			m_vecViewRank.clear();
			for ( int i = 0; i < m_nMaxRankCnt; i++ )
			{
				m_vecViewRank.push_back( i );
			}

			if( nBeforeRankCnt != m_vecViewRank.size() )
			{
				m_pScrollBar->GetThumbFrame()->SetState ( m_vecViewRank.size(), nMAX_ONE_VIEW_SLOT );
				ResetAllRankSlotRender( m_nMaxRankCnt );
				SetRankSlotRender ( 0, 0 + nViewPerPage );			
			}
			
		}
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nTotal = pThumbFrame->GetTotal ();
			if ( nTotal <= nMAX_ONE_VIEW_SLOT ) return ;

			const int nViewPerPage = pThumbFrame->GetViewPerPage ();

			if ( nViewPerPage < nTotal )
			{
				int nCurPos = nSTARTLINE;
				const int nMovableLine = nTotal - nViewPerPage;
				float fPercent = pThumbFrame->GetPercent ();

				nCurPos = (int)floor(fPercent * nMovableLine);
				if ( nCurPos < nSTARTLINE ) nCurPos = nSTARTLINE;

				if ( m_nCurPos == nCurPos ) return;

				m_nCurPos = nCurPos;

				ResetAllRankSlotRender ( nTotal );
				SetRankSlotRender ( nCurPos, nCurPos + nViewPerPage );
			}
		}
	}
}

void PlayerRankingDisplayPage::ResetAllRankSlotRender ( int nTotal )
{
	if ( nTotal < 0 ) return ;

	for ( int i = 0; i < nMAX_RANK_SLOT; i++ )
	{
		PlayerRankingDisplaySlot* pRankSlot = m_pRankSlot[i];
		if ( pRankSlot )
		{
			pRankSlot->SetVisibleSingle ( FALSE );
		}
	}
}

void PlayerRankingDisplayPage::SetRankSlotRender ( int nStartIndex, int nTotal )
{
	if ( nTotal < 0 ) return ;

	const UIRECT& rcParentPos = GetGlobalPos ();

	int iViewIndex = 0; 
	int iSlotNum   = 0;	
	for ( int i = nStartIndex; i < m_vecViewRank.size(); i++ )
	{
		if( i >= m_nMaxRankCnt ) break;
		if( i >= nTotal ) break;

		iSlotNum = m_vecViewRank[i];

		PlayerRankingDisplaySlot* pRankSlot = m_pRankSlot[iSlotNum];
		if ( pRankSlot )
		{
						
			int nAbsoluteIndex = iViewIndex;
			CUIControl* pDummyControl = m_pRankSlotArrayDummy[nAbsoluteIndex];

			const UIRECT& rcSlotPos = pDummyControl->GetGlobalPos ();
			const UIRECT& rcSlotLocalPos = pDummyControl->GetLocalPos ();

			pRankSlot->SetLocalPos ( rcSlotLocalPos );			
			pRankSlot->SetGlobalPos ( rcSlotPos );

			pRankSlot->SetVisibleSingle ( TRUE );

			iViewIndex++;
		}
	}

	if( m_vecViewRank.size() < nTotal )
	{
		int iMaxLackTotal = nTotal - m_vecViewRank.size();		
		for( i = 0; i < iMaxLackTotal; i++ )
		{
			iSlotNum++;
			PlayerRankingDisplaySlot* pRankSlot = m_pRankSlot[iSlotNum];
			if ( pRankSlot )
			{

				int nAbsoluteIndex = iViewIndex;
				CUIControl* pDummyControl = m_pRankSlotArrayDummy[nAbsoluteIndex];

				const UIRECT& rcSlotPos = pDummyControl->GetGlobalPos ();
				const UIRECT& rcSlotLocalPos = pDummyControl->GetLocalPos ();

				pRankSlot->SetLocalPos ( rcSlotLocalPos );			
				pRankSlot->SetGlobalPos ( rcSlotPos );

				pRankSlot->SetVisibleSingle ( TRUE );

				iViewIndex++;
			}
		}
	}


	
	for( i = nTotal; i < m_vecViewRank.size(); i++ )
	{
		iSlotNum = m_vecViewRank[i];

		if( m_pRankSlot[iSlotNum] ) m_pRankSlot[iSlotNum]->SetVisibleSingle( FALSE );
	}
}

void PlayerRankingDisplayPage::Init()
{
	m_pCTFBaseRank->ClearText();
	m_pCTFBaseSchool->ClearText();
	m_pCTFBaseClass->ClearText();
	m_pCTFBaseCharName->ClearText();
	m_pCTFBaseKill->ClearText();
	m_pCTFBaseDeath->ClearText();
	m_pCTFBaseKdr->ClearText();
	
	m_pCTFBaseRank->AddText( ID2GAMEWORD( "CTF_RESULT_TEXT_BASE", 0 ), NS_UITEXTCOLOR::WHITE);
	m_pCTFBaseSchool->AddText( ID2GAMEWORD( "CTF_RESULT_TEXT_BASE", 1 ), NS_UITEXTCOLOR::WHITE);
	m_pCTFBaseClass->AddText( ID2GAMEWORD( "CTF_RESULT_TEXT_BASE", 2 ), NS_UITEXTCOLOR::WHITE);
	m_pCTFBaseCharName->AddText( ID2GAMEWORD( "CTF_RESULT_TEXT_BASE", 3 ), NS_UITEXTCOLOR::WHITE);
	m_pCTFBaseKill->AddText( ID2GAMEWORD( "PLAYER_RANK_TEXT_BASE", 0 ), NS_UITEXTCOLOR::WHITE);
	m_pCTFBaseDeath->AddText( ID2GAMEWORD( "PLAYER_RANK_TEXT_BASE", 1 ), NS_UITEXTCOLOR::WHITE);
	m_pCTFBaseKdr->AddText( ID2GAMEWORD( "PLAYER_RANK_TEXT_BASE", 2 ), NS_UITEXTCOLOR::WHITE);
}

void PlayerRankingDisplayPage::RefreshRankSlot( bool bMyRank, int wSchool )
{
	if( !bMyRank )
	{
		PLAYER_RANKING_STATUS_VEC& vecPRank = GLGaeaClient::GetInstance().m_vecPlayerRank;
		int nRankNum = GLGaeaClient::GetInstance().m_vecPlayerRank.size();
		SetRankCnt( vecPRank.size() );
		int slotCnt = 0;

		for(int i=0;i<nRankNum;i++)
		{
			if( wSchool == vecPRank[i].wSchool )
			{
				/*
					m_pRankSlot[slotCnt]->Init(vecPRank[i].wCharRanking,
										vecPRank[i].wSchool,
										vecPRank[i].dwClass,
										vecPRank[i].szCharName,
										vecPRank[i].wKillNum,
										vecPRank[i].wDeathNum,
										false);
										*/
					slotCnt++;
					SetRankCnt( slotCnt );
			}
			else if( wSchool == 3)
			{
				m_pRankSlot[i]->Init(vecPRank[i].wCharRanking,
		                				vecPRank[i].wSchool,
										vecPRank[i].dwClass,
										vecPRank[i].szCharName,
										vecPRank[i].wKillNum,
										vecPRank[i].wDeathNum,
										false);
									
				slotCnt++;
				SetRankCnt( slotCnt );
			}
		}
	}
	else
	{
		const PLAYER_RANKING_EX& sMyRank = GLGaeaClient::GetInstance().m_sMyPKRank;
		m_pRankSlot[0]->Init(sMyRank.wCharRanking,
		sMyRank.wSchool,
		sMyRank.dwClass,
		sMyRank.szCharName,
		sMyRank.wKillNum,
		sMyRank.wDeathNum,
		true);
	}
}