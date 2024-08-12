#include "StdAfx.h"
#include "InventoryPage.h"
#include "ItemSlot.h"
#include "BasicScrollBarEx.h"
#include "DxFontMan.h"
#include "../Lib -- ClientUI/Interface/BasicScrollThumbFrame.h"
#include "../Lib -- ClientUI/Interface/BasicTextBox.h"
#include "../Lib -- ClientUI/Interface/BasicButton.h"
#include "GameTextControl.h"
#include "GLGaeaClient.h"
#include "InnerInterface.h"
#include "UITextControl.h"
#include "BasicLineBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CInventoryPage::nSTARTLINE = 0;
const int CInventoryPage::nOUTOFRANGE = -1;

CInventoryPage::CInventoryPage () :
	m_nCurPos ( -1 ),
	m_nONE_VIEW_SLOT ( nONE_VIEW_SLOT_DEFAULT )
{
	memset ( m_pItemSlotArray, 0, sizeof( m_pItemSlotArray ) );
	memset ( m_pItemSlotArrayDummy, 0, sizeof( m_pItemSlotArrayDummy ) );

	//inven page
	for ( int i = 0 ; i < nMAX_PAGES; ++i )	
	{
		m_pPages[i] = NULL;
		m_pPagesText[i] = NULL;
	}
}

CInventoryPage::~CInventoryPage ()
{
}

CUIControl*	CInventoryPage::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

void CInventoryPage::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_FLAG );

	std::string strPages[nMAX_PAGES] = 
	{
		"INVENTORY_WINDOW_RADIO_BUTTON_1",
		"INVENTORY_WINDOW_RADIO_BUTTON_2",
		"INVENTORY_WINDOW_RADIO_BUTTON_3"
	};

	std::string strPagesText[nMAX_PAGES] = 
	{
		"INVENTORY_WINDOW_RADIO_BUTTON_TEXT_1",
		"INVENTORY_WINDOW_RADIO_BUTTON_TEXT_2",
		"INVENTORY_WINDOW_RADIO_BUTTON_TEXT_3"
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



	CString strSlotKeyword[nMAX_ONE_VIEW_SLOT] =
	{
		"INVENTORY_ITEM_SLOT0",
		"INVENTORY_ITEM_SLOT1",
		"INVENTORY_ITEM_SLOT2",
		"INVENTORY_ITEM_SLOT3",
		"INVENTORY_ITEM_SLOT4",
		"INVENTORY_ITEM_SLOT5",
		"INVENTORY_ITEM_SLOT6",
		"INVENTORY_ITEM_SLOT7",
		"INVENTORY_ITEM_SLOT8",
		"INVENTORY_ITEM_SLOT9"
	};

	for ( int i = 0; i < nMAX_ONE_VIEW_SLOT; i++ )
	{
		m_pItemSlotArrayDummy[i] = CreateControl ( strSlotKeyword[i].GetString () );
	}
	
	for ( int i = 0; i < EM_INVENSIZE_Y_FORCE; i++ )
	{
		m_pItemSlotArray[i] = CreateItemSlot ( strSlotKeyword[0], ITEM_SLOT0 + i );
	}

	SetItemSlotRender ( nSTARTLINE, nMAX_ONE_VIEW_SLOT );
	OpenPage ( 0 );
}

void CInventoryPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		OpenPage( 0 );
	}
}

void CInventoryPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	SetItemIndex ( nOUTOFRANGE, nOUTOFRANGE );
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
	UpdateBLOCK ();
}

void CInventoryPage::ResetAllItemSlotRender ( int index, int nTotal )
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

void CInventoryPage::SetItemSlotRender ( int nStartIndex, int nTotal )
{
	if ( nTotal < 0 ) return ;

	const UIRECT& rcParentPos = GetGlobalPos ();

	for ( int i = nStartIndex; i < nTotal; i++ )
	{
		CItemSlot* pItemSlot = m_pItemSlotArray[i];
		if ( pItemSlot )
		{
			int nAbsoluteIndex = i - nStartIndex;

			CUIControl* pDummyControl = m_pItemSlotArrayDummy[nAbsoluteIndex];			
			const UIRECT& rcSlotPos = pDummyControl->GetGlobalPos ();
			const UIRECT& rcSlotLocalPos = pDummyControl->GetLocalPos ();

			pItemSlot->SetLocalPos ( rcSlotLocalPos );
			pItemSlot->SetGlobalPos ( rcSlotPos );

			pItemSlot->SetVisibleSingle ( TRUE );
		}
	}
}

CItemSlot* CInventoryPage::CreateItemSlot ( CString strKeyword, UIGUID ControlID )
{
	CItemSlot* pItemSlot = new CItemSlot;
	pItemSlot->CreateSub ( this, strKeyword.GetString (), UI_FLAG_DEFAULT, ControlID );
	pItemSlot->CreateSubControl ( EM_INVENSIZE_X, TRUE );	
	RegisterControl ( pItemSlot );
	return pItemSlot;
}


void CInventoryPage::SetItemIndex ( int nPosX, int nPosY )
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

void CInventoryPage::GetItemIndex ( int* pnPosX, int* pnPosY )
{
	*pnPosX = m_nPosX;
	*pnPosY = m_nPosY;
}

void CInventoryPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	if ( ITEM_SLOT0 <= ControlID && ControlID < ITEM_SLOT_END )
	{
		if ( CHECK_MOUSE_IN ( dwMsg ) )
		{
			const int nPosY = ControlID - ITEM_SLOT0;
			CItemSlot* pSlot = m_pItemSlotArray[nPosY];
			const int nPosX = pSlot->GetItemIndex ();

			if ( nPosY < 0 || nPosX < 0 ) return ;

			SetItemIndex ( nPosX, nPosY );
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
/*
	case HELP_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )	
			{
				std::vector<CString> vecText;
				vecText.clear();

				for ( int i=0; i<5; ++i )
				{
					CString strTemp;
					strTemp.Format("%s", ID2GAMEINTEXT( "INVENTORY_HELP_INVEN", i )  );
					vecText.push_back( strTemp );
				}
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO_VECTOR( vecText , NS_UITEXTCOLOR::WHITE );
			}
		}break;
*/
	};
}

void CInventoryPage::LoadItem ( SINVENITEM& ref_InvenItem )
{	
	int nPosX = ref_InvenItem.wPosX;
	int nPosY = ref_InvenItem.wPosY;
	m_pItemSlotArray[nPosY]->SetItemImage ( nPosX, ref_InvenItem );
}

void CInventoryPage::UnLoadItemPage ()
{
	for ( int y = 0; y < EM_INVENSIZE_Y_FORCE; y++ )
	{
		for ( int x = 0; x < EM_INVENSIZE_X; x++ )
		{
			UnLoadItem ( x, y );
		}
	}
}

SINVENITEM& CInventoryPage::GetItem ( int nPosX, int nPosY )
{
	return m_pItemSlotArray[nPosY]->GetItemImage ( nPosX );
}

void CInventoryPage::UnLoadItem ( int nPosX, int nPosY )
{
	m_pItemSlotArray[nPosY]->ResetItemImage ( nPosX );
}

void CInventoryPage::LoadItemPage ( GLInventory &ItemData )
{
	GLInventory::CELL_MAP *pItemList = ItemData.GetItemList();
	
	if ( pItemList->size () > (EM_INVENSIZE_Y_FORCE * EM_INVENSIZE_X) )
	{
		GASSERT ( 0 && "인터페이스 사이즈보다, 데이타크기가 큽니다." );
		return ;
	}

	{
		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for ( WORD y = 0; y < EM_INVENSIZE_Y_FORCE; y++ )
		{
			for ( WORD x = 0; x < EM_INVENSIZE_X; x++ )
			{
				SINVENITEM& ref_InvenItem = GetItem ( x, y );
				if ( ref_InvenItem.sItemCustom.sNativeID != NATIVEID_NULL() )
				{
					GLInventory::CELL_KEY foundkey ( x, y );
					GLInventory::CELL_MAP_ITER found = pItemList->find ( foundkey );
					if ( found == iter_end )
					{
						UnLoadItem ( x, y );
					}
				}
			}
		}
	}

	{
		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for ( ; iter!=iter_end; ++iter )
		{
			SINVENITEM* pInvenItem = (*iter).second;
			SINVENITEM& ref_InvenItemOld = GetItem ( pInvenItem->wPosX, pInvenItem->wPosY );
			if ( pInvenItem->sItemCustom != ref_InvenItemOld.sItemCustom )
			{
				LoadItem ( *pInvenItem );
			}	
		}
	}
}

void CInventoryPage::UpdateBLOCK ()
{
	const WORD& wAddINVENLINE = GLGaeaClient::GetInstance().GetCharacter()->GetOnINVENLINE();
	int nUSABLE_INVENLINE = EM_INVEN_DEF_SIZE_Y + wAddINVENLINE;

	if ( EM_INVENSIZE_Y_FORCE < nUSABLE_INVENLINE )
	{		
		GASSERT ( 0 && "데이타 오류입니다." );

		nUSABLE_INVENLINE = EM_INVENSIZE_Y_FORCE;
	}
	
	//	사용 가능
	for ( int i = 0; i < nUSABLE_INVENLINE; i++ )
		m_pItemSlotArray[i]->ResetBLOCK ();

	//	사용 불가능
	for ( int i = nUSABLE_INVENLINE; i < EM_INVENSIZE_Y; i++ )
		m_pItemSlotArray[i]->SetBLOCK ();
}

void CInventoryPage::SetOneViewSlot ( const int& nMAX_ONE_VIEW )
{
	m_nONE_VIEW_SLOT = nMAX_ONE_VIEW;
	m_nCurPos = -1;
}

void CInventoryPage::OpenPage ( int nPage )
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