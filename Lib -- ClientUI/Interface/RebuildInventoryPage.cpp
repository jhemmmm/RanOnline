#include "StdAfx.h"
#include "RebuildInventoryPage.h"

#include "ItemSlotEx.h"
#include "BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "./BasicTextBox.h"
#include "GLGaeaClient.h"

#include "./BasicButton.h"
#include "d3dfont.h"
#include "UITextControl.h"
#include "BasicTextBox.h"
#include "GameTextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------
// RebuildInventoryPage : 최준혁
//--------------------------------------------------------------------
const INT CRebuildInventoryPage::nSTARTLINE = 0;	// ITEMREBUILD_MARK
const INT CRebuildInventoryPage::nOUTOFRANGE = -1;

CRebuildInventoryPage::CRebuildInventoryPage()
	: m_nONE_VIEW_SLOT( nONE_VIEW_SLOT_DEFAULT )
	, m_nCurPos ( -1 )
{
	for( INT i=0; i<EM_INVENSIZE_Y; ++i )
		m_pItemSlotArray[i] = NULL;

	for( i=0; i<nMAX_ONE_VIEW_SLOT; ++i )
		m_pItemSlotArrayDummy[i] = NULL;

	//inven page
	for ( int i = 0 ; i < nMAX_PAGES; ++i )	
	{
		m_pPages[i] = NULL;
		m_pPagesText[i] = NULL;
	}
}

CRebuildInventoryPage::~CRebuildInventoryPage()
{
}

VOID CRebuildInventoryPage::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_FLAG );

	std::string strPages[nMAX_PAGES] = 
	{
		"REBUILDINVENTORY_WINDOW_RADIO_BUTTON_1",
		"REBUILDINVENTORY_WINDOW_RADIO_BUTTON_2",
		"REBUILDINVENTORY_WINDOW_RADIO_BUTTON_3"
	};

	std::string strPagesText[nMAX_PAGES] = 
	{
		"REBUILDINVENTORY_WINDOW_RADIO_BUTTON_TEXT_1",
		"REBUILDINVENTORY_WINDOW_RADIO_BUTTON_TEXT_2",
		"REBUILDINVENTORY_WINDOW_RADIO_BUTTON_TEXT_3"
	};

	for ( int i = 0 ; i < nMAX_PAGES; ++i ) 
	{
		m_pPages[i] = new CBasicButton;
		m_pPages[i]->CreateSub ( this, strPages[i].c_str(), UI_FLAG_DEFAULT, PAGE_BUTTON0 + i );
		m_pPages[i]->CreateFlip ( "INVENTORY_WINDOW_RADIO_BUTTON_F", CBasicButton::RADIO_FLIP );
		m_pPages[i]->SetFlip( false );
		m_pPages[i]->SetVisibleSingle ( TRUE );
		RegisterControl ( m_pPages[i] );

		CString strNumber;
		strNumber.Format( "%d", i+1 );

		m_pPagesText[i] = new CBasicTextBox;
		m_pPagesText[i]->CreateSub ( this, strPagesText[i].c_str(), UI_FLAG_DEFAULT );
		m_pPagesText[i]->SetFont ( pFont8 );
		m_pPagesText[i]->SetTextAlign ( TEXT_ALIGN_LEFT );	
		m_pPagesText[i]->AddText ( strNumber.GetString(), NS_UITEXTCOLOR::WHITE );
		RegisterControl ( m_pPagesText[i] );
	}

	CString strSlotKeyword[ nMAX_ONE_VIEW_SLOT ] =
	{
		"REBUILDINVENTORYSLOT_ITEM_SLOT0",
		"REBUILDINVENTORYSLOT_ITEM_SLOT1",
		"REBUILDINVENTORYSLOT_ITEM_SLOT2",
		"REBUILDINVENTORYSLOT_ITEM_SLOT3",
		"REBUILDINVENTORYSLOT_ITEM_SLOT4",
		"REBUILDINVENTORYSLOT_ITEM_SLOT5",
		"REBUILDINVENTORYSLOT_ITEM_SLOT6",
		"REBUILDINVENTORYSLOT_ITEM_SLOT7",
		"REBUILDINVENTORYSLOT_ITEM_SLOT8",
		"REBUILDINVENTORYSLOT_ITEM_SLOT9"
	};

	for( INT i=0; i<nMAX_ONE_VIEW_SLOT; ++i )
		m_pItemSlotArrayDummy[i] = CreateControl( strSlotKeyword[i].GetString() );

	for( INT i=0; i<EM_INVENSIZE_Y; ++i )
		m_pItemSlotArray[i] = CreateItemSlot( strSlotKeyword[0], ITEM_SLOT0 + i );

	{
		/*
		m_pScrollBar = new CBasicScrollBarEx;
		m_pScrollBar->CreateSub( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE );
		m_pScrollBar->CreateBaseScrollBar( "REBUILDINVENTORY_PAGE_SCROLL" );
		m_pScrollBar->GetThumbFrame()->SetState( EM_INVENSIZE_Y, m_nONE_VIEW_SLOT );
		RegisterControl( m_pScrollBar );
		*/
	}

	SetItemSlotRender ( nSTARTLINE, nMAX_ONE_VIEW_SLOT );
	OpenPage ( 0 );
}

CItemSlotEx* CRebuildInventoryPage::CreateItemSlot( CString strKeyword, UIGUID ControlID )
{
	CItemSlotEx* pItemSlot = new CItemSlotEx;
	pItemSlot->CreateSub ( this, strKeyword.GetString(), UI_FLAG_DEFAULT, ControlID );
	pItemSlot->CreateSubTradeInventory( EM_INVENSIZE_X );
	RegisterControl( pItemSlot );

	return pItemSlot;
}

CUIControl*	CRebuildInventoryPage::CreateControl( const TCHAR* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub( this, szControl );
	RegisterControl( pControl );
	return pControl;
}

VOID CRebuildInventoryPage::Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl )
{
	SetItemIndex( nOUTOFRANGE, nOUTOFRANGE );

	CUIGroup::Update( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	UpdateBLOCK();

	/*
	if( m_pScrollBar )
	{
		CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame();
		INT nTotal = pThumbFrame->GetTotal();

		if( nTotal < m_nONE_VIEW_SLOT )
			return;

		const INT nViewPerPage = pThumbFrame->GetViewPerPage();
		if( nViewPerPage <= nTotal )
		{
			INT nCurPos = nSTARTLINE;
			const INT nMovableLine = nTotal - nViewPerPage;
			FLOAT fPercent = pThumbFrame->GetPercent();

			nCurPos = (INT)floor( fPercent * nMovableLine );
			if( nCurPos < nSTARTLINE ) nCurPos = nSTARTLINE;

			if ( m_nCurPos == nCurPos ) return;
			
			m_nCurPos = nCurPos;

			ResetAllItemSlotRender( nTotal );    
			SetItemSlotRender( nCurPos, nCurPos + nViewPerPage );
		}
	}
	*/
}

VOID CRebuildInventoryPage::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	if( ITEM_SLOT0 <= ControlID && ControlID < ITEM_SLOT_END )
	{
		if( CHECK_MOUSE_IN( dwMsg ) )
		{
			const INT nPosY = ControlID - ITEM_SLOT0;
			CItemSlot* pSlot = m_pItemSlotArray[ nPosY ];
			const INT nPosX = pSlot->GetItemIndex();

			if( nPosY < 0 || nPosX < 0 )
				return;

			SetItemIndex( nPosX, nPosY );
		}
	}

	switch ( ControlID )
	{
	case PAGE_BUTTON0:
		{	
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	OpenPage( 0 );
		}break;
	case PAGE_BUTTON1:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	OpenPage( 1 );
		}break;
	case PAGE_BUTTON2:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	OpenPage( 2 );
		}break;
	}
}

void CRebuildInventoryPage::ResetAllItemSlotRender ( int index, int nTotal )
{
	if ( nTotal < 0 ) return ;

	for ( int i = index; i < nTotal; i++ )
	{
		CItemSlot* pItemSlot = m_pItemSlotArray[i];
		if ( pItemSlot )
		{
			pItemSlot->SetVisibleSingle ( FALSE );
		}
	}
}

VOID CRebuildInventoryPage::SetItemSlotRender( INT nStartIndex, INT nTotal )
{
	if( nTotal < 0 )
		return;

	const UIRECT& rcParentPos = GetGlobalPos();

	for( INT i=nStartIndex; i<nTotal; ++i )
	{
		CItemSlotEx* pItemSlot = m_pItemSlotArray[i];
		if( pItemSlot )
		{
			INT nAbsoluteIndex = i - nStartIndex;

			CUIControl* pDummyControl = m_pItemSlotArrayDummy[ nAbsoluteIndex ];
			const UIRECT& rcSlotPos = pDummyControl->GetGlobalPos();
			const UIRECT& rcSlotLocalPos = pDummyControl->GetLocalPos();

			pItemSlot->SetLocalPos( rcSlotLocalPos );
			pItemSlot->SetGlobalPos( rcSlotPos );

			pItemSlot->SetVisibleSingle( TRUE );
		}
	}
}

VOID CRebuildInventoryPage::SetItemIndex( INT nPosX, INT nPosY )
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

VOID CRebuildInventoryPage::GetItemIndex( INT* pnPosX, INT* pnPosY )
{
	*pnPosX = m_nPosX;
	*pnPosY = m_nPosY;
}

VOID CRebuildInventoryPage::LoadItemPage( GLInventory &ItemData )
{
	GLInventory::CELL_MAP* pItemList = ItemData.GetItemList();

	if( pItemList->size() > ( EM_INVENSIZE_X * EM_INVENSIZE_Y ) )
	{
		GASSERT( 0 && "인터페이스 사이즈보다, 데이타크기가 큽니다." );
		return;
	}

	//	삭제
	{
		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for( WORD y=0; y<EM_INVENSIZE_Y; ++y )
		{
			for( WORD x=0; x<EM_INVENSIZE_X; ++x )
			{
				SINVENITEM& ref_InvenItem = GetItem( x, y );
				if( ref_InvenItem.sItemCustom.sNativeID != NATIVEID_NULL() )
				{
					GLInventory::CELL_KEY foundkey( x, y );
					GLInventory::CELL_MAP_ITER found = pItemList->find( foundkey );
					if( found == iter_end )
						UnLoadItem( x, y );
				}
			}
		}
	}

	//	등록
	{
		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for( ; iter != iter_end; ++iter )
		{
			SINVENITEM* pInvenItem = (*iter).second;
			SINVENITEM& ref_InvenItemOld = GetItem( pInvenItem->wPosX, pInvenItem->wPosY );
			if( *pInvenItem != ref_InvenItemOld )
				LoadItem( *pInvenItem );
		}
	}
}

VOID CRebuildInventoryPage::LoadItem( SINVENITEM& ref_InvenItem )
{	
	INT nPosX = ref_InvenItem.wPosX;
	INT nPosY = ref_InvenItem.wPosY;
	m_pItemSlotArray[ nPosY ]->SetItemImage( nPosX, ref_InvenItem );
}

VOID CRebuildInventoryPage::UnLoadItemPage()
{
	for( INT y=0; y<EM_INVENSIZE_Y; ++y )
		for( INT x=0; x<EM_INVENSIZE_X; ++x )
			UnLoadItem( x, y );
}

VOID CRebuildInventoryPage::UnLoadItem( INT nPosX, INT nPosY )
{
	m_pItemSlotArray[ nPosY ]->ResetItemImage( nPosX );
}

VOID CRebuildInventoryPage::ResetAllFlipItem()
{
	for( WORD y=0; y<EM_INVENSIZE_Y; ++y )
		for( WORD x=0; x<EM_INVENSIZE_X; ++x )
			m_pItemSlotArray[y]->SetFlipItem( x, FALSE );
}

VOID CRebuildInventoryPage::UpdateFlipItem( GLInventory& ref_Inventory )
{
	ResetAllFlipItem();

	GLInventory::CELL_MAP_ITER iter = ref_Inventory.GetItemList()->begin();
	GLInventory::CELL_MAP_ITER iter_end = ref_Inventory.GetItemList()->end();

	for( ; iter != iter_end; ++iter )
	{
		SINVENITEM* pInvenItem = (*iter).second;
		if( pInvenItem )
		{
			INT nPosX = pInvenItem->wBackX;
			INT nPosY = pInvenItem->wBackY;
			m_pItemSlotArray[ nPosY ]->SetFlipItem( nPosX, TRUE );
		}
	}
}

VOID CRebuildInventoryPage::UpdateFlipItem( SINVEN_POS& ref_RebuildItem )
{
	ResetAllFlipItem();

	if( ref_RebuildItem.wPosY >= 0 && ref_RebuildItem.wPosY < EM_INVENSIZE_Y )
	{
		if( ref_RebuildItem.wPosX >= 0 && ref_RebuildItem.wPosX < EM_INVENSIZE_X )
			m_pItemSlotArray[ ref_RebuildItem.wPosY ]->SetFlipItem( ref_RebuildItem.wPosX, TRUE );
	}
}

VOID CRebuildInventoryPage::UpdateBLOCK()
{
	const WORD& wAddINVENLINE = GLGaeaClient::GetInstance().GetCharacter()->GetOnINVENLINE();
	INT nUSABLE_INVENLINE = EM_INVEN_DEF_SIZE_Y + wAddINVENLINE;

	if( EM_INVENSIZE_Y < nUSABLE_INVENLINE )
	{		
		GASSERT( 0 && "데이타 오류입니다." );

		nUSABLE_INVENLINE = EM_INVENSIZE_Y_FORCE;
	}

	// 사용 가능
	for( INT i=0; i<nUSABLE_INVENLINE; ++i )
		m_pItemSlotArray[i]->ResetBLOCK();

	// 사용 불가능
	for( INT i=nUSABLE_INVENLINE; i<EM_INVENSIZE_Y; ++i )
		m_pItemSlotArray[i]->SetBLOCK();
}

SINVENITEM& CRebuildInventoryPage::GetItem( INT nPosX, INT nPosY )
{
	return m_pItemSlotArray[ nPosY ]->GetItemImage( nPosX );
}

VOID CRebuildInventoryPage::SetOneViewSlot( const INT& nMAX_ONE_VIEW )
{
	m_nONE_VIEW_SLOT = nMAX_ONE_VIEW;
	m_nCurPos = -1;
}

void CRebuildInventoryPage::OpenPage ( int nPage )
{
	for ( int i = 0 ; i < nMAX_PAGES; ++i )	
	{
		m_pPages[i]->SetFlip ( false );
	}

	m_pPages[nPage]->SetFlip ( true );

	switch ( nPage )
	{
	case 0:
		{
			ResetAllItemSlotRender ( 10, 30 );
			SetItemSlotRender ( 0, 10 );
		}break;
	case 1:
		{
			ResetAllItemSlotRender ( 0, 10 );
			ResetAllItemSlotRender ( 20, 30 );
			SetItemSlotRender ( 10, 20 );
		}break;
	case 2:
		{
			ResetAllItemSlotRender ( 0, 20 );
			SetItemSlotRender ( 20, 30 );
		}break;
	};
}