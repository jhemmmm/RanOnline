#include "StdAfx.h"
#include "ProductWindowList.h"

#include "../Lib -- Engine/DxCommon/DxFontMan.h"
#include "UITextControl.h"
#include "GameTextControl.h"

#include "BasicTextBoxEx.h"
#include "BasicScrollBarEx.h"
#include "BasicScrollThumbFrame.h"

#include "GLItemMixMan.h"
#include "GLItemMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CProductWindowList::CProductWindowList ()
	: m_pListTextBox(NULL)
	, m_pListScrollBar(NULL)
	, m_nSelectIndex(-1)
	, m_dwSelectedData(0)
	, m_strSelectedText("")
{
}

CProductWindowList::~CProductWindowList ()
{
}

void CProductWindowList::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pListTextBox = new CBasicTextBoxEx;
	m_pListTextBox->CreateSub ( this, "RAN_PPRODUCT_WINDOW_LIST_TEXTBOX", UI_FLAG_DEFAULT, PRODUCT_WINDOW_LIST_TEXTBOX );
	m_pListTextBox->SetFont ( pFont9 );		
	m_pListTextBox->SetLineInterval ( 3.0f );
	m_pListTextBox->SetSensitive ( true );
	m_pListTextBox->SetLimitLine ( 10000 );
	//m_pListTextBox->CreateMouseClick( NS_UITEXTCOLOR::ENABLE );
	//m_pListTextBox->CreateMouseOver( NS_UITEXTCOLOR::ENABLE  );
	RegisterControl ( m_pListTextBox );

	int nTotalLine = m_pListTextBox->GetVisibleLine ();

	m_pListScrollBar = new CBasicScrollBarEx;
	m_pListScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, PRODUCT_WINDOW_LIST_SCROLLBAR );
	m_pListScrollBar->CreateBaseScrollBar ( "RAN_PPRODUCT_WINDOW_LIST_SCROLL" );
	m_pListScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( m_pListScrollBar );
}

void CProductWindowList::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case PRODUCT_WINDOW_LIST_TEXTBOX:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = m_pListTextBox->GetSelectPos ();
				if ( m_pListTextBox->GetCount () <= nIndex || nIndex < 0  ) return ;

				if ( UIMSG_LB_UP & dwMsg )
				{
					m_dwSelectedData = m_pListTextBox->GetTextData ( nIndex );
					m_strSelectedText = m_pListTextBox->GetText( nIndex );
					m_nSelectIndex = nIndex;

					AddMessageEx ( UIMSG_PRODUCT_WINDOW_LIST_SELECT );
				}

				m_pListTextBox->SetUseOverColor ( TRUE );
				m_pListTextBox->SetOverColor ( nIndex, NS_UITEXTCOLOR::ORANGE );
			}
		}break;
	};
}

void CProductWindowList::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_pListTextBox->SetUseOverColor ( FALSE );
	if ( m_pListTextBox->GetCount () && 0 <= m_nSelectIndex )
		m_pListTextBox->SetUseTextColor ( m_nSelectIndex, FALSE );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_pListTextBox->GetCount () && 0 <= m_nSelectIndex )
	{
		m_pListTextBox->SetUseTextColor ( m_nSelectIndex, TRUE );
		m_pListTextBox->SetTextColor ( m_nSelectIndex, NS_UITEXTCOLOR::ORANGE );
	}

	CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame ();
	const int nTotalLine = m_pListTextBox->GetTotalLine ();
	const int nLinePerOneView = m_pListTextBox->GetVisibleLine ();
	pThumbFrame->SetState ( nTotalLine, nLinePerOneView );
	if ( nLinePerOneView < nTotalLine )
	{
		const int nMovableLine = nTotalLine - nLinePerOneView;
		float fPercent = pThumbFrame->GetPercent ();
		int nPos = (int)floor(fPercent * nMovableLine);
		m_pListTextBox->SetCurLine ( nPos );			
	}
}

void CProductWindowList::LoadData( WORD wCategory, WORD wFilter )
{
	ClearData();

	ITEMMIX_KEYMAP &mapkeys = GLItemMixMan::GetInstance().m_mapKeyProduct;

	ITEMMIX_VEC vecitemmix;
	vecitemmix.reserve( mapkeys.size() );

	for( ITEMMIX_KEYMAP_ITER iter = mapkeys.begin();
		iter != mapkeys.end(); ++iter )
	{
		const ITEM_MIX& item_mix_data = (*iter).second;

		if ( wCategory > 0 )
		{
			WORD wcategory_check = wCategory - 1;

			if ( item_mix_data.wCategory != wcategory_check )
				continue;
		}

		vecitemmix.push_back( item_mix_data );
	}

	std::sort( vecitemmix.begin(), vecitemmix.end() );

	for ( int i=0; i<(int)vecitemmix.size(); ++i )
	{
		const ITEM_MIX& item_mix_data = vecitemmix[i];
		
		SITEM* pitem_data = GLItemMan::GetInstance().GetItem( item_mix_data.sResultItem.sNID );
		if ( !pitem_data )	continue;

		int nIndex = m_pListTextBox->AddText ( pitem_data->GetName(), COMMENT::ITEMCOLOR[pitem_data->sBasicOp.emLevel] );
		m_pListTextBox->SetTextImage( nIndex, "RAN_PRODUCT_WINDOW_TYPE_STATUS_NOTHING" );
		m_pListTextBox->SetTextData ( nIndex, item_mix_data.dwKey );
	}

	//select first line 
	if ( m_pListTextBox->GetCount () > 0 )
	{
		m_pListTextBox->SetSelectPos( 0 );
		int nIndex = m_pListTextBox->GetSelectPos ();
		if ( m_pListTextBox->GetCount () <= nIndex || nIndex < 0  ) return;

		m_dwSelectedData = m_pListTextBox->GetTextData ( nIndex );
		m_strSelectedText = m_pListTextBox->GetText( nIndex );
		m_nSelectIndex = nIndex;
		
		m_pListTextBox->SetUseOverColor ( TRUE );
		m_pListTextBox->SetOverColor ( nIndex, NS_UITEXTCOLOR::ORANGE );
	}
}

void CProductWindowList::ClearData()
{
	m_dwSelectedData = UINT_MAX;
	m_strSelectedText = "";
	m_nSelectIndex  = -1;
	m_pListTextBox->ClearText();
}